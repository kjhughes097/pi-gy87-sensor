/**
 * Ken Hughes
 * 
 * Test app for the MPU6050 class
 *
 */

#include <stdio.h>
#include <string.h>


#include "mpu6050.h"

int main()
{
    MPU6050 *mpu = new MPU6050();
    mpu->reset();
    if (mpu->whoAmI())
    {
        printf("WhoAmI was okay\n");
        bool enabled = mpu->getBypassEnable();
        printf("Bypass is %s\n", enabled ? "ENABLED" : "DISABLED");
        printf("Toggling...\n");
        mpu->setBypassEnable(!enabled);
        return 1;
    }
    return 0;
}