#ifndef STRTAB
#define STRTAB

#include "supervisor.h"

struct StrTab {
    char **content_tab;
    int size;
    int capacity;
};

// void printTabStrTab(StrTab *);

StrTab *create_StrTab(int size);

void printStrTab(StrTab);

int addToStrTab(StrTab *, char *);

#endif