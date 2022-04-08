// contains some utilitiy functions that can be helpful
#ifndef _UTILITY_H_
#define _UTILITY_H_

// export gpio pin
void export_to_gpio(const char *gpioNum);

// sleep for nanoseconds
void my_sleep_ns(long nanoseconds);

// sleep for miliseconds
void my_sleep_ms(long nanoseconds);

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

#endif