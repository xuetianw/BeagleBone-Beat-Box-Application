#include <stdio.h>
#include "audioMixer_template.h"
#include "music.h"
#include "udp_listen.h"


int main() {
    AudioMixer_init();

    music_init();

    UdpListener_startListening();

//    base_drum();
//    hi_hat();
//    snare();

    AudioMixer_cleanup();
    Udp_cleanup();

    return 0;
}