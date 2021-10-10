CXX=g++
CXXFLAGS=-m32 -g -I. -fPIC -Wall -Wno-reorder -Wno-write-strings -Wno-init-self -O0
LDFLAGS=-m32 -shared
TARGET=ut.so
PLUGINDIR="/home/konrad/.local/share/Steam/steamapps/common/Portal 2/portal2" # edit to suit your installation

SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))

all: clean $(TARGET) copy

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -rf $(TARGET) *.o

copy:
	cp -v $(TARGET) $(PLUGINDIR)
