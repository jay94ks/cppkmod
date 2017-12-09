
TARGET=../base
EXTENSION=.c
OBJ_EXTENSION=.o
OUTPUT=.object.generated.mk
MODULE_NAME=module

########################################################

.SUFFIXES := $(EXTENSION) $(OBJ_EXTENSION)

FILE_SELF := $(firstword $(MAKEFILE_LIST))
SOURCES := $(shell find $(TARGET) -type f -name "*$(EXTENSION)")
OBJECTS := $(SOURCES:$(EXTENSION)=$(OBJ_EXTENSION))

########################################################
 
MAKE=make
ECHO=echo
DELETE=rm -rf

########################################################

generate:
	@$(ECHO) "BEGIN $(OUTPUT)"
	@([ -f "$(OUTPUT)" ] && $(DELETE) $(OUTPUT)); true
	@$(MAKE) -f $(FILE_SELF) TARGET=$(TARGET) EXTENSION=$(EXTENSION) \
		OBJ_EXTENSION=$(OBJ_EXTENSION) OUTPUT=$(OUTPUT) stage-2

clean:
	@$(ECHO) "CLEAN $(OUTPUT)"
	@$(DELETE) $(OUTPUT)

stage-2: $(OBJECTS)
	@$(ECHO) "END   $(OUTPUT)"
	
%$(OBJ_EXTENSION):
	@$(ECHO) "ADDED $*.c -> $*.o"
	@$(ECHO) "$(MODULE_NAME)-objs += $*.o" >> $(OUTPUT)
