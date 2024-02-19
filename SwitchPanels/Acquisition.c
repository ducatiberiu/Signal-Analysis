#include <ansi_c.h>
#include <utility.h>
#include <cvirte.h>		
#include <userint.h>
#include <analysis.h>
#include "SwitchPanels.h"
#include "Acquisition.h"

#define WINTYPE_BLKHARRIS	0  /*Fereastra Blackman-Harris*/
#define WINTYPE_WELCH	1      /*Fereastra Welch*/
#define SIGTYPE_BW_LPF	0      /*Filtru Butterworth ordin 6*/
#define SIGTYPE_CH_LPF	1      /*Filtru Chebyshev trce jos pentru 1/3 din spectru*/

double powerPeak = 0.0;
double freqPeak = 0.0;
WindowConst winConst;
double *autoSpectrum;
double *convertedSpectrum;
double df = 0.0;
char unit[100]="V";
int N;

int CVICALLBACK OnFrequencyPanelCB (int panel, int event, void *callbackData,
									int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	}
	return 0;
}

int CVICALLBACK OnSpectrumButtonCB (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{	
	switch (event)
	{
		case EVENT_COMMIT:	
			GetCtrlVal (frqPanel, FREQ_PANEL_IDC_NR_PUNCTE, &N);
			
			//calcularea spectrului pentru semnalul brut
			autoSpectrum = (double*)calloc(N/2,sizeof(double));
			convertedSpectrum = (double*)calloc(npoints6s/12,sizeof(double));	
			
			ScaledWindowEx (waveData, N, RECTANGLE_, 0, &winConst);
			AutoPowerSpectrum (waveData, N, 1.0/sampleRate, autoSpectrum, &df);
			SpectrumUnitConversion (autoSpectrum, N/2, SPECTRUM_POWER, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrum, unit);	
			
			//afisarea semnalului brut 
			DeleteGraphPlot (frqPanel, FREQ_PANEL_IDC_GRAPH_RAW_DATA_FQ, -1, VAL_IMMEDIATE_DRAW);
			PlotY (frqPanel, FREQ_PANEL_IDC_GRAPH_RAW_DATA_FQ, waveData, npoints6s, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_CYAN);
			
			//afisarea spectrului pentru semnalul brut 
			DeleteGraphPlot (frqPanel, FREQ_PANEL_IDC_GRAPH_SPECTRUM, -1, VAL_IMMEDIATE_DRAW);
			PlotY (frqPanel, FREQ_PANEL_IDC_GRAPH_SPECTRUM, convertedSpectrum, npoints6s/12, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			break;
	}
	return 0;
}

int CVICALLBACK OnFilterButtonCB (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	int secunda;
	double* waveDataSec;
	double *APSpectrum;
	double *convSpectrumSec;
	double *filteredSignal;
	int windowType, filterType;
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (frqPanel, FREQ_PANEL_IDC_NR_PUNCTE, &N);
			double window[N]; 
			double signalXwin[N];
			
			filteredSignal = (double*)calloc(N,sizeof(double));
			waveDataSec = (double*)calloc(N, sizeof(double));		
			APSpectrum = (double*)calloc(N/2,sizeof(double));
			convSpectrumSec = (double*)calloc(npoints6s/12,sizeof(double));
			
			GetCtrlVal (frqPanel, FREQ_PANEL_IDC_SECOND, &secunda);
			
			for(int i = 0; i < N; i++)
			{
				if(secunda*npoints6s/6+i<npoints6s)
				{
					waveDataSec[i] = waveData[secunda*npoints6s/6 + i];
				}
				else
				{
					waveDataSec[i] = 0;
				}
			}
			
			//calcularea spectrului pentru semnalul brut in functie de secunda aleasa
			ScaledWindowEx (waveDataSec, N/2, RECTANGLE_, 0, &winConst);
			AutoPowerSpectrum (waveDataSec, N, 1.0/sampleRate, APSpectrum, &df);
			SpectrumUnitConversion (APSpectrum, N/2, SPECTRUM_POWER, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, winConst, convSpectrumSec, unit);
			
			//afisarea spectrului pentru semnalul brut in functie de secunda aleasa
			DeleteGraphPlot (frqPanel, FREQ_PANEL_IDC_GRAPH_SPECTRUM_S, -1, VAL_IMMEDIATE_DRAW);
			PlotY (frqPanel, FREQ_PANEL_IDC_GRAPH_SPECTRUM_S, convSpectrumSec, npoints6s/12, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			LinEv1D (waveDataSec, N, 0, 1, window);
			
			GetCtrlVal (frqPanel, FREQ_PANEL_IDC_WINDOW_TYPE, &windowType);
			GetCtrlVal (frqPanel, FREQ_PANEL_IDC_FILTER_TYPE_FREQ, &filterType);					
			
			switch (windowType)
			{
				case WINTYPE_BLKHARRIS:
					BlkHarrisWin (window, N);					
					break;
				case WINTYPE_WELCH:
					WelchWin (window, N);					
					break;
				default:
					break;
			}			
						
			Mul1D (waveDataSec, window, N, signalXwin);
		
			//afisarea ferestrei in functie de tipul de fereastra ales 
			DeleteGraphPlot (frqPanel, FREQ_PANEL_IDC_GRAPH_WINDOW, -1, VAL_IMMEDIATE_DRAW);
			PlotY (frqPanel, FREQ_PANEL_IDC_GRAPH_WINDOW, window, N, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_DK_CYAN);			
												
			//afisarea semnalului ferestruit
			DeleteGraphPlot (frqPanel, FREQ_PANEL_IDC_GRAPH_WIN_SIG, -1, VAL_IMMEDIATE_DRAW);
			PlotY (frqPanel, FREQ_PANEL_IDC_GRAPH_WIN_SIG, signalXwin, N, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_DK_CYAN);
			
			//calcularea spectrului pentru semnalul ferestruit
			double *winAPSpectrum;
			double *winConvSpectrum;			
			winAPSpectrum = (double*)calloc(N/2,sizeof(double));
			winConvSpectrum = (double*)calloc(npoints6s/12,sizeof(double));				
			AutoPowerSpectrum (signalXwin, N, 1.0/sampleRate, winAPSpectrum, &df);
			PowerFrequencyEstimate (winAPSpectrum, N/2, -1.0, winConst, df, 7, &freqPeak, &powerPeak);
			SpectrumUnitConversion (winAPSpectrum, N/2, SPECTRUM_POWER, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, winConst, winConvSpectrum, unit);
			
			//afisarea valorilor frequencyPeak ºi powerPeak 
			SetCtrlVal (frqPanel, FREQ_PANEL_IDC_FREQ_PEAK, freqPeak);
			SetCtrlVal (frqPanel, FREQ_PANEL_IDC_POWER_PEAK, powerPeak);		
			
			//afisarea spectrului pentru semnalul ferestruit
			DeleteGraphPlot (frqPanel, FREQ_PANEL_IDC_GRAPH_WIN_SPECT, -1, VAL_IMMEDIATE_DRAW);
			PlotY (frqPanel, FREQ_PANEL_IDC_GRAPH_WIN_SPECT, winConvSpectrum, npoints6s/12, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			switch (filterType)
			{
				case SIGTYPE_BW_LPF:
					Bw_LPF (signalXwin, N, npoints6s/6, N/3, 6, filteredSignal);
					break;
				case SIGTYPE_CH_LPF:
					Ch_LPF (signalXwin, N, npoints6s/6, N/3, 0.1, 4, filteredSignal);
					break;
				default:
					break;
			}
			
			//afisarea semnalului filtrat in functie de secunda aleasa
			DeleteGraphPlot (frqPanel, FREQ_PANEL_GRAPH_FILTERED_SIGNAL, -1, VAL_IMMEDIATE_DRAW);
			PlotY (frqPanel, FREQ_PANEL_GRAPH_FILTERED_SIGNAL, filteredSignal, N, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_CYAN);
						
			//calcularea spectrului pentru semnalul filtrat 
			double *filterAPSpectrum;
			double *filterConvSpectrum;			
			filterAPSpectrum = (double*)calloc(N/2,sizeof(double));
			filterConvSpectrum = (double*)calloc(npoints6s/12,sizeof(double));				
			AutoPowerSpectrum (filteredSignal, N, 1.0/sampleRate, filterAPSpectrum, &df);
			SpectrumUnitConversion (filterAPSpectrum, N/2, SPECTRUM_POWER, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, winConst, filterConvSpectrum, unit);
			
			//afisarea spectrului pentru semnalul filtrat 
			DeleteGraphPlot (frqPanel, FREQ_PANEL_GRAPH_FILT_SPECTRUM, -1, VAL_IMMEDIATE_DRAW);
			PlotY (frqPanel, FREQ_PANEL_GRAPH_FILT_SPECTRUM, filterConvSpectrum, npoints6s/12, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
			
			break;
	}
	return 0;
}


int CVICALLBACK OnSaveButtonFreqCB (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	int imghandle, year = 0, month = 0, day = 0, hour = 0, minute = 0, sec = 0;
	char fileName[256] = {0}, fileName1[256] = {0}, fileName2[256] = {0}, fileName3[256] = {0}, fileName4[256] = {0}, fileName5[256] = {0};
	switch (event)
	{
		case EVENT_COMMIT:
			//preluam data si ora
			GetSystemDate (&month, &day, &year);
			GetSystemTime (&hour, &minute, &sec);			
			
			//salvam imaginile
			sprintf (fileName, "Images/Raw_signal-%4d.%2d.%2d_%2d-%2d-%2d.jpeg", year, month, day, hour, minute, sec);			
			GetCtrlDisplayBitmap (panel, FREQ_PANEL_IDC_GRAPH_RAW_DATA_FQ, 1, &imghandle);
			SaveBitmapToJPEGFile (imghandle, fileName, JPEG_PROGRESSIVE, 100);
			
			sprintf (fileName1, "Images/Spectrum_per_seconds_raw_signal-%4d.%2d.%2d_%2d-%2d-%2d.jpeg", year, month, day, hour, minute, sec);			
			GetCtrlDisplayBitmap (panel, FREQ_PANEL_IDC_GRAPH_SPECTRUM_S, 1, &imghandle);
			SaveBitmapToJPEGFile (imghandle, fileName1, JPEG_PROGRESSIVE, 100);
			
			sprintf (fileName2, "Images/Filtered_signal-%4d.%2d.%2d_%2d-%2d-%2d.jpeg", year, month, day, hour, minute, sec);			
			GetCtrlDisplayBitmap (panel, FREQ_PANEL_GRAPH_FILTERED_SIGNAL, 1, &imghandle);
			SaveBitmapToJPEGFile (imghandle, fileName2, JPEG_PROGRESSIVE, 100);
			
			sprintf (fileName3, "Images/Spectrum_of_filtered_signal-%4d.%2d.%2d_%2d-%2d-%2d.jpeg", year, month, day, hour, minute, sec);			
			GetCtrlDisplayBitmap (panel, FREQ_PANEL_GRAPH_FILT_SPECTRUM, 1, &imghandle);
			SaveBitmapToJPEGFile (imghandle, fileName3, JPEG_PROGRESSIVE, 100);
			
			sprintf (fileName4, "Images/Windowed_signal-%4d.%2d.%2d_%2d-%2d-%2d.jpeg", year, month, day, hour, minute, sec);			
			GetCtrlDisplayBitmap (panel, FREQ_PANEL_IDC_GRAPH_WIN_SIG, 1, &imghandle);
			SaveBitmapToJPEGFile (imghandle, fileName4, JPEG_PROGRESSIVE, 100);
			
			sprintf (fileName5, "Images/Spectrum_of_windowed_signal-%4d.%2d.%2d_%2d-%2d-%2d.jpeg", year, month, day, hour, minute, sec);			
			GetCtrlDisplayBitmap (panel, FREQ_PANEL_IDC_GRAPH_WIN_SPECT, 1, &imghandle);
			SaveBitmapToJPEGFile (imghandle, fileName5, JPEG_PROGRESSIVE, 100);
			
			DiscardBitmap (imghandle);
			break;
	}
	return 0;
}
