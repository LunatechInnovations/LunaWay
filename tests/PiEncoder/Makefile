CC		= g++
CFLAGS	= -std=c++11 -Wall -g
LDFLAGS	= 
TARGET	= PiEncoder
ALLOBJS	= main.o Encoder.o

all: $(TARGET)

$(TARGET): $(ALLOBJS)
	$(CC) $(LDFLAGS) $(ALLOBJS) -o $(TARGET)
	
main.o: main.cpp
	$(CC) -c $(CFLAGS) main.cpp
	
Encoder.o: Encoder.cpp
	$(CC) -c $(CFLAGS) Encoder.cpp
	
clean:
	rm -f $(ALLOBJS) $(TARGET)