// wave player implementation
#include "audioMixer.h"
#include "../utils/utils.h"
#include "wave_player.h"
#include <stdio.h>
#include <pthread.h>

// File used for play-back:
// If cross-compiling, must have this file available, via this relative path,
// on the target when the application is run. This example's Makefile copies the wave-files/
// folder along with the executable to ensure both are present.
#define BPM_SLEEP_TIME(x) ((60.0/(x)/2.0)*1000.0)
#define ALARM_FILE_PATH "alarm_sound.wav" 
#define NUM_MODES 3
#define VOLUMN_CHANGE_VALUE 5
#define BPM_CHANGE_VALUE 5
#define MAX_BPM 300
#define MIN_BPM 40

static bool shouldPlay = false;
static int volumn = 0;
static int BPM = 180;
static int sleep_time_ms = BPM_SLEEP_TIME(150);
static pthread_t threadID;
static wavedata_t alarm_wavedata;

static void play_alarm() {
    AudioMixer_queueSound(&alarm_wavedata);
    my_sleep_sec(3);
}

static void *player_thread(void *args) {
	while (shouldPlay) {
        play_alarm();
    }

    return 0;
}

void WavePlayer_init() {
    AudioMixer_init();
    AudioMixer_readWaveFileIntoMemory(ALARM_FILE_PATH, &alarm_wavedata);
    volumn = AudioMixer_getVolume();
}

void WavePlayer_start() {
    shouldPlay = true;
    pthread_create(&threadID, NULL, &player_thread, NULL);
}

void WavePlayer_stop() {
    shouldPlay = false;
    pthread_join(threadID, NULL);
}

void WavePlayer_destory() {
    shouldPlay = false;
    pthread_join(threadID, NULL);
    AudioMixer_freeWaveFileData(&alarm_wavedata);
    AudioMixer_cleanup();
}

bool WavePlayer_setVolumn(int myVolumn) {
    if (myVolumn >= 0 && myVolumn <= 100) {
        printf("Volumn: %d\n", myVolumn);
        volumn = myVolumn;
        AudioMixer_setVolumn(myVolumn);
        return true;
    }

    return false;
}

void WavePlayer_raiseVolumn() {
    if (volumn != 100) {
        volumn += VOLUMN_CHANGE_VALUE;
        if (volumn > 100)
            volumn = 100;

        printf("Volumn: %d\n", volumn);
        AudioMixer_setVolumn(volumn);
    }
}

void WavePlayer_lowerVolumn() {
    if (volumn != 0) {
        volumn -= VOLUMN_CHANGE_VALUE;
        if (volumn < 0)
            volumn = 0;

        printf("Volumn: %d\n", volumn);
        AudioMixer_setVolumn(volumn);
    }
}

bool WavePlayer_setBPM(int myBPM){
    if (myBPM >= MIN_BPM && myBPM <= MAX_BPM)
    {
        BPM = myBPM;
        sleep_time_ms = BPM_SLEEP_TIME(BPM);
        printf("BPM: %d\n", BPM);

        return true;
    }

    return false;
}

void WavePlayer_raiseBPM()
{
    if (BPM != MAX_BPM) {
        BPM += BPM_CHANGE_VALUE;
        if (BPM > MAX_BPM)
            BPM = MAX_BPM;

        printf("BPM: %d\n", BPM);
        sleep_time_ms = BPM_SLEEP_TIME(BPM);
    }
}

void WavePlayer_lowerBPM()
{
    if (BPM != MIN_BPM)
    {
        BPM -= BPM_CHANGE_VALUE;
        if (BPM < MIN_BPM)
            BPM = MIN_BPM;

        printf("BPM: %d\n", BPM);
        sleep_time_ms = BPM_SLEEP_TIME(BPM);
    }
}

int WavePlayer_getCurrentBPM(void) {
    return BPM;
}

int WavePlayer_getCurrentVolumn(void) {
    return AudioMixer_getVolume();
}
