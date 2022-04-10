// utils.h
// Module of general purposed utility class
#ifndef _UTILS_H_
#define _UTILS_H_

static inline int min(int a, int b)
{
    if (a > b)
        return b;
    return a;
}

static inline int max(int a, int b)
{
    if (a > b)
        return a;
    return b;
}

void writeToFile(char *fileName, char *str);
void readFromFileToScreen(char *fileName, char *buf, int buf_size);

// Cancat strings to buffer res.
char* concatStrs(char* str1, char* str2, char* str3, char* res);

#endif