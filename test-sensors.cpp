// Ken Hughes
// July 2016


#include <unistd.h>//Needed for I2C port
#include <fcntl.h>//Needed for I2C port
#include <sys/ioctl.h>//Needed for I2C port
#include <linux/i2c-dev.h>//Needed for I2C port
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

const int HMC5883_ADDR = 0x00;

const int MCU6050_ADDR = 0x68;
const int ACCEL_XOUT_HI = 0x3B;
const int GYRO_XOUT_HI = 0x43;
const int TEMP_HI = 0x41;

const int BMP180_ADDR = 0x77;

int i2cAddress = MCU6050_ADDR;

void writeI2C(unsigned char* data, int bytes) {
    int i2cHandle;

    char *deviceName = (char*)"/dev/i2c-1";
    if ((i2cHandle = open(deviceName, O_RDWR)) < 0) {
        printf("error opening I2C\n");
    }
    else {
        if (ioctl(i2cHandle, I2C_SLAVE, i2cAddress) < 0) {
            printf("Error at ioctl\n");
        }
        else {
            /*
            printf("Writing %i bytes : ", bytes);
            for(int i=0; i< bytes; i++) {
                printf(" %x", data[i]);
            }
            printf("\n");
            */
            write(i2cHandle, data, bytes);
        }
            
        // Close the i2c device bus
        close(*deviceName);
    }
}

void readI2C(unsigned char* buffer, int bytes) {
    int i2cHandle;

    char *deviceName = (char*)"/dev/i2c-1";
    if ((i2cHandle = open(deviceName, O_RDWR)) < 0) {
        printf("error opening I2C\n");
    }
    else {
        if (ioctl(i2cHandle, I2C_SLAVE, i2cAddress) < 0) {
            printf("Error at ioctl\n");
        }
        else {
//            unsigned char cmdBuffer[1] = {0x06};
//            write(i2cHandle, cmdBuffer, 1);
            int bytesRead = read(i2cHandle, buffer, bytes);
            if (bytes != bytesRead) {
                printf("Error reading from I2C, expected %i bytes, but got %i\n", bytes, bytesRead);
            }
            else {
                /*
                printf("Read %i bytes : ", bytesRead);
                for(int i=0; i< bytesRead; i++) {
                    printf(" %x", buffer[i]);
                }
                printf("\n");
                */
            }
        }
            
        // Close the i2c device bus
        close(*deviceName);
    }
}

int main() {

    unsigned char whoAmIAddr[1] = {0x75};
    unsigned char setup1[2] = {0x19,0x07}; // sample rate divider (0x07 means 8kHz/(1 + 0x07) = 1kHz)
    unsigned char setup2[2] = {0x1B,0x08}; // gyro range (0x08 means +/-500 degs/sec)
    const float GYRO_DIV = 65.5;
    unsigned char setup3[2] = {0x6B,0x02};
    unsigned char setup4[2] = {0x1C,0x00}; // accel range (0x00 means +/- 2g)
    const float ACCEL_DIV = 16384;
    unsigned char accelBaseReg[1] = {ACCEL_XOUT_HI};
    unsigned char gyroBaseReg[1] = {GYRO_XOUT_HI};
    unsigned char tempBaseReg[1] = {TEMP_HI};

    // read WhoAmI register (should tell us '68')
    writeI2C(whoAmIAddr,1);
    unsigned char buff[256] = {0};
    readI2C(buff, 1);
    
    // write the config data
    writeI2C(setup1, 2);
    writeI2C(setup2, 2);
    writeI2C(setup3, 2);
    writeI2C(setup4, 2);

    float pitch = 0;
    float roll = 0;
    float pitchAccel;
    float rollAccel;
    struct timeval lastSample;
    struct timeval thisSample;
    struct timeval dtSample;
    gettimeofday(&lastSample, NULL);

    // main loop, reading sensors and calculating angle
    while(1) {

        writeI2C(tempBaseReg,1);
        readI2C(buff, 2);
        short int temp = (buff[0]<<8 | buff[1]);
        float tmp = ((float)temp/340) + 36.53;
        //printf("Temp Val = %f C\n",tmp); 
        
        writeI2C(accelBaseReg,1);
        readI2C(buff, 6);
        float x_accel = ((short int)(buff[0]<<8 | buff[1]))/ACCEL_DIV;
        float y_accel = ((short int)(buff[2]<<8 | buff[3]))/ACCEL_DIV;
        float z_accel = ((short int)(buff[4]<<8 | buff[5]))/ACCEL_DIV;
        //printf("Accel Vals = %d : %d : %d\n", x_val, y_val, z_val);

        writeI2C(gyroBaseReg,1);
        readI2C(buff, 6);
        float x_gyro = ((short int)(buff[0]<<8 | buff[1]))/65.5;
        float y_gyro = ((short int)(buff[2]<<8 | buff[3]))/65.5;
        float z_gyro = ((short int)(buff[4]<<8 | buff[5]))/65.5;

        gettimeofday(&thisSample, NULL);
        timersub(&thisSample, &lastSample, &dtSample);
        gettimeofday(&lastSample, NULL);
        //printf("dtSample = %d uS : (%08.6f S)\n", dtSample.tv_usec, ((float)dtSample.tv_usec / 1000000));
        //printf("GX=% 07.2f GY=% 07.2f GZ=% 07.2f AX=% 05.3f AY=% 05.3f AZ=% 05.3f TMP=% 06.2f\n", x_gyro, y_gyro, z_gyro, x_accel, y_accel, z_accel, tmp);

        pitch += ((float)x_gyro * ((float)dtSample.tv_usec / 1000000));
        roll -= ((float)y_gyro * ((float)dtSample.tv_usec / 1000000));
        pitchAccel = (atan2f(x_accel, z_accel) + 3.14159265359) * (180 / 3.14159265359);
        rollAccel = (atan2f(y_accel, z_accel) + 3.14159265359) * (180 / 3.14159265359);
        pitch = (pitch * 0.98) + (pitchAccel * 0.02);
        roll = (roll * 0.98) + (rollAccel * 0.02);
        //float pitch = atan2f(y_accel, sqrt(pow(y_accel,2) - (pow(z_accel,2)))) * 57.2958;
        printf("Pitch = %05.1f : Roll = %05.1f\n ", pitch, roll);

    }


    /*
    // configure continuous mode
    unsigned char setConfig1[2] = {0x3C,0x70};
    unsigned char setConfig2[2] = {0x3C,0xA0};
    unsigned char setConfig3[2] = {0x3C,0x00};
    unsigned char setAddress[1] = {0x03};
    writeI2C(setConfig1,2);
    writeI2C(setConfig2,2);
    writeI2C(setConfig3,2);
    usleep(47000);
    writeI2C(setAddress,1);

    unsigned char buff[6] = {0};
    while(1) {
        readI2C(buff, 6);
        usleep(500000);
    }
    */

    /*
    unsigned char buffer[3] = {0};
    buffer[0] = 10;
    writeI2C(buffer, 1);
    readI2C(buffer, 3);
    */
}