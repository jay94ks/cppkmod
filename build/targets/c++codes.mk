.SUFFIXES := .cpp .cpp.o

ECHO=echo
DELETE=rm -rf

DIRECTORY=../sources
TARGET=../build/usercode.po

CXX_FLAGS = -mcmodel=kernel -ffreestanding -fno-exceptions -fno-rtti -nostdinc
CXX_SOURCES = $(shell find $(DIRECTORY) -type f -name "*.cpp")
CXX_INCLUDES = -I../headers -I/usr/src/linux-headers-$(shell uname -r)/include
CXX_OBJS := $(CXX_SOURCES:.cpp=.cpp.o)

all: $(TARGET)

clean:
	@$(ECHO) "CLEAN $(TARGET)"
	@$(DELETE) $(TARGET)
	@$(DELETE) $(CXX_OBJS)

$(TARGET): $(CXX_OBJS)
	@$(ECHO) "LINK  $(TARGET)"
	@$(LD) -r -o $(TARGET) $(CXX_OBJS)

%.cpp.o:
	@$(ECHO) "CXX   $@"
	@$(CXX) $(CXX_FLAGS) -c -o $*.cpp.o $*.cpp $(CXX_INCLUDES)