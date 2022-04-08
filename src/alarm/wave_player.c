// wave player implementation
#include "audioMixer.h"
#include "../utility/utility.h"
#include "wave_player.h"
#include <stdio.h>
#include <pthread.h>

// File used for play-back:
// If cross-compiling, must have this file available, via this relative path,
// on the target when the application is run. This example's Makefile copies the wave-files/
// folder along with the executable to ensure both are present.
#define BASE_DRUM "beatbox-wav-files/100051__menegass__gui-drum-bd-hard.wav"
#define HI_HAT "beatbox-wav-files/100053__menegass__gui-drum-cc.wav"
#define SNARE "beatbox-wav-files/100059__menegass__gui-drum-snare-soft.wav"
#define BPM_SLEEP_TIME(x) ((60.0/(x)/2.0)*1000.0)
#define NUM_MODES 3
#define VOLUMN_CHANGE_VALUE 5
#define BPM_CHANGE_VALUE 5
#define MAX_BPM 300
#define MIN_BPM 40

static int myMode = 1;
static bool shouldPlay = false;
static int volumn = 0;
static int BPM = 120;
static int sleep_time_ms = BPM_SLEEP_TIME(120);
static pthread_t threadID;
static wavedata_t base_drum, hi_hat, snare;

static void play_default_beat() {
    AudioMixer_queueSound(&hi_hat);
    AudioMixer_queueSound(&base_drum);
    my_sleep_ms(sleep_time_ms);
    AudioMixer_queueSound(&hi_hat);
    my_sleep_ms(sleep_time_ms);
    AudioMixer_queueSound(&hi_hat);
    AudioMixer_queueSound(&snare);
    my_sleep_ms(sleep_time_ms);
    AudioMixer_queueSound(&hi_hat);
    my_sleep_ms(sleep_time_ms);
}

static void play_beat_2() {
    AudioMixer_queueSound(&base_drum);
    my_sleep_ms(sleep_time_ms);
    AudioMixer_queueSound(&base_drum);
    my_sleep_ms(sleep_time_ms);
    AudioMixer_queueSound(&hi_hat);
    AudioMixer_queueSound(&snare);
    my_sleep_ms(sleep_time_ms);
    my_sleep_ms(sleep_time_ms);
}


static void *player_thread(void *args) {
	while (shouldPlay)
	{
        switch (myMode)
        {
        case 0:
            my_lock_signal_wait();
            break;

        case 1:
            play_default_beat();
            break;

        case 2:
            play_beat_2();
            break;

        default:
            printf("Unknow mode\n");
            break;
        }
    }

    return 0;
}

void WavePlayer_playBeat(drum_beats drumBeats) {
    switch (drumBeats)
    {
    case 0:
        AudioMixer_queueSound(&base_drum);
        break;
    
    case 1:
        AudioMixer_queueSound(&hi_hat);
        break;

    case 2:
        AudioMixer_queueSound(&snare);
        break;

    default:
        printf("Unknow drum beat\n");
        break;
    }
}

void WavePlayer_changeMode(modes modes) {
    if (myMode != modes) {
        if (myMode == 0) {
            myMode = modes;
            my_lock_signal_signal();
        } else {
            myMode = modes;
        }
        printf("Mode: %d\n", myMode);
    }
}

void WavePlayer_goNextMode() {
    if (myMode == 0) {
        myMode = (myMode + 1) % NUM_MODES;
        my_lock_signal_signal();
    } else {
        myMode = (myMode + 1) % NUM_MODES;
    }
    printf("Mode: %d\n", myMode);
}

void WavePlayer_init() {
    AudioMixer_init();
    initialize_my_lock_signal_wait();
    AudioMixer_readWaveFileIntoMemory(BASE_DRUM, &base_drum);
    AudioMixer_readWaveFileIntoMemory(HI_HAT, &hi_hat);
    AudioMixer_readWaveFileIntoMemory(SNARE, &snare);
    shouldPlay = true;
    volumn = AudioMixer_getVolume();
    pthread_create(&threadID, NULL, &player_thread, NULL);
}

void WavePlayer_stop() {
    shouldPlay = false;
    if (myMode == 0)
        my_lock_signal_signal();
    
    pthread_join(threadID, NULL);
    my_lock_signal_destory();
    AudioMixer_freeWaveFileData(&base_drum);
    AudioMixer_freeWaveFileData(&hi_hat);
    AudioMixer_freeWaveFileData(&snare);
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

modes WavePlayer_getCurrentMode(void) {
    return myMode;
}
