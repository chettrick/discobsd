#ifndef _STDINT_H
#define _STDINT_H

typedef signed char         int8_t;
typedef short int           int16_t;
typedef int                 int32_t;
typedef long long           int64_t;

typedef unsigned char       uint8_t;
typedef unsigned short int  uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long long  uint64_t;

#define UINT32_C(x)         (x##U)
#define UINT64_C(x)         (x##ULL)

#endif /* _STDINT_H */
