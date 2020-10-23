#ifndef _M_ZSHA256_H
#define _M_ZSHA256_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <string.h>

#ifdef  __cplusplus
extern "C" {
#endif

int sha256(const uint8_t *src, uint32_t len, uint32_t *hash);

#ifdef  __cplusplus
}
#endif
#endif