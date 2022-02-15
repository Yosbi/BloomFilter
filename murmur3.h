//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the
// public domain. The author hereby disclaims copyright to this source
// code.

#ifndef _MURMURHASH3_H_
#define _MURMURHASH3_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Modified by Yosbi Alves Saenz
// On: 04/04/2021
//-----------------------------------------------------------------------------
//uint64_t MurmurHash3_x86_64 ( const void * key, int len, uint32_t seed);
uint32_t MurmurHash3_x86_32( const void * key, int len, int seed);

//-----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif // _MURMURHASH3_H_
