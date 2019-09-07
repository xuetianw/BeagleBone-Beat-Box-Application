//
// Created by fred on 07/09/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "joystick.h"
#include "music.h"
#include "audioMixer_template.h"

#define EXPORT_PATH "/sys/class/gpio/export"

#define GPIO_PIN_UP "/sys/class/gpio/gpio26/value"
#define GPIO_PIN_RIGHT "/sys/class/gpio/gpio47/value"
#define GPIO_PIN_DOWN "/sys/class/gpio/gpio46/value"
#define GPIO_PIN_LEFT "/sys/class/gpio/gpio65/value"
#define GPIO_JOY_IN "/sys/class/gpio/gpio27/value"


static pthread_t joystick_threadId;

void *joystick_thread(void* arg);

static int stopping = 0;

enum direction {
    LEFT, RIGHT, UP, DOWN, IN
};

enum direction read_joystick();

int readFromFileToScreen(char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("ERROR: Unable to open file (%s) for read\n", fileName);
        exit(-1);
    }
// Read string (line)
    const int max_length = 1024;
    char buff[max_length];
    char* result = fgets(buff, max_length, file);
//    printf("result%s\n", result);
// Close
    fclose(file);
    return atoi(result);

}


void export_gpio(int pin) {// Use fopen() to open the file for write access.
    FILE* pfile = fopen(EXPORT_PATH, "w");
    if (pfile == NULL) {
        printf("ERROR: Unable to open export file.\n");
        exit(1);
    }
    // Write to data to the file using fprintf():
    fprintf(pfile, "%d", pin);
    fclose(pfile);
}


void joystick_init() {
    export_gpio(26);
    export_gpio(47);
    export_gpio(46);
    export_gpio(65);
    export_gpio(27);

    pthread_create(&joystick_threadId, NULL, joystick_thread, NULL);
}

void joystick_cleanup() {
    pthread_join(joystick_threadId, NULL);;
}

void *joystick_thread(void* arg) {
    while (!stopping) {
        enum direction user_input = read_joystick();
        if (user_input == UP) {
            int current_valume = AudioMixer_getVolume();
            if (current_valume == MAX_VOLUME) {
            } else {
                if (current_valume + 5 > MAX_VOLUME) {
                    AudioMixer_setVolume(current_valume);
                } else {
                    int newVolum = current_valume + 5;
                    AudioMixer_setVolume(newVolum);
                }
            }
        } else if (user_input == DOWN) {
            int current_valume =  AudioMixer_getVolume();
            if (current_valume == MIN_VOLUME) {
            } else {
                if (current_valume - 5 < MIN_VOLUME) {
                    AudioMixer_setVolume(current_valume);
                } else {
                    int newVolum = current_valume - 5;
                    AudioMixer_setVolume(newVolum);
                }
            }
        } else if (user_input == LEFT) {
            int current_bpm = get_BPM();
            if (current_bpm == MIN_BPM) {
            } else {
                if (current_bpm - 5 < MIN_BPM) {
                    set_BPM(MIN_BPM);
                } else {
                    int newBPM = current_bpm - 5;
                    set_BPM(newBPM);
                }
            }
        } else if (user_input == RIGHT){
            int current_BPM = get_BPM();
            if (current_BPM == MAX_BPM) {
            } else {
                if (current_BPM + 5 > MAX_BPM) {
                    set_BPM(MAX_BPM);
                } else {
                    int newBPM = current_BPM + 5;
                    set_BPM(newBPM);
                }
            }
        } else {
            int music_num = get_music_number();
            music_num++;
            if (music_num > 2) {
                music_num = 0;
            }
            set_music_number(music_num);
        }
    }
}

enum direction read_joystick() {
    while (1) {
        if (readFromFileToScreen(GPIO_PIN_UP) == 0) {
            while (readFromFileToScreen(GPIO_PIN_UP) == 0) {
            }
            return UP;
        }
        if (readFromFileToScreen(GPIO_PIN_DOWN) == 0) {
            while (readFromFileToScreen(GPIO_PIN_DOWN) == 0) {
            }
            return DOWN;
        }
        if (readFromFileToScreen(GPIO_PIN_LEFT) == 0) {
            while (readFromFileToScreen(GPIO_PIN_LEFT) == 0) {
            }
            return LEFT;
        }
        if (readFromFileToScreen(GPIO_PIN_RIGHT) == 0) {
            while (readFromFileToScreen(GPIO_PIN_RIGHT) == 0) {
            }
            return RIGHT;
        }
        if (readFromFileToScreen(GPIO_JOY_IN) == 0) {
            while (readFromFileToScreen(GPIO_JOY_IN) == 0) {
            }
            return IN;
        }
    }
}

