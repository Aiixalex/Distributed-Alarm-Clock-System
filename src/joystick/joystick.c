#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>
#include <errno.h>
#include "joystick.h"
#include "../utils/utils.h"

#define GPIO_DIRPATH "/sys/class/gpio/gpio"
#define GPIO_EXPORT "/sys/class/gpio/export"
#define JOYSTICK_UP "26"
#define JOYSTICK_DOWN "46"
#define JOYSTICK_RIGHT "47"
#define JOYSTICK_LEFT "65"
#define JOYSTICK_IN "27"

#define MAX_LENGTH 1024

#define JOYSTICK_INTERVAL_TIME 200000 // 200ms
#define POOL_INPUT_TIME 10000 // 10ms
#define EXPORT_GPIO_LATENCY_TIME 330000 // 330ms

static _Bool stopping = false;

static pthread_t joystick_thread_id;

#define JOYSTICK_DIRECTIONS_COUNT 5
static char* joystick_directions[JOYSTICK_DIRECTIONS_COUNT] = 
            {JOYSTICK_UP, JOYSTICK_DOWN, JOYSTICK_RIGHT, JOYSTICK_LEFT, JOYSTICK_IN};

void Joystick_exportAndSetDirectionOfAllGPIOPins()
{
    char buf[MAX_LENGTH];

    for (int i = 0; i < JOYSTICK_DIRECTIONS_COUNT; i++)
    {
        DIR *dir = opendir(concatStrs(GPIO_DIRPATH, joystick_directions[i], "", buf));
        if (errno == ENOENT)
        {
            writeToFile(GPIO_EXPORT, joystick_directions[i]);
        }
        closedir(dir);

        usleep(EXPORT_GPIO_LATENCY_TIME);

        writeToFile(concatStrs(GPIO_DIRPATH, joystick_directions[i], "/direction", buf), "in");
    }
}

enum JoystickDirection Joystick_getDirection()
{
    char buf_filename[MAX_LENGTH];
    char buf_value[MAX_LENGTH];

    for (int i = 0; i < JOYSTICK_DIRECTIONS_COUNT; i++)
    {
        concatStrs(GPIO_DIRPATH, joystick_directions[i], "/value", buf_filename);
        readFromFileToScreen(buf_filename, buf_value, MAX_LENGTH);
        if (strcmp(buf_value, "0\n") == 0)
        {
            return i; // UP or DOWN or RIGHT or LEFT or IN
        }
    }

    return NO_DIRECTION;
}

void *joystickThread()
{
    Joystick_exportAndSetDirectionOfAllGPIOPins();

    while (!stopping)
    {
        enum JoystickDirection joystick_direction = Joystick_getDirection();
        switch (joystick_direction)
        {
        case UP:
            break;
        case DOWN:
            break;
        case RIGHT:
            break;
        case LEFT:
            break;
        case IN:
            break;
        default:
            break;
        }

        if (joystick_direction != NO_DIRECTION)
        {
            usleep(JOYSTICK_INTERVAL_TIME);
        }
        usleep(POOL_INPUT_TIME);
    }

    return NULL;
}

void Joystick_init()
{
    pthread_create(&joystick_thread_id, NULL, &joystickThread, NULL);
}

void Joystick_quit()
{
    pthread_join(joystick_thread_id, NULL);
}