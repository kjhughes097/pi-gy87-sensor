// Ken Hughes
// July 2016


#include <unistd.h>//Needed for I2C port
#include <fcntl.h>//Needed for I2C port
#include <sys/ioctl.h>//Needed for I2C port
#include <linux/i2c-dev.h>//Needed for I2C port
#include <stdio.h>
#include <string.h>
#include <math.h>

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
    unsigned char setup1[2] = {0x19,0x07};
    unsigned char setup2[2] = {0x1B,0x08};
    unsigned char setup3[2] = {0x6B,0x02};
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

    // read the accel X registers (hi and lo bytes)
    while(1) {
        writeI2C(tempBaseReg,1);
        readI2C(buff, 2);
        short int temp = (buff[0]<<8 | buff[1]);
        float tmp = ((float)temp/340) + 36.53;
        //printf("Temp Val = %f C\n",tmp); 
        
        writeI2C(accelBaseReg,1);
        readI2C(buff, 6);
        short int x_val = (buff[0]<<8 | buff[1]);
        short int y_val = (buff[2]<<8 | buff[3]);
        short int z_val = (buff[4]<<8 | buff[5]);
        //printf("Accel Vals = %d : %d : %d\n", x_val, y_val, z_val);

        writeI2C(gyroBaseReg,1);
        readI2C(buff, 6);
        float x = ((short int)(buff[0]<<8 | buff[1]))/65.5;
        float y = ((short int)(buff[2]<<8 | buff[3]))/65.5;
        float z = ((short int)(buff[4]<<8 | buff[5]))/65.5;
        printf("GX=% 07.2f GY=% 07.2f GZ=% 07.2f AX=% 06d AY=% 06d AZ=% 06d TMP=% 06.2f\n", x, y, z, x_val, y_val, z_val, tmp);

        float pitch = (atan2f(y_val, z_val) + 3.14);
        printf("Pitch = %06f\n", pitch);

        usleep(250000);
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