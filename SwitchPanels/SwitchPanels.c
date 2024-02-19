#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include <formatio.h>
#include <analysis.h>
#include "SwitchPanels.h"
#include "Acquisition.h"
#include "toolbox.h"

#define SAMPLE_RATE		0
#define NPOINTS			1

double* filter = 0;
int plotHandle = -1;
double *waveData256 = 0;
double* anvelopa = 0;
int npoints6s = 0;//nr de valori pentru primele 6s

int wavePanel = 0;
frqPanel = 0;

int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements
double sampleRate = 0.0;
int npoints = 0;
double *waveData = 0;

/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
	int error = 0;
	
	nullChk (InitCVIRTE (0, argv, 0));
	errChk (wavePanel = LoadPanel (0, "SwitchPanels.uir", WAVE_PANEL));
	errChk (frqPanel = LoadPanel (0, "SwitchPanels.uir", FREQ_PANEL));
	
	errChk (DisplayPanel (wavePanel));
	errChk (RunUserInterface ());

Error:
	if (wavePanel > 0)
		DiscardPanel (wavePanel);
	return 0;
}

/// HIFN Exit when the user dismisses the panel.
int CVICALLBACK OnWavePanelCB (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	if (event == EVENT_CLOSE)
		QuitUserInterface (0);
	return 0;
}

int CVICALLBACK OnSwitchPanelCB (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(panel == wavePanel)
			{
				SetCtrlVal(frqPanel, FREQ_PANEL_IDC_SWITCH_PANEL, 1);
				DisplayPanel(frqPanel);
				HidePanel(panel);
			}
			else
			{
				SetCtrlVal(wavePanel, FREQ_PANEL_IDC_SWITCH_PANEL, 0);
				DisplayPanel(wavePanel);
				HidePanel(panel);
			}
			break;
	}
	return 0;
}

int CVICALLBACK OnLoadButtonCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	int zero = 0;
	int intervals = 10;
	double axis[100];
	int hist[100];
	int minIndex = 0;
	double minVal = 0.0;
	int maxIndex = 0;
	double maxVal = 0.0;
	double mean = 0.0;
	double median = 0.0;
	double variance = 0.0;
	double skewness = 0.0;
	double kurtosis = 0.0;
	int samples256 = 256;
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			npoints6s = sampleRate*6;//numarul de valori pentru primele 6s
			
			//alocare memorie pentru numarul de puncte
			waveData = (double *) calloc(npoints6s, sizeof(double));
			filter = (double *) calloc(npoints6s, sizeof(double));
			waveData256 = (double *) calloc(samples256, sizeof(double));
			
			//incarcare din fisierul .txt in memorie (vector)
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints6s, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);//citesc din fisier numai primele npoints6s valori si le stochez in waveData	
			
			//afisare pe grapf
			DeleteGraphPlot (panel, WAVE_PANEL_IDC_GRAPH_RAW_DATA, plotHandle, VAL_IMMEDIATE_DRAW);
			PlotY(panel, WAVE_PANEL_IDC_GRAPH_RAW_DATA, waveData, npoints6s, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			MaxMin1D (waveData, npoints6s, &maxVal, &maxIndex, &minVal, &minIndex);//valoare minima-maxima
			Mean(waveData, npoints6s, &mean);//media
			Median (waveData, npoints6s, &median);//mediana
			Variance(waveData, npoints6s, &mean, &variance);//dispersia
						
			Histogram (waveData, npoints6s, minVal-1, maxVal+1, hist, axis, intervals);//histograma
			PlotXY (panel, WAVE_PANEL_GRAPH_HISTOGRAMA, axis, hist, intervals, VAL_DOUBLE, VAL_SSIZE_T, VAL_VERTICAL_BAR, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);			
			
			//skewness si kurtosis
			for(int i = 0; i < samples256; ++i)//salvarea primelor 256 de esantioane
			{
				waveData256[i] = waveData[i];
			}
			Moment (waveData256, samples256, 3, &skewness);
			Moment (waveData256, samples256, 4, &kurtosis);
			SetCtrlVal (panel, WAVE_PANEL_IDC_NUMERIC_SKEWNESS, skewness);//afisez asimetria
			SetCtrlVal (panel, WAVE_PANEL_IDC_NUMERIC_KURTOSIS, kurtosis);//afisez aplatizarea
			
			//numarul de treceri prin zero
			for(int i = 0; i < npoints6s-1; ++i)
			{
				if(((waveData[i]<0) && (waveData[i+1]>0)) || ((waveData[i]>0) && (waveData[i+1]<0)))
				{
					zero++;
				}
			}				
			SetCtrlVal (panel, WAVE_PANEL_IDC_NUMERIC_ZERO, zero);
			
			SetCtrlVal (panel, WAVE_PANEL_IDC_NUMERIC_MIN, minVal);
			SetCtrlVal (panel, WAVE_PANEL_IDC_NUMERIC_MAX, maxVal);
			SetCtrlVal (panel, WAVE_PANEL_IDC_NUMERIC_MAX_INDEX, (double)maxIndex);
			SetCtrlVal (panel, WAVE_PANEL_IDC_NUMERIC_MIN_INDEX, (double)minIndex);
			SetCtrlVal (panel, WAVE_PANEL_IDC_NUMERIC_MEAN, mean);
			SetCtrlVal (panel, WAVE_PANEL_IDC_NUMERIC_MEDIAN, median);
			SetCtrlVal (panel, WAVE_PANEL_IDC_NUMERIC_VARIANCE, variance);
			
			break;
	}
	return 0;
}

double* filtru_mediere(int n)
{
	double suma = 0.0;
	double* filter = (double*)malloc(npoints6s*sizeof(double));
	for(int i = 0; i < n-1; ++i)
	{
		suma = suma+waveData[i];
		filter[i] = suma/(i+1);
	}
	for(int k = n; k < npoints6s; ++k)
	{
		suma = suma-waveData[k-n]+waveData[k];
		filter[k] = suma/n;
	}
	return filter;
}

double* filtru_ordin1(double alpha)
{
	for(int i = 1; i < npoints6s; ++i)
	{
		filter[i] = (1-alpha)*filter[i-1]+alpha*waveData[i];
	}
	return filter;
}


int CVICALLBACK OnFilterTypeCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	double alpha;
	int val, dim;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (wavePanel, WAVE_PANEL_FITLER_TYPE, &val);
			if(val == 0)
			{
				SetCtrlAttribute (wavePanel, WAVE_PANEL_IDC_ALPHA, ATTR_DIMMED, 1);
				SetCtrlAttribute (wavePanel, WAVE_PANEL_DIM, ATTR_DIMMED, 0);
				GetCtrlVal (wavePanel, WAVE_PANEL_DIM, &dim);
				filter = filtru_mediere(dim);
				DeleteGraphPlot (wavePanel, WAVE_PANEL_IDC_GRAPH_FILTER, plotHandle, VAL_IMMEDIATE_DRAW);
				PlotY (wavePanel, WAVE_PANEL_IDC_GRAPH_FILTER, filter, npoints6s, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			}
			else
			{
				SetCtrlAttribute(wavePanel, WAVE_PANEL_DIM, ATTR_DIMMED, 1);
				SetCtrlAttribute (wavePanel, WAVE_PANEL_IDC_ALPHA, ATTR_DIMMED, 0);
				GetCtrlVal (wavePanel, WAVE_PANEL_IDC_ALPHA, &alpha);
				filter = filtru_ordin1(alpha);
				DeleteGraphPlot (wavePanel, WAVE_PANEL_IDC_GRAPH_FILTER, plotHandle, VAL_IMMEDIATE_DRAW);
				PlotY (wavePanel, WAVE_PANEL_IDC_GRAPH_FILTER, filter, npoints6s, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			}
			break;
	}
	return 0;
}



int CVICALLBACK OnNextButton (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	int start = 0, stop = 0;
	double* signal1;
	double* signal2;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(wavePanel, WAVE_PANEL_IDC_START, &start);
			GetCtrlVal(wavePanel, WAVE_PANEL_IDC_STOP, &stop);
			
			signal1 = (double*)calloc(npoints6s/6, sizeof(double));
			signal2 = (double*)calloc(npoints6s/6, sizeof(double));
			if(stop<6)
			{
				++start;
				++stop;
				SetCtrlVal (wavePanel, WAVE_PANEL_IDC_STOP, stop);
				SetCtrlVal (wavePanel, WAVE_PANEL_IDC_START, start);
				for(int i = 0; i < npoints6s/6; ++i)
				{
					signal1[i] = filter[start*npoints6s/6+i];
				}
				for(int i = 0; i < npoints6s/6; ++i)
				{
					signal2[i] = waveData[start*npoints6s/6+i];
				}
				DeleteGraphPlot (wavePanel, WAVE_PANEL_IDC_GRAPH_RAW_DATA, plotHandle, VAL_IMMEDIATE_DRAW);
				PlotY (wavePanel, WAVE_PANEL_IDC_GRAPH_RAW_DATA, signal2, npoints6s/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				
				DeleteGraphPlot (wavePanel, WAVE_PANEL_IDC_GRAPH_FILTER, plotHandle, VAL_IMMEDIATE_DRAW);
				PlotY (wavePanel, WAVE_PANEL_IDC_GRAPH_FILTER, signal1, npoints6s/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			}
			break;
	}
	return 0;
}


int CVICALLBACK OnPrevButton (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	int start = 0, stop = 0;
	double* signal1;
	double* signal2;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(wavePanel, WAVE_PANEL_IDC_START, &start);
			GetCtrlVal(wavePanel, WAVE_PANEL_IDC_STOP, &stop);
			
			signal1 = (double*)calloc(npoints6s/6, sizeof(double));
			signal2 = (double*)calloc(npoints6s/6, sizeof(double));
			if(stop>1)
			{
				--stop;
				--start;
				SetCtrlVal (wavePanel, WAVE_PANEL_IDC_STOP, stop);
				SetCtrlVal (wavePanel, WAVE_PANEL_IDC_START, start);
				for(int i = 0; i < npoints6s/6; ++i)
				{
					signal1[i] = filter[start*npoints6s/6+i];
				}
				for(int i = 0; i < npoints6s/6; ++i)
				{
					signal2[i] = waveData[start*npoints6s/6+i];
				}
				DeleteGraphPlot (wavePanel, WAVE_PANEL_IDC_GRAPH_RAW_DATA, plotHandle, VAL_IMMEDIATE_DRAW);
				PlotY (wavePanel, WAVE_PANEL_IDC_GRAPH_RAW_DATA, signal2, npoints6s/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				
				DeleteGraphPlot (wavePanel, WAVE_PANEL_IDC_GRAPH_FILTER, plotHandle, VAL_IMMEDIATE_DRAW);
				PlotY (wavePanel, WAVE_PANEL_IDC_GRAPH_FILTER, signal1, npoints6s/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			}
			break;
	}
	return 0;
}

int CVICALLBACK OnApplyButtonCB (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			OnFilterTypeCB(WAVE_PANEL, WAVE_PANEL_FITLER_TYPE, EVENT_COMMIT, 0, 0, 0);
			DeleteGraphPlot(panel, WAVE_PANEL_IDC_GRAPH_RAW_DATA, plotHandle, VAL_IMMEDIATE_DRAW);
			PlotY (panel, WAVE_PANEL_IDC_GRAPH_RAW_DATA, waveData, npoints6s, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);		 
			break;
	}
	return 0;
}


int CVICALLBACK OnEnvelopeButtonCB (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			anvelopa = (double *) calloc(npoints6s, sizeof(double));
			
			FileToArray("anvelopa.txt", anvelopa, VAL_DOUBLE, npoints6s, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			DeleteGraphPlot (panel, WAVE_PANEL_IDC_GRAPH_RAW_DATA, plotHandle, VAL_IMMEDIATE_DRAW);
			
			PlotY(panel, WAVE_PANEL_IDC_GRAPH_RAW_DATA, waveData, npoints6s, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			PlotY(panel, WAVE_PANEL_IDC_GRAPH_RAW_DATA, anvelopa, npoints6s, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
			break;
	}
	return 0;
}

int CVICALLBACK OnSaveButtonCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	int bitmapID;
	char fileName[256] = {0}, fileName1[256] = {0};
	switch (event)
	{
		case EVENT_COMMIT:
			sprintf (fileName, "Images/Raw_signal.jpeg");			
			GetCtrlDisplayBitmap (panel, WAVE_PANEL_IDC_GRAPH_RAW_DATA, 1, &bitmapID);
			SaveBitmapToJPEGFile (bitmapID, fileName, JPEG_PROGRESSIVE, 100);
			
			sprintf (fileName1, "Images/Filtered_signal.jpeg");			
			GetCtrlDisplayBitmap (panel, WAVE_PANEL_IDC_GRAPH_FILTER, 1, &bitmapID);
			SaveBitmapToJPEGFile (bitmapID, fileName1, JPEG_PROGRESSIVE, 100);
			
			DiscardBitmap (bitmapID);
			
			break;
	}
	return 0;
}
