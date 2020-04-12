//
//  KKCallStackSymbol.c
//  KKMagicHook
//
//  Created by 吴凯凯 on 2020/4/9.
//  Copyright © 2020 吴凯凯. All rights reserved.
//

#include "KKCallStackSymbol.h"

#import <mach/mach.h>
#include <stdlib.h>
#include <string.h>
#include <mach-o/dyld.h>
#include <mach-o/nlist.h>

typedef struct {
    const struct mach_header *header;
    const char *name;
    uintptr_t slide;
} KKMachHeader;

typedef struct {
    KKMachHeader *array;
    uint32_t allocLength;
} KKMachHeaderArr;

static KKMachHeaderArr *machHeaderArr = NULL;

void getMachHeader()
{
    machHeaderArr = (KKMachHeaderArr *)malloc(sizeof(KKMachHeaderArr));
    machHeaderArr->allocLength = _dyld_image_count();
    machHeaderArr->array = (KKMachHeader *)malloc(sizeof(KKMachHeader) * machHeaderArr->allocLength);
    for (uint32_t i = 0; i < machHeaderArr->allocLength; i++) {
        KKMachHeader *machHeader = &machHeaderArr->array[i];
        machHeader->header = _dyld_get_image_header(i);
        machHeader->name = _dyld_get_image_name(i);
        machHeader->slide = _dyld_get_image_vmaddr_slide(i);
    }
}

bool pcIsInMach(uintptr_t slidePC, const struct mach_header *header)
{
    uintptr_t cur = (uintptr_t)(((struct mach_header_64*)header) + 1);
    for (uint32_t i = 0; i < header->ncmds; i++) {
        struct load_command *command = (struct load_command *)cur;
        if (command->cmd == LC_SEGMENT_64) {
            struct segment_command_64 *segmentCommand = (struct segment_command_64 *)command;
            uintptr_t start = segmentCommand->vmaddr;
            uintptr_t end = segmentCommand->vmaddr + segmentCommand->vmsize;
            if (slidePC >= start && slidePC <= end) {
                return true;
            }
        }
        cur = cur + command->cmdsize;
    }
    return false;
}

KKMachHeader *getPCInMach(uintptr_t pc)
{
    if (!machHeaderArr) {
        getMachHeader();
    }
    for (uint32_t i = 0; i < machHeaderArr->allocLength; i++) {
        KKMachHeader *machHeader = &machHeaderArr->array[i];
        if (pcIsInMach(pc-machHeader->slide, machHeader->header)) {
            return machHeader;
        }
    }
    return NULL;
}



void findPCSymbolInMach(uintptr_t pc, KKMachHeader *machHeader, CallStackInfo *csInfo)
{
    if (!machHeader) {
        return;
    }
    
    struct segment_command_64 *seg_linkedit = NULL;
    struct symtab_command *sym_command = NULL;
    const struct mach_header *header = machHeader->header;
    uintptr_t cur = (uintptr_t)(((struct mach_header_64*)header) + 1);
    for (uint32_t i = 0; i < header->ncmds; i++) {
        struct load_command *command = (struct load_command *)cur;
        if (command->cmd == LC_SEGMENT_64) {
            struct segment_command_64 *segmentCommand = (struct segment_command_64 *)command;
            if (strcmp(segmentCommand->segname, SEG_LINKEDIT)==0) {
                seg_linkedit = segmentCommand;
            }
        }
        else if (command->cmd == LC_SYMTAB)
        {
            sym_command = (struct symtab_command*)command;
        }
        cur = cur + command->cmdsize;
    }
    if (!seg_linkedit || !sym_command) {
        return;
    }
    
    uintptr_t linkedit_base = (uintptr_t)machHeader->slide + seg_linkedit->vmaddr - seg_linkedit->fileoff;
    struct nlist_64 *symtab = (struct nlist_64 *)(linkedit_base + sym_command->symoff);
    const uintptr_t strtab = linkedit_base + sym_command->stroff;
    
    uintptr_t slidePC = pc - machHeader->slide;
    uint64_t offset = UINT64_MAX;
    int best = -1;
    for (uint32_t i = 0; i < sym_command->nsyms; i++) {
        uint64_t distance = slidePC - symtab[i].n_value;
        if (slidePC >= symtab[i].n_value && distance <= offset) {
            offset = distance;
            best = i;
        }
    }
    
    if (best >= 0) {
        FuncInfo *funcInfo = &csInfo->stacks[csInfo->length++];
        funcInfo->machOName = machHeader->name;
        funcInfo->addr = symtab[best].n_value;
        funcInfo->offset = offset;
        funcInfo->symbol = (char *)(strtab + symtab[best].n_un.n_strx);
        if (*funcInfo->symbol == '_')
        {
            funcInfo->symbol++;
        }
        if (funcInfo->machOName == NULL) {
            funcInfo->machOName = "";
        }
    }
}

void callStackOfSymbol(uintptr_t *pcArr, int arrLen, CallStackInfo *csInfo)
{
    for (int i = 0; i < arrLen; i++) {
        KKMachHeader *machHeader = getPCInMach(pcArr[i]);
        if (machHeader) {
            findPCSymbolInMach(pcArr[i], machHeader, csInfo);
        }
    }
}






