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

// export gpio pin
void export_to_gpio(const char *gpioNum);

// sleep for nanoseconds
void my_sleep_ns(long nanoseconds);

// sleep for miliseconds
void my_sleep_ms(long nanoseconds);

// sleep for seconds
void my_sleep_sec(int seconds);

// write to a file
void file_write(char *filename, const char *content);

// read a int from file
int file_read_int(char *filename);

// initialize the locks
void initialize_my_lock_signal_wait(void);

// wait until other threads signlas
void my_lock_signal_wait(void);

// signal the other threads
void my_lock_signal_signal(void);

// destory the lock
void my_lock_signal_destory(void);

void writeToFile(char *fileName, char *str);

void readFromFileToScreen(char *fileName, char *buf, int buf_size);

// Cancat strings to buffer res.
char* concatStrs(char* str1, char* str2, char* str3, char* res);

#endif