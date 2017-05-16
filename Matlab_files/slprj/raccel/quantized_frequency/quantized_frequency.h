#include "__cf_quantized_frequency.h"
#ifndef RTW_HEADER_quantized_frequency_h_
#define RTW_HEADER_quantized_frequency_h_
#include <stddef.h>
#include <float.h>
#include <string.h>
#include "rtw_modelmap.h"
#ifndef quantized_frequency_COMMON_INCLUDES_
#define quantized_frequency_COMMON_INCLUDES_
#include <stdlib.h>
#include "rtwtypes.h"
#include "sigstream_rtw.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "raccel.h"
#include "rt_logging.h"
#include "dt_info.h"
#include "ext_work.h"
#endif
#include "quantized_frequency_types.h"
#include "multiword_types.h"
#include "mwmathutil.h"
#include "rt_defines.h"
#include "rtGetInf.h"
#include "rt_nonfinite.h"
#define MODEL_NAME quantized_frequency
#define NSAMPLE_TIMES (2) 
#define NINPUTS (0)       
#define NOUTPUTS (0)     
#define NBLOCKIO (7) 
#define NUM_ZC_EVENTS (0) 
#ifndef NCSTATES
#define NCSTATES (3)   
#elif NCSTATES != 3
#error Invalid specification of NCSTATES defined in compiler command
#endif
#ifndef rtmGetDataMapInfo
#define rtmGetDataMapInfo(rtm) (*rt_dataMapInfoPtr)
#endif
#ifndef rtmSetDataMapInfo
#define rtmSetDataMapInfo(rtm, val) (rt_dataMapInfoPtr = &val)
#endif
#ifndef IN_RACCEL_MAIN
#endif
typedef struct { real_T ovfe5mm3fb ; real_T nwehrftxrj ; real_T dovl3jdmp1 ;
real_T jdn0x0is3x ; real_T g4jyymfj4m ; real_T l3gi2ul2wo ; real_T clccew1ilz
; } B ; typedef struct { struct { real_T modelTStart ; } bj2lsztjg1 ; struct
{ void * TUbufferPtrs [ 2 ] ; } mt55ertkgm ; struct { void * LoggedData [ 2 ]
; } c5wdu3igvz ; struct { void * LoggedData ; } jcyoaboq2x ; struct { int_T
Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize
; } fsuprnnjnr ; int_T i3l3fcuz52 ; } DW ; typedef struct { real_T pwmjzlwnod
; real_T in1y5hcijm ; real_T i3bcheyv5f ; } X ; typedef struct { real_T
pwmjzlwnod ; real_T in1y5hcijm ; real_T i3bcheyv5f ; } XDot ; typedef struct
{ boolean_T pwmjzlwnod ; boolean_T in1y5hcijm ; boolean_T i3bcheyv5f ; } XDis
; typedef struct { real_T pwmjzlwnod ; real_T in1y5hcijm ; real_T i3bcheyv5f
; } CStateAbsTol ; typedef struct { real_T exxkbi3cyb ; } ZCV ; typedef
struct { rtwCAPI_ModelMappingInfo mmi ; } DataMapInfo ; struct P_ { real_T
PIDController_D ; real_T PIDController_I ; real_T PIDController_N ; real_T
PIDController_P ; real_T Tdes_Time ; real_T Tdes_Y0 ; real_T Tdes_YFinal ;
real_T Treal_Delay ; real_T Treal_InitOutput ; real_T Integrator_IC ; real_T
Filter_IC ; real_T TransferFcn_A ; real_T TransferFcn_C ; } ; extern P rtP ;
extern const char * RT_MEMORY_ALLOCATION_ERROR ; extern B rtB ; extern X rtX
; extern DW rtDW ; extern const rtwCAPI_ModelMappingStaticInfo *
quantized_frequency_GetCAPIStaticMap ( void ) ; extern SimStruct * const rtS
; extern const int_T gblNumToFiles ; extern const int_T gblNumFrFiles ;
extern const int_T gblNumFrWksBlocks ; extern rtInportTUtable *
gblInportTUtables ; extern const char * gblInportFileName ; extern const
int_T gblNumRootInportBlks ; extern const int_T gblNumModelInputs ; extern
const int_T gblInportDataTypeIdx [ ] ; extern const int_T gblInportDims [ ] ;
extern const int_T gblInportComplex [ ] ; extern const int_T
gblInportInterpoFlag [ ] ; extern const int_T gblInportContinuous [ ] ;
extern const int_T gblParameterTuningTid ; extern DataMapInfo *
rt_dataMapInfoPtr ; extern rtwCAPI_ModelMappingInfo * rt_modelMapInfoPtr ;
void MdlOutputs ( int_T tid ) ; void MdlOutputsParameterSampleTime ( int_T
tid ) ; void MdlUpdate ( int_T tid ) ; void MdlTerminate ( void ) ; void
MdlInitializeSizes ( void ) ; void MdlInitializeSampleTimes ( void ) ;
SimStruct * raccel_register_model ( void ) ;
#endif
