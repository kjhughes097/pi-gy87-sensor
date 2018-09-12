/**
 * Ken Hughes
 * 
 * Test app for the MPU6050 class
 *
 */


 #include "mpu6050.h"

 int main()
 {
     MPU6050 *mpu = new MPU6050();
     if (mpu->whoAmI())
     {
         mpu->setBypassEnable(false);
         return 1;
     }
     return 0;
 }