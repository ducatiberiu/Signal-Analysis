#ifndef __Acquisition_H__
#define __Acquisition_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include "cvidef.h"

int frqPanel;
double *waveData;
double sampleRate;
int npoints6s;

int Declare_Your_Functions_Here (int x);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __Acquisition_H__ */
