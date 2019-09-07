#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include "music.h"
#include "string.h"

#define I2C_DEVICE_ADDRESS 0x1C

#define STATUS 0x00
#define REG_XMSB 0x01
#define REG_XLSB 0x02
#define REG_YMSB 0x03
#define REG_YLSB 0x04
#define REG_ZMSB 0x05
#define REG_ZLSB 0x06
#define CTRL_REG1 0x2A
#define ACTIVATION_VALUE 0x01

#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"

#define X_OFFSET -320
#define Y_OFFSET 280
#define Z_OFFSET 16000
#define X_THRESHOLD 800
#define Y_THRESHOLD 800
#define Z_THRESHOLD 800

static pthread_t accelerometer_id;

void* accelerometer_thread(void* arg);

int stopping = 0;

static int i2cFileDesc;


int FileIODrv_echo_to_file(char* file, char* buff)
{
    // Open direction file
    int file_desc = open(file, O_WRONLY);
    if (file_desc < 0) {
        printf("FILEIODRV ERROR: Unable to open file for write: %s\n", file);
        perror("Error is:");
        exit(-1);
    }

    int bytes_written = write(file_desc, buff, strlen(buff));
//    die_on_failed("Unable to write to file for echo.");

    // Close
    close(file_desc);

    return bytes_written;
}


static int initI2cBus(char* bus, int address)
{
	int i2cFileDesc = open(bus, O_RDWR);
	if (i2cFileDesc < 0) {
		printf("I2C: Unable to open bus for read/write (%s)\n", bus);
		perror("Error is:");
		exit(1);
	}
	int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
	if (result < 0) {
		perror("I2C: Unable to set I2C device to slave address.");
		exit(1);
	}
	return i2cFileDesc;
}

static void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value)
{
	unsigned char buff[2];
	buff[0] = regAddr;
	buff[1] = value;
	int res = write(i2cFileDesc, buff, 2);
	if (res != 2) {
		perror("I2C: Unable to write i2c register.");
		exit(1);
	}
}

/*
static unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr)
{
	// To read a register, must first write the address
	int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
	if (res != sizeof(regAddr)) {
		perror("I2C: Unable to write to i2c register.");
		exit(1);
	}
	// Now read the value and return it
	char value = 0;
	res = read(i2cFileDesc, &value, sizeof(value));
	if (res != sizeof(value)) {
		perror("I2C: Unable to read from i2c register");
		exit(1);
	}
	return value;
}
*/

static void readI2cBuffer(int i2cFileDesc, unsigned char buffer[], int size)
{
	unsigned char firstRegAddr = 0x00;
	// To read a register, must first write the address
	int res = write(i2cFileDesc, &firstRegAddr, sizeof(firstRegAddr));
	if (res != sizeof(firstRegAddr)) {
		perror("I2C: Unable to write to i2c register.");
		exit(1);
	}

	res = read(i2cFileDesc, buffer, sizeof(unsigned char)*size);
	if (res != sizeof(unsigned char)*size) {
		perror("I2C: Unable to read entire buffer");
		exit(1);
	}
}


void accelerometer_init()
{
    FileIODrv_echo_to_file("/sys/devices/platform/bone_capemgr/slots", "BB-I2C1");

	i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);

	//static void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value)
	writeI2cReg(i2cFileDesc, CTRL_REG1, ACTIVATION_VALUE);

    pthread_create(&accelerometer_id, NULL, accelerometer_thread, NULL);
}

void accelerometer_cleanup() {
    pthread_join(accelerometer_id, NULL);
}


void* accelerometer_thread(void* arg)
{
	unsigned char buff[7];

	int16_t previous_x = 0;
	int16_t previous_y = 0;
	int16_t previous_z = 0;
	while (!stopping) {
		readI2cBuffer(i2cFileDesc, buff, 7);
		int16_t x = (buff[REG_XMSB] << 8) | (buff[REG_XLSB]);
		int16_t y = (buff[REG_YMSB] << 8) | (buff[REG_YLSB]);
		int16_t z = (buff[REG_ZMSB] << 8) | (buff[REG_ZLSB]);

		x -= X_OFFSET;
		y -= Y_OFFSET;
		z -= Z_OFFSET;

		_Bool happened = false;
		if (abs(x - previous_x) > X_THRESHOLD) {
			printf("diff X = %d\n", x - previous_x);
			hi_hat();
			happened = true;
		}
		if (abs(y - previous_y) > Y_THRESHOLD) {
			printf("diff Y = %d\n", y - previous_y);
            base_drum();
			happened = true;
		}
		if (abs(z - previous_z) > Z_THRESHOLD) {
			printf("diff Z = %d\n", z - previous_z);
            hi_snare();
			happened = true;
		}

		if (happened) {
			long nanoseconds = 100000000;
			timing(nanoseconds);
		}

		previous_x = x;
		previous_y = y;
		previous_z = z;

		// sleep(1);
	}
	// Cleanup I2C access;
	close(i2cFileDesc);
	return NULL;
}

