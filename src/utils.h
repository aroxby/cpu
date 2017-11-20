#ifndef _INC_UTILS_H
#define _INC_UTILS_H

#include "baseTypes.h"

void *advancePtr(void *vp, SizeType len) {
    return (void*)((unsigned char *)(vp) + len);
}

const void *advancePtr(const void *vp, SizeType len) {
    return (void*)((unsigned char *)(vp) + len);
}

#endif//_INC_UTILS_H