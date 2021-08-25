
#ifndef MEMHIER_H
#define MEMHIER_H
#include "stdio.h"

typedef unsigned int uint;

// Data TLB Config
typedef struct config {
    uint tlbSets;
    uint tlbSetSize;

    // Page Table Config
    uint ptVPages;
    uint ptPPages;
    uint ptPageSize;

    // Data Cache Config
    uint dcSets;
    uint dcSetSize;
    uint dcLineSize;
    char dcWriteThrough;

    // L2 Cache Config
    uint L2Sets;
    uint L2SetSize;
    uint L2LineSize;
    char L2WriteThrough;

    char vAddr;
    char tlb;
    char L2;
} Config;

const uint SSIZE = 100;

void readLine(char *str, FILE *f);

void validateOption(char choice, char *option);

Config *readConf(char *fileName);

void printConf(Config *conf);

#endif