/**
 * Ken Hughes
 * 
 * C library for the MPU6050 gryo
 * Datasheet : https://www.invensense.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
 *
 */

#include <unistd.h>//Needed for I2C port
#include <fcntl.h>//Needed for I2C port
#include <sys/ioctl.h>//Needed for I2C port
#include <linux/i2c-dev.h>//Needed for I2C port
#include <cstdint>
#include <stdio.h>
#include <string.h>

#include "mpu6050.h"


void MPU6050::setDebug(bool dbg)
{
    debug = dbg;
}

void MPU6050::reset()
{
    debugLog("Resetting device\n");
    writeRegister8(MPU6050_REG_PWR_MGMT_1, 0x80);
}

bool MPU6050::getBypassEnable()
{
    uint8_t currentVal = (uint8_t)readRegister8(MPU6050_REG_INT_PIN_CONF);
    if (currentVal & 0x02)
    {
        return true;
    }
    return false;
}

void MPU6050::setBypassEnable(bool enabled)
{
    // make sure I2C_MST_EN (reg 0x6A) bit 5 is set to 0...
    writeRegister8(MPU6050_REG_USER_CNTL, 0x00);
    uint8_t currentVal = (uint8_t)readRegister8(MPU6050_REG_INT_PIN_CONF);
    if (enabled)
    {
        currentVal |= 0x02;
    }
    else
    {
        currentVal &= 0xFD;
    }
    writeRegister8(MPU6050_REG_INT_PIN_CONF, currentVal);
}

bool MPU6050::whoAmI()
{
    if (readRegister8(MPU6050_REG_WHO_AM_I) == 0x68)
    {
        return true;
    }
    return false;
}

clockSource MPU6050::getClockSource()
{
    uint8_t currentVal = (uint8_t)readRegister8(MPU6050_REG_PWR_MGMT_1);
    currentVal &= 0x07;
    return (clockSource)currentVal;
}

void MPU6050::setClockSource(clockSource clock)
{
    uint8_t currentVal = (uint8_t)readRegister8(MPU6050_REG_PWR_MGMT_1);
    currentVal &= 0xF8; // mask out all other bits
    currentVal += clock; // add the clock value
    writeRegister8(MPU6050_REG_PWR_MGMT_1, currentVal);
}

fullScaleAccRange MPU6050::getFullScaleAccRange()
{
    uint8_t currentVal = (uint8_t)readRegister8(MPU6050_REG_ACC_CONF);
    currentVal &= 0x18;
    currentVal >>= 3;
    return (fullScaleAccRange)currentVal;
}

void MPU6050::setFullScaleAccRange(fullScaleAccRange range)
{
    uint8_t currentVal = (uint8_t)readRegister8(MPU6050_REG_ACC_CONF);
    currentVal &= 0xF8; // mask out all other bits
    uint8_t rangeVal = (int)range;
    rangeVal <<= 3;
    currentVal += rangeVal; // add the range value
    writeRegister8(MPU6050_REG_ACC_CONF, currentVal);
}

fullScaleGyroRange MPU6050::getFullScaleGyroRange()
{
    uint8_t currentVal = (uint8_t)readRegister8(MPU6050_REG_GYRO_CONF);
    currentVal &= 0x18;
    currentVal >>= 3;
    return (fullScaleGyroRange)currentVal;
}

void MPU6050::setFullScaleGyroRange(fullScaleGyroRange range)
{
    uint8_t currentVal = (uint8_t)readRegister8(MPU6050_REG_GYRO_CONF);
    currentVal &= 0xF8; // mask out all other bits
    uint8_t rangeVal = (int)range;
    rangeVal <<= 3;
    currentVal += rangeVal; // add the range value
    writeRegister8(MPU6050_REG_GYRO_CONF, currentVal);
}


// private functions
void MPU6050::writeRegister(uint8_t* data, uint8_t bytes)
{
    int i2cHandle;

    char *deviceName = (char*)"/dev/i2c-1";
    if ((i2cHandle = open(deviceName, O_RDWR)) < 0) 
    {
        printf("Error opening I2C channel\n");
    }
    else 
    {
        debugLog("writeRegister opened i2c device for RDWR\n");
        if (ioctl(i2cHandle, I2C_SLAVE, MPU6050_ADDR) < 0) 
        {
            printf("Error at ioctl\n");
        }
        else 
        {
            debugLog("Writing to bytes to i2c :\n");
            for (int i=0; i< bytes; i++)
            {
                char byteStr[10];
                sprintf(byteStr, " - 0x%x2\n", data[i]);
                debugLog(byteStr);
            }
            write(i2cHandle, data, bytes);
        }
        debugLog("writeRegister closing i2c device\n");
        close(*deviceName);
    }
}

void MPU6050::writeRegister8(uint8_t registerAddr, uint8_t value)
{
    uint8_t buffer[2] = {registerAddr, value};
    writeRegister(buffer, 2);
}

void MPU6050::readRegister(uint8_t* buffer, uint8_t bytes)
{
    int i2cHandle;

    char *deviceName = (char*)"/dev/i2c-1";
    if ((i2cHandle = open(deviceName, O_RDWR)) < 0) 
    {
        printf("Error opening I2C channel\n");
    }
    else 
    {
        debugLog("readRegister opened i2c device for RDWR\n");
        if (ioctl(i2cHandle, I2C_SLAVE, MPU6050_ADDR) < 0) 
        {
            printf("Error at ioctl\n");
        }
        else 
        {
            debugLog("Reading bytes from i2c :\n");
            int bytesRead = read(i2cHandle, buffer, bytes);
            if (bytes != bytesRead) 
            {
                printf("Error reading from I2C, expected %i bytes, but got %i\n", bytes, bytesRead);
            }
            else
            {
                for (int i=0; i< bytesRead; i++)
                {
                    char byteStr[10];
                    sprintf(byteStr, " - 0x%x2\n", buffer[i]);
                    debugLog(byteStr);
                }
            }
        }
        debugLog("readRegister closing i2c device\n");
        close(*deviceName);
    }
}

int MPU6050::readRegister8(uint8_t registerAddr)
{
    uint8_t buffer[1] = {registerAddr};
    writeRegister(buffer, 1); // send the register we are interested in
    readRegister(buffer, 1); // get the value from it
    return (int)buffer[0];
}

int MPU6050::readRegister16(uint8_t registerAddr)
{
    uint8_t buffer[2] = {registerAddr, 0};
    writeRegister(buffer, 1); // send the register we are interested in
    readRegister(buffer, 2); // get the value from it (HI byte) and the next (LO byte)
    int retVal = buffer[0] * 0xFF;
    retVal += buffer[1];
    return retVal;
}

void MPU6050::debugLog(const char *message)
{
    if (this->debug)
    {
        printf("%s", message);
    }
}