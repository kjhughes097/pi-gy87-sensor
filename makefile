all:
	gcc test-sensor.cpp -o test-sensor -std=c++11

clean:
	rm -f test-sensor

