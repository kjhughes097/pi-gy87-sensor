all:
	gcc test-sensors.cpp -o test-sensors -std=c++11 -lm

clean:
	rm -f test-sensors

