/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  FREQ_PANEL                       1       /* callback function: OnFrequencyPanelCB */
#define  FREQ_PANEL_IDC_SWITCH_PANEL      2       /* control type: binary, callback function: OnSwitchPanelCB */
#define  FREQ_PANEL_IDC_NR_PUNCTE         3       /* control type: ring, callback function: (none) */
#define  FREQ_PANEL_IDC_SPECTRUM_BUTTON   4       /* control type: command, callback function: OnSpectrumButtonCB */
#define  FREQ_PANEL_IDC_FREQ_PEAK         5       /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_IDC_POWER_PEAK        6       /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_IDC_FILTER_BUTTON     7       /* control type: command, callback function: OnFilterButtonCB */
#define  FREQ_PANEL_IDC_GRAPH_RAW_DATA_FQ 8       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_IDC_SECOND            9       /* control type: ring, callback function: (none) */
#define  FREQ_PANEL_IDC_GRAPH_SPECTRUM_S  10      /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_IDC_WINDOW_TYPE       11      /* control type: ring, callback function: (none) */
#define  FREQ_PANEL_IDC_GRAPH_WINDOW      12      /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_IDC_FILTER_TYPE_FREQ  13      /* control type: ring, callback function: (none) */
#define  FREQ_PANEL_GRAPH_FILTERED_SIGNAL 14      /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_GRAPH_FILT_SPECTRUM   15      /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_IDC_GRAPH_WIN_SIG     16      /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_IDC_GRAPH_WIN_SPECT   17      /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_IDC_GRAPH_SPECTRUM    18      /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_DECORATION            19      /* control type: deco, callback function: (none) */
#define  FREQ_PANEL_DECORATION_2          20      /* control type: deco, callback function: (none) */
#define  FREQ_PANEL_DECORATION_3          21      /* control type: deco, callback function: (none) */
#define  FREQ_PANEL_IDC_SAVE_BUTTON_FREQ  22      /* control type: command, callback function: OnSaveButtonFreqCB */
#define  FREQ_PANEL_DECORATION_4          23      /* control type: deco, callback function: (none) */
#define  FREQ_PANEL_DECORATION_5          24      /* control type: deco, callback function: (none) */

#define  WAVE_PANEL                       2       /* callback function: OnWavePanelCB */
#define  WAVE_PANEL_IDC_SWITCH_PANEL      2       /* control type: binary, callback function: OnSwitchPanelCB */
#define  WAVE_PANEL_IDC_GRAPH_FILTER      3       /* control type: graph, callback function: (none) */
#define  WAVE_PANEL_IDC_GRAPH_RAW_DATA    4       /* control type: graph, callback function: (none) */
#define  WAVE_PANEL_IDC_PREV_BUTTON       5       /* control type: command, callback function: OnPrevButton */
#define  WAVE_PANEL_LOAD_BUTTON           6       /* control type: command, callback function: OnLoadButtonCB */
#define  WAVE_PANEL_IDC_NEXT_BUTTON       7       /* control type: command, callback function: OnNextButton */
#define  WAVE_PANEL_IDC_APLICA_BUTTON     8       /* control type: command, callback function: OnApplyButtonCB */
#define  WAVE_PANEL_IDC_STOP              9       /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_IDC_START             10      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_IDC_NUMERIC_MEAN      11      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_FITLER_TYPE           12      /* control type: ring, callback function: OnFilterTypeCB */
#define  WAVE_PANEL_IDC_NUMERIC_MAX       13      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_IDC_NUMERIC_MIN       14      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_IDC_NUMERIC_MEDIAN    15      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_IDC_NUMERIC_VARIANCE  16      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_GRAPH_HISTOGRAMA      17      /* control type: graph, callback function: (none) */
#define  WAVE_PANEL_IDC_NUMERIC_ZERO      18      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_IDC_ALPHA             19      /* control type: scale, callback function: (none) */
#define  WAVE_PANEL_DIM                   20      /* control type: ring, callback function: (none) */
#define  WAVE_PANEL_IDC_NUMERIC_SKEWNESS  21      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_IDC_NUMERIC_KURTOSIS  22      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_IDC_ANVELOPA_BUTTON   23      /* control type: command, callback function: OnEnvelopeButtonCB */
#define  WAVE_PANEL_IDC_SAVE_BUTTON       24      /* control type: command, callback function: OnSaveButtonCB */
#define  WAVE_PANEL_IDC_NUMERIC_MAX_INDEX 25      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_IDC_NUMERIC_MIN_INDEX 26      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_DECORATION_2          27      /* control type: deco, callback function: (none) */
#define  WAVE_PANEL_TEXTMSG               28      /* control type: textMsg, callback function: (none) */
#define  WAVE_PANEL_DECORATION            29      /* control type: deco, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK OnApplyButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnEnvelopeButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFilterButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFilterTypeCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFrequencyPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLoadButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnNextButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPrevButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSaveButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSaveButtonFreqCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSpectrumButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSwitchPanelCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnWavePanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif