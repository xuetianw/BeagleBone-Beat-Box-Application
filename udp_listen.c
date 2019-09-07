#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>			// for strncmp()
#include <unistd.h>			// for close()
#include <pthread.h>
#include "udp_listen.h"
#include "music.h"
#include "audioMixer_template.h"


#define MSG_MAX_LEN 10240
#define PORT		8088




pthread_t udp_id;
void *udp_thread();
void process_message(char *message, struct sockaddr_in* sin);
static int stopping = 0;

void UdpListener_startListening()
{
    pthread_create(&udp_id, NULL, udp_thread, NULL);
}

void Udp_cleanup()
{
    pthread_join(udp_id, NULL);
}

int socketDescriptor;
unsigned int sin_len;

void send_message(const char* message, struct sockaddr_in* sin);

void *udp_thread()
{
	printf("Fred's Net Listen Test on UDP port %d:\n", PORT);
	printf("Connect using: \n");
//	printf("    netcat -u 127.0.0.1 %d\n", PORT);
    printf("    netcat -u 192.168.7.2 %d\n", PORT);

    // Buffer to hold packet data:
	char message[MSG_MAX_LEN];

	// Address
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;                   // Connection may be from network
	sin.sin_addr.s_addr = htonl(INADDR_ANY);    // Host to Network long
	sin.sin_port = htons(PORT);                 // Host to Network short
	
	// Create the socket for UDP
	socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

	// Bind the socket to the port (PORT) that we specify
	bind (socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));
	
	while (!stopping) {
		// Get the data (blocking)
		// Will change sin (the address) to be the address of the client.
		// Note: sin passes information in and out of call!
		sin_len = sizeof(sin);
		int bytesRx = recvfrom(socketDescriptor, message, MSG_MAX_LEN, 0,
				(struct sockaddr *) &sin, &sin_len);

		// Make it null terminated (so string functions work):
		message[bytesRx] = 0;

		process_message(message, &sin);

		// Transmit a reply:
        send_message(message, &sin);
    }
    printf("udp_thread terminated\n");

	// Close
	close(socketDescriptor);
	
	return 0;
}

void send_message(const char* message, struct sockaddr_in* sin) {
    sendto(socketDescriptor,
           message, strlen(message),
           0,
           (struct sockaddr*) sin, sin_len);
}

void process_message(char *message, struct sockaddr_in* sin) {
    printf("%s\n", message);
    if (strcmp(message, "none\n") == 0) {

        set_music_number(MUSIC_NOTHING);

    } else if (strcmp(message, "rock 1\n") == 0) {

        set_music_number(ROCK1NUM);

    } else if (strcmp(message, "rock 2\n") == 0) {
        set_music_number(ROCK2NUM);

    } else if (strcmp(message, "current mode\n") == 0) {
        if (get_music_number() == MUSIC_NOTHING) {
            sprintf(message, "current mode is no muisc\n");
        } else if (get_music_number() == ROCK1NUM) {
            sprintf(message, "current mode is rock 1\n");
        } else if (get_music_number() == ROCK2NUM) {
            sprintf(message, "current mode is rock 2\n");
        }

    } else if (strcmp(message, "get volume\n") == MIN_VOLUME) {

        sprintf(message, "current volume is %d\n", AudioMixer_getVolume());

    } else if(strcmp(message, "increase volume\n") == 0) {

        int current_valume = AudioMixer_getVolume();
        if (current_valume == MAX_VOLUME) {
            sprintf(message, "volume cannot go above %d\n", MAX_VOLUME);
        } else {
            if (current_valume + 5 > MAX_VOLUME) {
                AudioMixer_setVolume(current_valume);
                sprintf(message, "volume has been increased to %d\n", MAX_VOLUME);
            } else {
                int newVolum = current_valume + 5;
                AudioMixer_setVolume(newVolum);
                sprintf(message, "volume has been increased to %d\n", newVolum);
            }
        }

    } else if (strcmp(message, "decrease volume\n") == MIN_VOLUME) {

        int current_valume =  AudioMixer_getVolume();
        if (current_valume == MIN_VOLUME) {
            sprintf(message, "volume cannot go below %d\n", MIN_VOLUME);
        } else {
            if (current_valume - 5 < MIN_VOLUME) {
                AudioMixer_setVolume(current_valume);
                sprintf(message, "volume has been increased to %d\n", MIN_VOLUME);
            } else {
                int newVolum = current_valume - 5;
                AudioMixer_setVolume(newVolum);
                sprintf(message, "volume has been increased to %d\n", newVolum);
            }
        }

    } else if (strcmp(message, "get bpm\n") == 0) {

        sprintf(message, "current BPM is %d\n", get_BPM());

    } else if (strcmp(message, "increase BPM\n") == 0) {
        int current_BPM = get_BPM();
        if (current_BPM == MAX_BPM) {
            sprintf(message, "BPM cannot go above %d\n", MAX_BPM);
        } else {
            if (current_BPM + 5 > MAX_BPM) {
                set_BPM(MAX_BPM);
                sprintf(message, "BPM has been increased to %d\n", MAX_BPM);
            } else {
                int newBPM = current_BPM + 5;
                set_BPM(newBPM);
                sprintf(message, "BPM has been increased to %d\n", newBPM);
            }
        }

    } else if (strcmp(message, "decrease BPM\n") == 0) {
        int current_bpm = get_BPM();
        if (current_bpm == MIN_BPM) {
            sprintf(message, "BPM cannot go below %d\n", MIN_VOLUME);
        } else {
            if (current_bpm - 5 < MIN_BPM) {
                set_BPM(MIN_BPM);
                sprintf(message, "BPM has been increased to %d\n", MIN_VOLUME);
            } else {
                int newBPM = current_bpm - 5;
                set_BPM(newBPM);
                sprintf(message, "BPM has been increased to %d\n", newBPM);
            }
        }

    } else if (strcmp(message, "hi-hat\n") == 0) {

        hi_hat();

    } else if (strcmp(message, "base\n") == 0) {

        base_drum();

    } else if (strcmp(message, "snare\n") == 0) {

        hi_snare();

    }
}
