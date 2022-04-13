#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "utils.h"

#define MS_TO_NS_MULTIPLE 1000000
#define MAX_READ_LENGTH 1024
#define GPIO_EXPORT_PATH "/sys/class/gpio/export"

static pthread_mutex_t myMutex;

void my_sleep_ns(long nanoseconds) {
    struct timespec reqDelay = {0, nanoseconds}; 
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

void my_sleep_sec(int seconds) {
    struct timespec reqDelay = {seconds, 0};
    nanosleep(&reqDelay, (struct timespec *)NULL);
}

void my_sleep_ms(long nanoseconds) {
    struct timespec reqDelay = {0, nanoseconds * MS_TO_NS_MULTIPLE};
    nanosleep(&reqDelay, (struct timespec *)NULL);
}

void file_write(char *filename, const char *content)
{
    FILE *myFile = fopen(filename, "w");
    if (myFile == NULL)
    {
        printf("ERROR open file %s\n", filename);
        exit(1);
    }

    fprintf(myFile, "%s", content);

    fclose(myFile);
}

int file_read_int(char *filename)
{
    FILE *myFile = fopen(filename, "r");
    if (myFile == NULL)
    {
        printf("ERROR open file %s\n", filename);
        exit(1);
    }

    char buff[MAX_READ_LENGTH];
    char *ret = fgets(buff, MAX_READ_LENGTH, myFile);
    if (ret == NULL) {
        printf("ERROR reading file %s\n", filename);
        fclose(myFile);
        exit(-1);
    }

    fclose(myFile);

    return atoi(buff);
}

void initialize_my_lock_signal_wait() {
    pthread_mutex_init(&myMutex, NULL);
    pthread_mutex_lock(&myMutex);
}

void my_lock_signal_wait() {
    pthread_mutex_lock(&myMutex);
}

void my_lock_signal_signal() {
    pthread_mutex_unlock(&myMutex);
}

void my_lock_signal_destory() {
    pthread_mutex_destroy(&myMutex);
}

void export_to_gpio(const char *gpioNum) {
    file_write(GPIO_EXPORT_PATH, gpioNum);
}

void writeToFile(char *fileName, char *str)
{
    FILE *pFile = fopen(fileName, "w");
    if (pFile == NULL)
    {
        printf("ERROR: Unable to open export file.\n");
        exit(EXIT_FAILURE);
    }

    // Write to data to the file using fprintf()
    int charWritten = fprintf(pFile, "%s", str);
    if (charWritten <= 0)
    {
        printf("ERROR WRITING DATA");
        exit(EXIT_FAILURE);
    }

    fclose(pFile);
}

void readFromFileToScreen(char *fileName, char *buf, int buf_size)
{
    FILE *pFile = fopen(fileName, "r");
    if (pFile == NULL)
    {
        printf("ERROR: Unable to open file (%s) for read\n", fileName);
        exit(EXIT_FAILURE);
    }

    // Read string (line)
    fgets(buf, buf_size, pFile);

    // Close
    fclose(pFile);
}

char* concatStrs(char* str1, char* str2, char* str3, char* res)
{
    strcpy(res, str1);
    strcat(res, str2);
    strcat(res, str3);
    return res;
}