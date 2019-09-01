//
// Created by fred on 31/08/19.
//

#include "music.h"
#include "audioMixer_template.h"

static wavedata_t beats[3];

void music_init() {
    AudioMixer_readWaveFileIntoMemory("wave-files/100051__menegass__gui-drum-bd-hard.wav", &beats[0]);
    AudioMixer_readWaveFileIntoMemory("wave-files/100053__menegass__gui-drum-cc.wav", &beats[1]);
    AudioMixer_readWaveFileIntoMemory("wave-files/100059__menegass__gui-drum-snare-soft.wav", &beats[2]);
}

void base_drum() {
    AudioMixer_queueSound(&beats[0]);
}


void hi_hat() {
    AudioMixer_queueSound(&beats[1]);
}

void hi_snare() {
    AudioMixer_queueSound(&beats[2]);
}