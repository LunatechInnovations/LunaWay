CC	= /home/john/raspistuff/toolchain/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-g++
pi:	CC = g++-4.7
CFLAGS	= -Wall -std=c++11 -I/home/john/raspistuff/toolchain/rootfs_monstermash/rootfs/usr/local/include $(DEFINES)
LDFLAGS = -L/home/john/raspistuff/toolchain/rootfs_monstermash/rootfs/usr/local/lib -lpthread
DEFINES = -D_GLIBCXX_USE_NANOSLEEP=1
ALLOBJS = main.o Encoder.o Motor.o SegwayPlotterCom.o I2Cdev.o MPU6050.o Angles.o PID.o AbstractCyclicThread.o GPIOPin.o GPIO.o Segway.o XBoxCtrlServer.o
TARGET = LunaWayCtrl

doc:
	doxygen Doxyfile
	
all: $(TARGET)

pi: $(TARGET)
	
$(TARGET) : $(ALLOBJS)
	$(CC) $(ALLOBJS) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

main.o : src/main.cpp
	$(CC) -c src/main.cpp $(CFLAGS)

Encoder.o : src/Encoder.cpp
	$(CC) -c src/Encoder.cpp $(CFLAGS)

Motor.o : src/Motor.cpp
	$(CC) -c src/Motor.cpp $(CFLAGS)

SegwayPlotterCom.o : src/SegwayPlotterCom.cpp
	$(CC) -c src/SegwayPlotterCom.cpp $(CFLAGS)
	
I2Cdev.o : src/I2Cdev.cpp
	$(CC) -c src/I2Cdev.cpp $(CFLAGS)

MPU6050.o : src/MPU6050.cpp
	$(CC) -c src/MPU6050.cpp $(CFLAGS)
	
Angles.o : src/Angles.cpp
	$(CC) -c src/Angles.cpp $(CFLAGS)
	
PID.o : src/PID.cpp
	$(CC) -c src/PID.cpp $(CFLAGS)
	
AbstractCyclicThread.o : src/AbstractCyclicThread.cpp
	$(CC) -c src/AbstractCyclicThread.cpp $(CFLAGS)
	
GPIOPin.o : src/GPIOPin.cpp
	$(CC) -c src/GPIOPin.cpp $(CFLAGS)
	
GPIO.o : src/GPIO.cpp
	$(CC) -c src/GPIO.cpp $(CFLAGS)
	
Segway.o : src/Segway.cpp
	$(CC) -c src/Segway.cpp $(CFLAGS)
	
XBoxCtrlServer.o : src/XBoxCtrlServer.cpp
	$(CC) -c src/XBoxCtrlServer.cpp $(CFLAGS)
	
clean: 
	rm -f $(ALLOBJS) $(TARGET)
	rm -rf doxygen-output
