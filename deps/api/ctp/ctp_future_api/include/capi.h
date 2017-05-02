/*


 */

#ifndef CTP_CAPI_H_
#define CTP_CAPI_H_

#ifdef __cplusplus
extern "C" {
#endif

// includes

#include "ThostFtdcUserApiStruct.h"

// types

typedef struct ctp_md_spi_t ctp_md_spi_t;
typedef struct ctp_md_api_t ctp_md_api_t;

typedef void (*ctp_md_front_connected_cb)();

// functions

ctp_md_spi_t* ctp_create_md_spi();
void ctp_set_md_front_connected_cb(ctp_md_spi_t* spi,
                                   ctp_md_front_connected_cb cb);

ctp_md_api_t* ctp_create_md_api(const char* flowpath,
                                bool useudp,
                                bool multicast);

#ifdef __cplusplus
} /* end extern "C" */
#endif

#endif  // CTP_CAPI_H_