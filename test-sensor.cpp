// Ken Hughes
// July 2016


#include <unistd.h>//Needed for I2C port
#include <fcntl.h>//Needed for I2C port
#include <sys/ioctl.h>//Needed for I2C port
#include <linux/i2c-dev.h>//Needed for I2C port
#include <stdio.h>
#include <string.h>

const int i2cAddress = 0x68;

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
                printf("Read %i bytes : ", bytesRead);
                for(int i=0; i< bytesRead; i++) {
                    printf(" %x", buffer[i]);
                }
                printf("\n");
            }
        }
            
        // Close the i2c device bus
        close(*deviceName);
    }
}

int main() {

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

    /*
    unsigned char buffer[3] = {0};
    buffer[0] = 10;
    writeI2C(buffer, 1);
    readI2C(buffer, 3);
    */
}