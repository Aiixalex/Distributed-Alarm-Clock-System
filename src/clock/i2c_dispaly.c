#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#include "i2c_display.h"
#include "../utility/utility.h"

#define I2C_DEVICE_ADDRESS 0x20
#define REG_DIRA 0x00
#define REG_DIRB 0x01
#define REG_OUTA 0x14
#define REG_OUTB 0x15
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define SET_PIN_OUTPUT_COMMAND "i2cset -y 1 0x20 0x00 0x00 && i2cset -y 1 0x20 0x01 0x00"
#define GPIO_PATH "/sys/class/gpio/export"
#define GPIO_DISPLAY_PATH_1 "/sys/class/gpio/gpio61/direction"
#define GPIO_DISPLAY_PATH_2 "/sys/class/gpio/gpio44/direction"
#define LEFT_DIGIT_PATH "/sys/class/gpio/gpio61/value"
#define RIGHT_DIGIT_PATH "/sys/class/gpio/gpio44/value"
#define SLEEP_TIME_NANO 5000000

static int firstDigit = 0;
static int secondDigit = 0;
static bool threadStatus = true;
static int i2cFileDesc;
static pthread_t displayThreadID;
static pthread_mutex_t digitsMutex;

static const char bit_pattern[24] = {
    0xA1, 0x86,
    0x80, 0x12,
    0x31, 0x0e,
    0xb0, 0x06,
    0x90, 0x8a,
    0xb0, 0x8c,
    0xb1, 0x8c,
    0x04, 0x14,
    0xb1, 0x8e,
    0x90, 0x8e,
    0x21, 0x84, // C
    0xA0, 0x06, // flipped C
};

void Display_update_number(int number) {
    pthread_mutex_lock(&digitsMutex);
    firstDigit = number / 10;
    secondDigit = number % 10;
    pthread_mutex_unlock(&digitsMutex);
}

void Display_show_rectangle(void) {
    pthread_mutex_lock(&digitsMutex);
    firstDigit = 10;
    secondDigit = 11;
    pthread_mutex_unlock(&digitsMutex);
}

static int initI2cBus(char *bus, int address)
{
    int i2cFileDesc = open(bus, O_RDWR);
    int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
    if (result < 0)
    {
        printf("I2C:  Unable to set I2C device to slave address.\n");
        exit(1);
    }
    return i2cFileDesc;
}

static void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value)
{
    unsigned char buff[2];
    buff[0] = regAddr;
    buff[1] = value;
    int res = write(i2cFileDesc, buff, 2);
    if (res != 2)
    {
        printf("I2C:  Unable to write i2c register.\n");
        exit(1);
    }
}

static void display_number(int number) {
    writeI2cReg(i2cFileDesc, REG_OUTA, bit_pattern[number * 2]);
    writeI2cReg(i2cFileDesc, REG_OUTB, bit_pattern[number * 2 + 1]);
}

static void *display_thread(void *args) {
    while (threadStatus) {
        pthread_mutex_lock(&digitsMutex);
        int left = firstDigit;
        int right = secondDigit;
        pthread_mutex_unlock(&digitsMutex);

        file_write(LEFT_DIGIT_PATH, "0");
        file_write(RIGHT_DIGIT_PATH, "0");
        
        display_number(left);
        file_write(LEFT_DIGIT_PATH, "1");
        my_sleep_ns(SLEEP_TIME_NANO);

        file_write(LEFT_DIGIT_PATH, "0");

        display_number(right);
        file_write(RIGHT_DIGIT_PATH, "1");
        my_sleep_ns(SLEEP_TIME_NANO);
    }

    file_write(LEFT_DIGIT_PATH, "0");
    file_write(RIGHT_DIGIT_PATH, "0");

    pthread_exit(NULL);
}

void Display_start() {
    export_to_gpio("61");
    export_to_gpio("44");
    my_sleep_ms(300);
    file_write(GPIO_DISPLAY_PATH_1, "out");
    file_write(GPIO_DISPLAY_PATH_2, "out");
    if (system(SET_PIN_OUTPUT_COMMAND)) {
        printf("ERROR running %s command\n", SET_PIN_OUTPUT_COMMAND);
        exit(-1);
    }

    i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
    my_sleep_sec(1);

    printf("i2c display initialized\n");
    pthread_mutex_init(&digitsMutex, NULL);
    pthread_create(&displayThreadID, NULL, &display_thread, NULL);
}

void Display_stop() {
    threadStatus = false;
    pthread_join(displayThreadID, NULL);
    pthread_mutex_destroy(&digitsMutex);

    close(i2cFileDesc);
}
