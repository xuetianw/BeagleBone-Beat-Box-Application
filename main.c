#include <stdio.h>
#include "audioMixer_template.h"


int main() {
    AudioMixer_init();

    wavedata_t beats[3];
    AudioMixer_readWaveFileIntoMemory("wave-files/100051__menegass__gui-drum-bd-hard.wav", &beats[0]);
    AudioMixer_readWaveFileIntoMemory("wave-files/100053__menegass__gui-drum-cc.wav", &beats[1]);
    AudioMixer_readWaveFileIntoMemory("wave-files/100059__menegass__gui-drum-snare-soft.wav", &beats[2]);

    AudioMixer_queueSound(&beats[0]);

    AudioMixer_cleanup();
    return 0;
}