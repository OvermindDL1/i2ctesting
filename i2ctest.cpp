#include<ncurses.h>
#include<iostream>
#include"i2c.h"
#include<unistd.h>

//Device and status register addresses
#define DEVADDR 0x68
#define STATUS 0x6b

using namespace std;

//function to combine upper and lower bytes of register values from MPU-6050
signed short int com_val(signed short int devaddr, signed short int hiaddr, signed short int loaddr);

	myI2C hiptr;
	myI2C loptr;
	myI2C status;

int main(int argc, char *argv[]){
	int sleep = 0;
	if (argc >1) {
		sleep = (atoi(argv[1]) * 1000);
	}
	else {
		sleep = 250000;
	}

//	int sleep=250000;
	float tempconv = 0.0;
	float gyroX, gyroY, gyroZ = 0.0;
	float accelX, accelY, accelZ = 0.0;
	if (status.Read_I2C_Byte(DEVADDR, STATUS) != 0){
		status.Send_I2C_Byte(DEVADDR, STATUS, 0x00);
	}
	initscr();
	curs_set(0);
	while(1) {

	gyroX = com_val(0x68, 0x43, 0x44)/131.0;
	gyroY = com_val(0x68, 0x45, 0x46)/131.0;
	gyroZ = com_val(0x68, 0x47, 0x48)/131.0;

	accelX = com_val(0x68, 0x3b, 0x3c)/16384.0;
	accelY = com_val(0x68, 0x3d, 0x3e)/16384.0;
	accelZ = com_val(0x68, 0x3f, 0x40)/16384.0;

	tempconv = ((com_val(0x68, 0x41, 0x42)/340.0 + 36.53) * 1.8 +32);

	printw("Temperature (F): %f", tempconv);

	move(1,0);
	printw("Gyro X: %f", gyroX);

	move(2,0);
	printw("Gyro Y: %f", gyroY);

	move(3,0);
	printw("Gyro Z: %f", gyroZ);

	move(4,0);
	printw("Accel X: %f", accelX);

	move(5,0);
	printw("Accel Y: %f", accelY);

	move(6,0);
	printw("Accel Z: %f", accelZ);

	move(0, 0);

	refresh();

	usleep(sleep);
	}

	endwin();
	return(0);
}

signed short int com_val(signed short int devaddr, signed short int hiaddr, signed short int loaddr) {
	signed short int hibyte = hiptr.Read_I2C_Byte(devaddr, hiaddr);
	signed short int lobyte = loptr.Read_I2C_Byte(devaddr, loaddr);
	signed short int n = (hibyte<<8) | lobyte;

	return n;
}
