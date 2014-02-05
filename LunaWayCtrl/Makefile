CC	= /home/john/raspistuff/toolchain/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-g++
CFLAGS	= -Wall -std=c++11 -I/home/john/raspistuff/toolchain/rootfs_monstermash/rootfs/usr/local/include
LDFLAGS = -L/home/john/raspistuff/toolchain/rootfs_monstermash/rootfs/usr/local/lib -lwiringPi
ALLOBJS = main.o Encoder.o Motor.o Switch.o
TARGET = LunaWayCtrl

all: $(TARGET)

$(TARGET) : $(ALLOBJS)
	$(CC) $(ALLOBJS) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

main.o : src/main.cpp
	$(CC) -c src/main.cpp $(CFLAGS)

Encoder.o : src/Encoder.cpp
	$(CC) -c src/Encoder.cpp $(CFLAGS)

Motor.o : src/Motor.cpp
	$(CC) -c src/Motor.cpp $(CFLAGS)

Switch.o : src/Switch.cpp
	$(CC) -c src/Switch.cpp $(CFLAGS)
	
clean: 
	rm -f $(ALLOBJS) $(TARGET)
