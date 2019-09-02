//
// Created by fred on 31/08/19.
//

#ifndef BEAT_BOX_MUSIC_H
#define BEAT_BOX_MUSIC_H

void music_init();
void base_drum();
void hi_hat();
void hi_snare();
void rock1();

void set_BPM(int new_BPM);
int get_BPM();

int get_music_number();

void set_music_number(int new_music_num);

#define MUSIC_NOTHING 0
#define ROCK1NUM 1
#define ROCK2NUM 2

#endif //BEAT_BOX_MUSIC_H
