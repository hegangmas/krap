TARGET=sender
CXXFLAGS=-std=gnu++0x -Wall -O2  -D_GLIBCXX_USE_NANOSLEEP
LDLIBS=-lpthread

all: $(TARGET)
run: 
	@sh sender 1.2.3.4 2048 1024 1000000 40
clean:
	@rm -f *.o $(TARGET) *~
