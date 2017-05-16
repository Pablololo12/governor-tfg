#include "__cf_quantized_frequency.h"
#ifndef _RTW_HEADER_quantized_frequency_cap_host_h__
#define _RTW_HEADER_quantized_frequency_cap_host_h__
#ifdef HOST_CAPI_BUILD
#include "rtw_capi.h"
#include "rtw_modelmap.h"
typedef struct { rtwCAPI_ModelMappingInfo mmi ; }
quantized_frequency_host_DataMapInfo_T ;
#ifdef __cplusplus
extern "C" {
#endif
void quantized_frequency_host_InitializeDataMapInfo (
quantized_frequency_host_DataMapInfo_T * dataMap , const char * path ) ;
#ifdef __cplusplus
}
#endif
#endif
#endif
