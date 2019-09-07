#include <stdio.h>
#include "audioMixer_template.h"
#include "music.h"
#include "udp_listen.h"
#include "accelerometer.h"
#include "joystick.h"


int main() {
    AudioMixer_init();
    music_init();
    accelerometer_init();
    UdpListener_startListening();
    joystick_init();


    AudioMixer_cleanup();
    Udp_cleanup();
    music_cleanup();
    accelerometer_cleanup();
    joystick_cleanup();

    return 0;
}