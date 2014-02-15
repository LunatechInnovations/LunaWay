CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 -std=c++11

OBJS =		main.o XBoxCtrl.o

LIBS =		-lpthread

TARGET =	RemoteGamepad

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
