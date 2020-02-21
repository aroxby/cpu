#ifndef _INC_BASETYPES_H
#define _INC_BASETYPES_H

// It's very very hard to mock non-virtual methods so I just make some of them virtual in tests
#ifdef GTEST_TEST
    #ifndef _TEST
        #define _TEST
    #endif
#endif
#ifdef _TEST
    #define MOCKABLE virtual
#else
    #define MOCKABLE
#endif

typedef unsigned long long SizeType;
typedef unsigned char byte;
typedef SizeType Interrupt;

#endif//_INC_BASETYPES_H