#include <stdio.h>
#include "audioMixer_template.h"
#include "music.h"


int main() {
    AudioMixer_init();

    music_init();

    base_drum();
    hi_hat();
    hi_snare();

    AudioMixer_cleanup();

    return 0;
}