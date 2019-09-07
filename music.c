//
// Created by fred on 31/08/19.
//

#include "music.h"
#include "audioMixer_template.h"
#include <time.h>
#include <pthread.h>
#include <stdio.h>




static wavedata_t beats[3];
static int BPM;
static int stopping = 0;
static int music_number = 0;

static pthread_t music_threadId;



void* music_thread(void* arg);

int get_music_number() {
    return music_number;
}

void set_music_number(int new_music_num) {
    music_number = new_music_num;
}

void music_init() {
    AudioMixer_readWaveFileIntoMemory("wave-files/100051__menegass__gui-drum-bd-hard.wav", &beats[0]);
    AudioMixer_readWaveFileIntoMemory("wave-files/100053__menegass__gui-drum-cc.wav", &beats[1]);
    AudioMixer_readWaveFileIntoMemory("wave-files/100059__menegass__gui-drum-snare-soft.wav", &beats[2]);

    pthread_create(&music_threadId, NULL, music_thread, NULL);
    set_music_number(MUSIC_NOTHING);
}

void music_cleanup() {
    pthread_join(music_threadId, NULL);
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

int get_BPM() {
    return BPM;
}

void set_BPM(int new_BPM) {
    BPM = new_BPM;
}

void rock1() {
    // Time For Half Beat [sec] = 60 [sec/min] / BPM / 2 [half-beats per beat]
    long sleeptime = (60. / BPM / 2) * 1000000000;


    //Beat 1
    hi_hat();
    base_drum();
    timing(sleeptime);

    //Beat 1.5
    hi_hat();
    timing(sleeptime);

    //Beat 2
    hi_hat();
    hi_snare();
    timing(sleeptime);

    //Beat 2.5
    hi_hat();
    timing(sleeptime);

    //Beat 3
    hi_hat();
    base_drum();
    timing(sleeptime);

    //Beat 3.5
    hi_hat();
    timing(sleeptime);

    //Beat 4
    hi_hat();
    hi_snare();
    timing(sleeptime);

    //Beat 4.5
    hi_hat();
    timing(sleeptime);
}


void rock2() {
    // Time For Half Beat [sec] = 60 [sec/min] / BPM / 2 [half-beats per beat]
    long sleeptime = (60. / BPM / 2) * 1000000000;

    //Beat 1
    hi_hat();
    hi_snare();
    timing(sleeptime);

    //Beat 1.5
    base_drum();
    timing(sleeptime);

    //Beat 2
    hi_hat();
    hi_snare();
    timing(sleeptime);

    //Beat 2.5
    base_drum();
    timing(sleeptime);

    //Beat 3
    hi_hat();
    base_drum();
    timing(sleeptime);

    //Beat 3.5
    hi_snare();
    timing(sleeptime);

    //Beat 4
    hi_hat();
    base_drum();
    timing(sleeptime);

    //Beat 4.5
    hi_snare();
    timing(sleeptime);
}

void timing(long nanoseconds) {
    long seconds = 0;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}


void* music_thread(void* arg)
{
    while (!stopping) {
        if (music_number == MUSIC_NOTHING) {
        } else if(music_number == ROCK1NUM) {
            rock1();
        } else if(music_number == ROCK2NUM) {
            rock2();
        }
    }

    return NULL;
}

