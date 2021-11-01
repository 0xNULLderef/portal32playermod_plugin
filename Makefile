CXX=g++
CXXFLAGS=-m32 -g -I. -I/usr/include/freetype2 -fPIC -Wall -Wno-reorder -Wno-write-strings -Wno-init-self -O0 -std=c++20
LDFLAGS=-m32 -shared -lfreetype -lSDL2
TARGET=ut.so
PLUGINDIR="/home/konrad/.local/share/Steam/steamapps/common/Portal 2/portal2" # edit to suit your installation

SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))

all: clean $(TARGET)
	cp -v $(TARGET) $(PLUGINDIR)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -rf $(TARGET) *.o