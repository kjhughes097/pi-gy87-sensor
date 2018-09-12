all:
	gcc test-sensors.cpp -o test-sensors -std=c++11 -lm

clean:
	rm -f test-sensors

mpu6050:
	gcc mpu6050.cpp mpu6050-test.cpp -o mpu6050-test -std=c++11 -lm -lstdc++
