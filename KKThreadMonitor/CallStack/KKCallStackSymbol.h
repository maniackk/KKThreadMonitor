//
//  KKCallStackSymbol.h
//  KKMagicHook
//
//  Created by 吴凯凯 on 2020/4/9.
//  Copyright © 2020 吴凯凯. All rights reserved.
//

#ifndef KKCallStackSymbol_h
#define KKCallStackSymbol_h

#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint64_t addr;
    uint64_t offset;
    const char *symbol;
    const char *machOName;
} FuncInfo;

typedef struct {
    FuncInfo *stacks;
    int allocLength;
    int length;
}CallStackInfo;

void callStackOfSymbol(uintptr_t *pcArr, int arrLen, CallStackInfo *csInfo);

#endif /* KKCallStackSymbol_h */
