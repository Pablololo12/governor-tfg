#include "__cf_quantized_frequency.h"
#include "rtw_capi.h"
#ifdef HOST_CAPI_BUILD
#include "quantized_frequency_capi_host.h"
#define sizeof(s) ((size_t)(0xFFFF))
#undef rt_offsetof
#define rt_offsetof(s,el) ((uint16_T)(0xFFFF))
#define TARGET_CONST
#define TARGET_STRING(s) (s)    
#else
#include "builtin_typeid_types.h"
#include "quantized_frequency.h"
#include "quantized_frequency_capi.h"
#include "quantized_frequency_private.h"
#ifdef LIGHT_WEIGHT_CAPI
#define TARGET_CONST                  
#define TARGET_STRING(s)               (NULL)                    
#else
#define TARGET_CONST                   const
#define TARGET_STRING(s)               (s)
#endif
#endif
static const rtwCAPI_Signals rtBlockSignals [ ] = { { 0 , 1 , TARGET_STRING (
"quantized_frequency/Compute_freq" ) , TARGET_STRING ( "" ) , 0 , 0 , 0 , 0 ,
0 } , { 1 , 0 , TARGET_STRING ( "quantized_frequency/Tdes" ) , TARGET_STRING
( "" ) , 0 , 0 , 0 , 0 , 1 } , { 2 , 0 , TARGET_STRING (
"quantized_frequency/Transfer Fcn" ) , TARGET_STRING ( "" ) , 0 , 0 , 0 , 0 ,
0 } , { 3 , 0 , TARGET_STRING ( "quantized_frequency/Treal" ) , TARGET_STRING
( "" ) , 0 , 0 , 0 , 0 , 0 } , { 4 , 0 , TARGET_STRING (
"quantized_frequency/PID Controller/Filter Coefficient" ) , TARGET_STRING (
"" ) , 0 , 0 , 0 , 0 , 0 } , { 5 , 0 , TARGET_STRING (
"quantized_frequency/PID Controller/Integral Gain" ) , TARGET_STRING ( "" ) ,
0 , 0 , 0 , 0 , 0 } , { 6 , 0 , TARGET_STRING (
"quantized_frequency/PID Controller/Sum" ) , TARGET_STRING ( "" ) , 0 , 0 , 0
, 0 , 0 } , { 0 , 0 , ( NULL ) , ( NULL ) , 0 , 0 , 0 , 0 , 0 } } ; static
const rtwCAPI_BlockParameters rtBlockParameters [ ] = { { 7 , TARGET_STRING (
"quantized_frequency/PID Controller" ) , TARGET_STRING ( "P" ) , 0 , 0 , 0 }
, { 8 , TARGET_STRING ( "quantized_frequency/PID Controller" ) ,
TARGET_STRING ( "I" ) , 0 , 0 , 0 } , { 9 , TARGET_STRING (
"quantized_frequency/PID Controller" ) , TARGET_STRING ( "D" ) , 0 , 0 , 0 }
, { 10 , TARGET_STRING ( "quantized_frequency/PID Controller" ) ,
TARGET_STRING ( "N" ) , 0 , 0 , 0 } , { 11 , TARGET_STRING (
"quantized_frequency/Tdes" ) , TARGET_STRING ( "Time" ) , 0 , 0 , 0 } , { 12
, TARGET_STRING ( "quantized_frequency/Tdes" ) , TARGET_STRING ( "Before" ) ,
0 , 0 , 0 } , { 13 , TARGET_STRING ( "quantized_frequency/Tdes" ) ,
TARGET_STRING ( "After" ) , 0 , 0 , 0 } , { 14 , TARGET_STRING (
"quantized_frequency/Transfer Fcn" ) , TARGET_STRING ( "A" ) , 0 , 0 , 0 } ,
{ 15 , TARGET_STRING ( "quantized_frequency/Transfer Fcn" ) , TARGET_STRING (
"C" ) , 0 , 0 , 0 } , { 16 , TARGET_STRING ( "quantized_frequency/Treal" ) ,
TARGET_STRING ( "DelayTime" ) , 0 , 0 , 0 } , { 17 , TARGET_STRING (
"quantized_frequency/Treal" ) , TARGET_STRING ( "InitialOutput" ) , 0 , 0 , 0
} , { 18 , TARGET_STRING ( "quantized_frequency/PID Controller/Filter" ) ,
TARGET_STRING ( "InitialCondition" ) , 0 , 0 , 0 } , { 19 , TARGET_STRING (
"quantized_frequency/PID Controller/Integrator" ) , TARGET_STRING (
"InitialCondition" ) , 0 , 0 , 0 } , { 0 , ( NULL ) , ( NULL ) , 0 , 0 , 0 }
} ; static const rtwCAPI_ModelParameters rtModelParameters [ ] = { { 0 , (
NULL ) , 0 , 0 , 0 } } ;
#ifndef HOST_CAPI_BUILD
static void * rtDataAddrMap [ ] = { & rtB . clccew1ilz , & rtB . ovfe5mm3fb ,
& rtB . l3gi2ul2wo , & rtB . nwehrftxrj , & rtB . dovl3jdmp1 , & rtB .
g4jyymfj4m , & rtB . jdn0x0is3x , & rtP . PIDController_P , & rtP .
PIDController_I , & rtP . PIDController_D , & rtP . PIDController_N , & rtP .
Tdes_Time , & rtP . Tdes_Y0 , & rtP . Tdes_YFinal , & rtP . TransferFcn_A , &
rtP . TransferFcn_C , & rtP . Treal_Delay , & rtP . Treal_InitOutput , & rtP
. Filter_IC , & rtP . Integrator_IC , } ; static int32_T * rtVarDimsAddrMap [
] = { ( NULL ) } ;
#endif
static TARGET_CONST rtwCAPI_DataTypeMap rtDataTypeMap [ ] = { { "double" ,
"real_T" , 0 , 0 , sizeof ( real_T ) , SS_DOUBLE , 0 , 0 } } ;
#ifdef HOST_CAPI_BUILD
#undef sizeof
#endif
static TARGET_CONST rtwCAPI_ElementMap rtElementMap [ ] = { { ( NULL ) , 0 ,
0 , 0 , 0 } , } ; static const rtwCAPI_DimensionMap rtDimensionMap [ ] = { {
rtwCAPI_SCALAR , 0 , 2 , 0 } } ; static const uint_T rtDimensionArray [ ] = {
1 , 1 } ; static const real_T rtcapiStoredFloats [ ] = { 0.0 , 1.0 } ; static
const rtwCAPI_FixPtMap rtFixPtMap [ ] = { { ( NULL ) , ( NULL ) ,
rtwCAPI_FIX_RESERVED , 0 , 0 , 0 } , } ; static const rtwCAPI_SampleTimeMap
rtSampleTimeMap [ ] = { { ( const void * ) & rtcapiStoredFloats [ 0 ] , (
const void * ) & rtcapiStoredFloats [ 0 ] , 0 , 0 } , { ( const void * ) &
rtcapiStoredFloats [ 0 ] , ( const void * ) & rtcapiStoredFloats [ 1 ] , 1 ,
0 } } ; static rtwCAPI_ModelMappingStaticInfo mmiStatic = { { rtBlockSignals
, 7 , ( NULL ) , 0 , ( NULL ) , 0 } , { rtBlockParameters , 13 ,
rtModelParameters , 0 } , { ( NULL ) , 0 } , { rtDataTypeMap , rtDimensionMap
, rtFixPtMap , rtElementMap , rtSampleTimeMap , rtDimensionArray } , "float"
, { 3735422752U , 3491485978U , 678185484U , 4274392443U } , ( NULL ) , 0 , 0
} ; const rtwCAPI_ModelMappingStaticInfo *
quantized_frequency_GetCAPIStaticMap ( ) { return & mmiStatic ; }
#ifndef HOST_CAPI_BUILD
void quantized_frequency_InitializeDataMapInfo ( SimStruct * const rtS ) {
rtwCAPI_SetVersion ( ( * rt_dataMapInfoPtr ) . mmi , 1 ) ;
rtwCAPI_SetStaticMap ( ( * rt_dataMapInfoPtr ) . mmi , & mmiStatic ) ;
rtwCAPI_SetLoggingStaticMap ( ( * rt_dataMapInfoPtr ) . mmi , ( NULL ) ) ;
rtwCAPI_SetDataAddressMap ( ( * rt_dataMapInfoPtr ) . mmi , rtDataAddrMap ) ;
rtwCAPI_SetVarDimsAddressMap ( ( * rt_dataMapInfoPtr ) . mmi ,
rtVarDimsAddrMap ) ; rtwCAPI_SetInstanceLoggingInfo ( ( * rt_dataMapInfoPtr )
. mmi , ( NULL ) ) ; rtwCAPI_SetChildMMIArray ( ( * rt_dataMapInfoPtr ) . mmi
, ( NULL ) ) ; rtwCAPI_SetChildMMIArrayLen ( ( * rt_dataMapInfoPtr ) . mmi ,
0 ) ; }
#else
#ifdef __cplusplus
extern "C" {
#endif
void quantized_frequency_host_InitializeDataMapInfo (
quantized_frequency_host_DataMapInfo_T * dataMap , const char * path ) {
rtwCAPI_SetVersion ( dataMap -> mmi , 1 ) ; rtwCAPI_SetStaticMap ( dataMap ->
mmi , & mmiStatic ) ; rtwCAPI_SetDataAddressMap ( dataMap -> mmi , NULL ) ;
rtwCAPI_SetVarDimsAddressMap ( dataMap -> mmi , NULL ) ; rtwCAPI_SetPath (
dataMap -> mmi , path ) ; rtwCAPI_SetFullPath ( dataMap -> mmi , NULL ) ;
rtwCAPI_SetChildMMIArray ( dataMap -> mmi , ( NULL ) ) ;
rtwCAPI_SetChildMMIArrayLen ( dataMap -> mmi , 0 ) ; }
#ifdef __cplusplus
}
#endif
#endif
