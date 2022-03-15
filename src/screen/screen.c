// Some of code found from calss website /resources/Fall 2016 Student How-To Guides (for BeagleBone Black)
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

#include "u8g2.h"

#define I2CDRV_LINUX_BUS "/dev/i2c-2"
#define I2C_DEVICE_ADDRESS 0x3c

#define COMMAND_MODE 0x80
#define DATA_MODE 0x40

//Global Var.
static int i2cFileDesc;
// Function to initalize the I2C device for use
// char *bus => the I2C device to use, “/dev/i2c-2”
// int address => the address for the device, 0x3C
int initI2cBus(char *bus, int address) {
    int fd = open(I2CDRV_LINUX_BUS, O_RDWR);
    if(fd < 0) {
    printf("i2c: Unable to open bus for read/write (%s)\n", bus);
        exit(-1);
    }
    int result = ioctl(fd, I2C_SLAVE, address);
    if(result < 0) {
        printf("i2c: Unable to set i2c device to slave address\n");
        exit(-1);
    }
    return fd;
}
// cmd will be a hex code specifying a command defined in the
// display’s datasheet
void sendCommand(unsigned char cmd) {
    char buf[2];
    buf[0] = COMMAND_MODE;
    buf[1] = cmd;
    int result = write(i2cFileDesc, buf, 2);
    if(result != 2) {
        printf("Unable to write to i2c register\n");
        exit(-1);
    }
}

// c will contain the data you wish to display
void sendData(unsigned char c) {
    char buf[2];
    buf[0] = DATA_MODE;
    buf[1] = c;
    int result = write(i2cFileDesc, buf, 2);
    if(result != 2) {
        printf("Error sending data...\n");
        exit(-1);
    }
}

