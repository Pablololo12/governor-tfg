#include "__cf_quantized_frequency.h"
#include "rtwtypes.h"
#include <stdio.h>
#include <string.h>
#if defined(_MSC_VER) && (_MSC_VER < 1600)
typedef unsigned short uint16_t ;
#elif defined(__LCC__)
typedef unsigned short uint16_t ;
#else
#include <stdint.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif
uint16_t getCommServicePort ( ) ;
#ifdef __cplusplus
}
#endif
static char serverInfoFileName [ 1024 ] ; static bool
writePortNumToServerInfoFile = false ; static unsigned short port = 17726U ;
#include "ToAsyncQueueTgtAppSvc/ToAsyncQueueTgtAppSvcCIntrf.h"
#include "ParamTuningTgtAppSvc/ParamTuningTgtAppSvcCIntrf.h"
#ifdef __cplusplus
#define EXTERN_C "C"
#else
#define EXTERN_C 
#endif
extern EXTERN_C rtwCAPI_ModelMappingInfo * rt_modelMapInfoPtr ; void
TgtConnBackgroundTask ( ) { } const char * TgtConnInit ( int_T argc , char_T
* argv [ ] ) { const char * result = NULL ; { int_T tvar ; for ( tvar = 1 ;
tvar < argc ; tvar ++ ) { if ( strcmp ( argv [ tvar ] , "-tgtconn_port" ) ==
0 ) { char tmpstr [ 2 ] ; const char * portStr = ( char * ) argv [ tvar + 1 ]
; int itemsConverted = sscanf ( portStr , "%hd%1s" , & port , tmpstr ) ; if (
itemsConverted != 1 || ( ( port != 0 && port < 255 ) || port > 65535 ) ) {
result = "Illegal port value for target connectivity comm service\n" ; return
( result ) ; } argv [ tvar ++ ] = NULL ; argv [ tvar ] = NULL ; continue ; }
if ( strcmp ( argv [ tvar ] , "-tgtconn_server_info_file" ) == 0 ) {
writePortNumToServerInfoFile = true ; sprintf ( serverInfoFileName , "%s" ,
argv [ tvar + 1 ] ) ; argv [ tvar ++ ] = NULL ; argv [ tvar ] = NULL ;
continue ; } } port = getCommServicePort ( ) ; if (
writePortNumToServerInfoFile ) { FILE * fh ; fh = fopen ( serverInfoFileName
, "w" ) ; if ( fh == NULL ) { result =
"Unable to create target connectivity server info file\n" ; return ( result )
; } ( void ) fprintf ( fh , "Server Port Number: %d\n" , ( unsigned int )
port ) ; fclose ( fh ) ; } } if ( startParamTuningTgtAppSvc ( ) ) { result =
"Could not start ParamTuning app service" ; return ( result ) ; } if (
startToAsyncQueueTgtAppSvc ( ) ) { result =
"Could not start ToAsyncQueue app service" ; return ( result ) ; } return (
result ) ; } void TgtConnTerm ( ) { terminateParamTuningTgtAppSvc ( ) ;
terminateToAsyncQueueTgtAppSvc ( ) ; } void TgtConnPreStep ( int_T tid ) {
tunePendingParameterChanges ( rt_modelMapInfoPtr ) ; } void TgtConnPostStep (
int_T tid ) { }
