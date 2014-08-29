#ifndef _UTILITY_H_
#define _UTILITY_H_
#include "nordic_common.h"

#define CHECK_ERROR(f) do \
                        { \
                            uint32_t err_code = 0; \
                            err_code = f; \
                            APP_ERROR_CHECK(err_code); \
                        } while(0);

#endif
