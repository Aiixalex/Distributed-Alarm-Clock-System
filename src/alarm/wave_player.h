#include <stdbool.h>
// intreact with the player such as play predfined drum beat on the waveplayer or change volumn/BPM
#ifndef _WAVE_PLAYER_H_
#define _WAVE_PLAYER_H_

// initialize the WavePlyer
void WavePlayer_init(void);

// stop the player
void WavePlayer_stop(void);

// change the player mode to the next one
void WavePlayer_goNextMode(void);

// change the player's mode to a mode
void WavePlayer_changeMode(modes modes);

// play a beat right now
void WavePlayer_playBeat(drum_beats drumBeats);

// raise/decrease player Volumn by 5
void WavePlayer_raiseVolumn(void);
void WavePlayer_lowerVolumn(void);

// set Volumn, return true on success and false otherwise (0 - 100)
bool WavePlayer_setVolumn(int volumn);

// raise/decrease player BPM by 5
void WavePlayer_raiseBPM(void);
void WavePlayer_lowerBPM(void);

// set BPM of the player
bool WavePlayer_setBPM(int BPM);

//all getters
int WavePlayer_getCurrentBPM(void);
int WavePlayer_getCurrentVolumn(void);
modes WavePlayer_getCurrentMode(void);

#endif
