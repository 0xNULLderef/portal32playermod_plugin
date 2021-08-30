CXX=g++
CXXFLAGS=-m32 -g -I. -fPIC -Wall -Wno-reorder -Wno-write-strings -Wno-init-self -O0
LDFLAGS=-m32 -shared
TARGET=pl.so

SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))

all: clean $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -rf $(TARGET) *.o
