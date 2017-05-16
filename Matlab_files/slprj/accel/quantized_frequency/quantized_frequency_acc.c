#include "__cf_quantized_frequency.h"
#include <math.h>
#include "quantized_frequency_acc.h"
#include "quantized_frequency_acc_private.h"
#include <stdio.h>
#include "slexec_vm_simstruct_bridge.h"
#include "slexec_vm_zc_functions.h"
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
#ifndef __RTW_UTFREE__  
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#endif
boolean_T quantized_frequency_acc_rt_TDelayUpdateTailOrGrowBuf ( int_T *
bufSzPtr , int_T * tailPtr , int_T * headPtr , int_T * lastPtr , real_T
tMinusDelay , real_T * * tBufPtr , real_T * * uBufPtr , real_T * * xBufPtr ,
boolean_T isfixedbuf , boolean_T istransportdelay , int_T * maxNewBufSzPtr )
{ int_T testIdx ; int_T tail = * tailPtr ; int_T bufSz = * bufSzPtr ; real_T
* tBuf = * tBufPtr ; real_T * xBuf = ( NULL ) ; int_T numBuffer = 2 ; if (
istransportdelay ) { numBuffer = 3 ; xBuf = * xBufPtr ; } testIdx = ( tail <
( bufSz - 1 ) ) ? ( tail + 1 ) : 0 ; if ( ( tMinusDelay <= tBuf [ testIdx ] )
&& ! isfixedbuf ) { int_T j ; real_T * tempT ; real_T * tempU ; real_T *
tempX = ( NULL ) ; real_T * uBuf = * uBufPtr ; int_T newBufSz = bufSz + 1024
; if ( newBufSz > * maxNewBufSzPtr ) { * maxNewBufSzPtr = newBufSz ; } tempU
= ( real_T * ) utMalloc ( numBuffer * newBufSz * sizeof ( real_T ) ) ; if (
tempU == ( NULL ) ) { return ( false ) ; } tempT = tempU + newBufSz ; if (
istransportdelay ) tempX = tempT + newBufSz ; for ( j = tail ; j < bufSz ; j
++ ) { tempT [ j - tail ] = tBuf [ j ] ; tempU [ j - tail ] = uBuf [ j ] ; if
( istransportdelay ) tempX [ j - tail ] = xBuf [ j ] ; } for ( j = 0 ; j <
tail ; j ++ ) { tempT [ j + bufSz - tail ] = tBuf [ j ] ; tempU [ j + bufSz -
tail ] = uBuf [ j ] ; if ( istransportdelay ) tempX [ j + bufSz - tail ] =
xBuf [ j ] ; } if ( * lastPtr > tail ) { * lastPtr -= tail ; } else { *
lastPtr += ( bufSz - tail ) ; } * tailPtr = 0 ; * headPtr = bufSz ; utFree (
uBuf ) ; * bufSzPtr = newBufSz ; * tBufPtr = tempT ; * uBufPtr = tempU ; if (
istransportdelay ) * xBufPtr = tempX ; } else { * tailPtr = testIdx ; }
return ( true ) ; } real_T quantized_frequency_acc_rt_TDelayInterpolate (
real_T tMinusDelay , real_T tStart , real_T * tBuf , real_T * uBuf , int_T
bufSz , int_T * lastIdx , int_T oldestIdx , int_T newIdx , real_T initOutput
, boolean_T discrete , boolean_T minorStepAndTAtLastMajorOutput ) { int_T i ;
real_T yout , t1 , t2 , u1 , u2 ; if ( ( newIdx == 0 ) && ( oldestIdx == 0 )
&& ( tMinusDelay > tStart ) ) return initOutput ; if ( tMinusDelay <= tStart
) return initOutput ; if ( ( tMinusDelay <= tBuf [ oldestIdx ] ) ) { if (
discrete ) { return ( uBuf [ oldestIdx ] ) ; } else { int_T tempIdx =
oldestIdx + 1 ; if ( oldestIdx == bufSz - 1 ) tempIdx = 0 ; t1 = tBuf [
oldestIdx ] ; t2 = tBuf [ tempIdx ] ; u1 = uBuf [ oldestIdx ] ; u2 = uBuf [
tempIdx ] ; if ( t2 == t1 ) { if ( tMinusDelay >= t2 ) { yout = u2 ; } else {
yout = u1 ; } } else { real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ;
real_T f2 = 1.0 - f1 ; yout = f1 * u1 + f2 * u2 ; } return yout ; } } if (
minorStepAndTAtLastMajorOutput ) { if ( newIdx != 0 ) { if ( * lastIdx ==
newIdx ) { ( * lastIdx ) -- ; } newIdx -- ; } else { if ( * lastIdx == newIdx
) { * lastIdx = bufSz - 1 ; } newIdx = bufSz - 1 ; } } i = * lastIdx ; if (
tBuf [ i ] < tMinusDelay ) { while ( tBuf [ i ] < tMinusDelay ) { if ( i ==
newIdx ) break ; i = ( i < ( bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } } else { while
( tBuf [ i ] >= tMinusDelay ) { i = ( i > 0 ) ? i - 1 : ( bufSz - 1 ) ; } i =
( i < ( bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } * lastIdx = i ; if ( discrete ) {
double tempEps = ( DBL_EPSILON ) * 128.0 ; double localEps = tempEps *
muDoubleScalarAbs ( tBuf [ i ] ) ; if ( tempEps > localEps ) { localEps =
tempEps ; } localEps = localEps / 2.0 ; if ( tMinusDelay >= ( tBuf [ i ] -
localEps ) ) { yout = uBuf [ i ] ; } else { if ( i == 0 ) { yout = uBuf [
bufSz - 1 ] ; } else { yout = uBuf [ i - 1 ] ; } } } else { if ( i == 0 ) {
t1 = tBuf [ bufSz - 1 ] ; u1 = uBuf [ bufSz - 1 ] ; } else { t1 = tBuf [ i -
1 ] ; u1 = uBuf [ i - 1 ] ; } t2 = tBuf [ i ] ; u2 = uBuf [ i ] ; if ( t2 ==
t1 ) { if ( tMinusDelay >= t2 ) { yout = u2 ; } else { yout = u1 ; } } else {
real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ; real_T f2 = 1.0 - f1 ; yout
= f1 * u1 + f2 * u2 ; } } return ( yout ) ; } static void mdlOutputs (
SimStruct * S , int_T tid ) { real_T m1ala14ge1 ; msyfmklfa1 * _rtB ;
cmohif4hzl * _rtP ; hq1s54pjoh * _rtX ; epliwftoqw * _rtDW ; _rtDW = ( (
epliwftoqw * ) ssGetRootDWork ( S ) ) ; _rtX = ( ( hq1s54pjoh * )
ssGetContStates ( S ) ) ; _rtP = ( ( cmohif4hzl * ) ssGetDefaultParam ( S ) )
; _rtB = ( ( msyfmklfa1 * ) _ssGetBlockIO ( S ) ) ; if ( ssIsSampleHit ( S ,
1 , 0 ) ) { _rtDW -> i3l3fcuz52 = ( ssGetTaskTime ( S , 1 ) >= _rtP -> P_0 )
; if ( _rtDW -> i3l3fcuz52 == 1 ) { _rtB -> pu330hditd = _rtP -> P_2 ; } else
{ _rtB -> pu330hditd = _rtP -> P_1 ; } } { real_T * * uBuffer = ( real_T * *
) & _rtDW -> mt55ertkgm . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T
* * ) & _rtDW -> mt55ertkgm . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT (
S ) ; real_T tMinusDelay = simTime - _rtP -> P_3 ; _rtB -> cqkdlvkr0t =
quantized_frequency_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * tBuffer
, * uBuffer , _rtDW -> fsuprnnjnr . CircularBufSize , & _rtDW -> fsuprnnjnr .
Last , _rtDW -> fsuprnnjnr . Tail , _rtDW -> fsuprnnjnr . Head , _rtP -> P_4
, 0 , ( boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S
) == ssGetT ( S ) ) ) ) ; } m1ala14ge1 = _rtB -> pu330hditd - _rtB ->
cqkdlvkr0t ; _rtB -> bu355nem23 = ( _rtP -> P_7 * m1ala14ge1 - _rtX ->
in1y5hcijm ) * _rtP -> P_9 ; _rtB -> ljzo20zp1w = ( _rtP -> P_5 * m1ala14ge1
+ _rtX -> pwmjzlwnod ) + _rtB -> bu355nem23 ; ssCallAccelRunBlock ( S , 0 , 0
, SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 1 , 11 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 1 , 12 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> d5o2my5ui0 = _rtP -> P_10 * m1ala14ge1 ; _rtB
-> gqt3cleinb = 0.0 ; _rtB -> gqt3cleinb += _rtP -> P_12 * _rtX -> i3bcheyv5f
; UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { msyfmklfa1 * _rtB ;
cmohif4hzl * _rtP ; epliwftoqw * _rtDW ; _rtDW = ( ( epliwftoqw * )
ssGetRootDWork ( S ) ) ; _rtP = ( ( cmohif4hzl * ) ssGetDefaultParam ( S ) )
; _rtB = ( ( msyfmklfa1 * ) _ssGetBlockIO ( S ) ) ; { real_T * * uBuffer = (
real_T * * ) & _rtDW -> mt55ertkgm . TUbufferPtrs [ 0 ] ; real_T * * tBuffer
= ( real_T * * ) & _rtDW -> mt55ertkgm . TUbufferPtrs [ 1 ] ; real_T simTime
= ssGetT ( S ) ; _rtDW -> fsuprnnjnr . Head = ( ( _rtDW -> fsuprnnjnr . Head
< ( _rtDW -> fsuprnnjnr . CircularBufSize - 1 ) ) ? ( _rtDW -> fsuprnnjnr .
Head + 1 ) : 0 ) ; if ( _rtDW -> fsuprnnjnr . Head == _rtDW -> fsuprnnjnr .
Tail ) { if ( ! quantized_frequency_acc_rt_TDelayUpdateTailOrGrowBuf ( &
_rtDW -> fsuprnnjnr . CircularBufSize , & _rtDW -> fsuprnnjnr . Tail , &
_rtDW -> fsuprnnjnr . Head , & _rtDW -> fsuprnnjnr . Last , simTime - _rtP ->
P_3 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW ->
fsuprnnjnr . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
fsuprnnjnr . Head ] = simTime ; ( * uBuffer ) [ _rtDW -> fsuprnnjnr . Head ]
= _rtB -> gqt3cleinb ; } UNUSED_PARAMETER ( tid ) ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) { msyfmklfa1 * _rtB ; cmohif4hzl
* _rtP ; hq1s54pjoh * _rtX ; kure3o1bkm * _rtXdot ; _rtXdot = ( ( kure3o1bkm
* ) ssGetdX ( S ) ) ; _rtX = ( ( hq1s54pjoh * ) ssGetContStates ( S ) ) ;
_rtP = ( ( cmohif4hzl * ) ssGetDefaultParam ( S ) ) ; _rtB = ( ( msyfmklfa1 *
) _ssGetBlockIO ( S ) ) ; _rtXdot -> pwmjzlwnod = _rtB -> d5o2my5ui0 ;
_rtXdot -> in1y5hcijm = _rtB -> bu355nem23 ; _rtXdot -> i3bcheyv5f = 0.0 ;
_rtXdot -> i3bcheyv5f += _rtP -> P_11 * _rtX -> i3bcheyv5f ; _rtXdot ->
i3bcheyv5f += _rtB -> dnme4prbdt ; }
#define MDL_ZERO_CROSSINGS
static void mdlZeroCrossings ( SimStruct * S ) { cmohif4hzl * _rtP ;
j42jsjpr0t * _rtZCSV ; _rtZCSV = ( ( j42jsjpr0t * ) ssGetSolverZcSignalVector
( S ) ) ; _rtP = ( ( cmohif4hzl * ) ssGetDefaultParam ( S ) ) ; _rtZCSV ->
exxkbi3cyb = ssGetT ( S ) - _rtP -> P_0 ; } static void mdlInitializeSizes (
SimStruct * S ) { ssSetChecksumVal ( S , 0 , 3819347200U ) ; ssSetChecksumVal
( S , 1 , 780983911U ) ; ssSetChecksumVal ( S , 2 , 625715380U ) ;
ssSetChecksumVal ( S , 3 , 2620704752U ) ; { mxArray * slVerStructMat = NULL
; mxArray * slStrMat = mxCreateString ( "simulink" ) ; char slVerChar [ 10 ]
; int status = mexCallMATLAB ( 1 , & slVerStructMat , 1 , & slStrMat , "ver"
) ; if ( status == 0 ) { mxArray * slVerMat = mxGetField ( slVerStructMat , 0
, "Version" ) ; if ( slVerMat == NULL ) { status = 1 ; } else { status =
mxGetString ( slVerMat , slVerChar , 10 ) ; } } mxDestroyArray ( slStrMat ) ;
mxDestroyArray ( slVerStructMat ) ; if ( ( status == 1 ) || ( strcmp (
slVerChar , "8.6" ) != 0 ) ) { return ; } } ssSetOptions ( S ,
SS_OPTION_EXCEPTION_FREE_CODE ) ; if ( ssGetSizeofDWork ( S ) != sizeof (
epliwftoqw ) ) { ssSetErrorStatus ( S ,
"Unexpected error: Internal DWork sizes do "
"not match for accelerator mex file." ) ; } if ( ssGetSizeofGlobalBlockIO ( S
) != sizeof ( msyfmklfa1 ) ) { ssSetErrorStatus ( S ,
"Unexpected error: Internal BlockIO sizes do "
"not match for accelerator mex file." ) ; } { int ssSizeofParams ;
ssGetSizeofParams ( S , & ssSizeofParams ) ; if ( ssSizeofParams != sizeof (
cmohif4hzl ) ) { static char msg [ 256 ] ; sprintf ( msg ,
"Unexpected error: Internal Parameters sizes do "
"not match for accelerator mex file." ) ; } } _ssSetDefaultParam ( S , (
real_T * ) & gjtgce4ujq ) ; rt_InitInfAndNaN ( sizeof ( real_T ) ) ; } static
void mdlInitializeSampleTimes ( SimStruct * S ) { { SimStruct * childS ;
SysOutputFcn * callSysFcns ; childS = ssGetSFunction ( S , 0 ) ; callSysFcns
= ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; } } static void mdlTerminate ( SimStruct * S ) { }
#include "simulink.c"
