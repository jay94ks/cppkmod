
MAKE=make
ECHO=echo
DELETE=rm -rf

BASE_LIST=base-list.mk
BUILD_PATH=/lib/modules/$(shell uname -r)/build
KBUILD_MAKEFILE=Makefile
MODULE_CFG_FILE=../base/.mod-cfg.c

include ../ModuleInfo.mk

all: $(MODULE_NAME).ko

clean: clean-usercode.po clean-combined.po clean-$(MODULE_NAME).ko 

clean-all: clean clean-libjay.po

libjay.po: targets/c++codes.mk
	@$(MAKE) -f targets/c++codes.mk DIRECTORY=../libjay TARGET=libjay.po all
	
usercode.po: targets/c++codes.mk
	@$(MAKE) -f targets/c++codes.mk DIRECTORY=../sources TARGET=usercode.po all

combined.po: libjay.po usercode.po
	@$(ECHO) "LINK  libjay.po + usercode.po -> combined.po"
	@$(LD) -T linker/jayabi.ld -r libjay.po usercode.po -o combined.po

clean-libjay.po: targets/c++codes.mk
	@$(MAKE) -f targets/c++codes.mk DIRECTORY=../libjay TARGET=libjay.po clean
		
clean-usercode.po: targets/c++codes.mk
	@$(MAKE) -f targets/c++codes.mk DIRECTORY=../sources TARGET=usercode.po clean
	
clean-combined.po:
	@$(ECHO) "CLEAN combined.po"
	@$(DELETE) combined.po

$(BASE_LIST): targets/list-gen.mk $(MODULE_CFG_FILE)
	@$(MAKE) -f targets/list-gen.mk TARGET=../base EXTENSION=.c OBJ_EXTENSION=.o \
		OUTPUT=$(BASE_LIST) MODULE_NAME=$(MODULE_NAME) generate
		
clean-$(BASE_LIST): targets/list-gen.mk clean-$(MODULE_CFG_FILE)
	@$(MAKE) -f targets/list-gen.mk TARGET=../base EXTENSION=.c OBJ_EXTENSION=.o \
		OUTPUT=$(BASE_LIST) MODULE_NAME=$(MODULE_NAME) clean
		
$(MODULE_CFG_FILE):
	@$(ECHO) "#include <linux/init.h>" > $(MODULE_CFG_FILE)
	@$(ECHO) "#include <linux/kernel.h>" >> $(MODULE_CFG_FILE)
	@$(ECHO) "#include <linux/module.h>" >> $(MODULE_CFG_FILE)
	@$(ECHO) "" >> $(MODULE_CFG_FILE)
	@$(ECHO) "MODULE_LICENSE(\"$(MODULE_LICENSE)\");" >> $(MODULE_CFG_FILE)
	@$(ECHO) "MODULE_AUTHOR(\"$(MODULE_AUTHOR)\");" >> $(MODULE_CFG_FILE)
	@$(ECHO) "const char* __module_name = \"$(MODULE_NAME)\";" >> $(MODULE_CFG_FILE)

clean-$(MODULE_CFG_FILE):
	@$(DELETE) $(MODULE_CFG_FILE)
	
$(KBUILD_MAKEFILE):
	@$(ECHO) "obj-m += $(MODULE_NAME).o" > $(KBUILD_MAKEFILE)
	@$(ECHO) "include $(shell realpath $(BASE_LIST))" >> $(KBUILD_MAKEFILE)
	@$(ECHO) "$(MODULE_NAME)-objs += combined.po" >> $(KBUILD_MAKEFILE)
	@$(ECHO) "EXTRA_CFLAGS += -I$(PWD)/../headers" >> $(KBUILD_MAKEFILE)
	@$(ECHO) "all:" >> $(KBUILD_MAKEFILE)
	@$(ECHO) "\t@$(MAKE) -C $(BUILD_PATH) M=$(PWD) modules" >> $(KBUILD_MAKEFILE)
	@$(ECHO) "clean:" >> $(KBUILD_MAKEFILE)
	@$(ECHO) "\t@$(MAKE) -C $(BUILD_PATH) M=$(PWD) clean" >> $(KBUILD_MAKEFILE)

clean-$(KBUILD_MAKEFILE):
	@$(DELETE) $(KBUILD_MAKEFILE)
	
targets/build-mod.mk-all: combined.po $(BASE_LIST) $(KBUILD_MAKEFILE)
targets/build-mod.mk-clean: $(BASE_LIST) $(KBUILD_MAKEFILE)

$(MODULE_NAME).ko: targets/build-mod.mk-all 
	@$(MAKE) -f targets/build-mod.mk BASE_LIST=$(BASE_LIST) BUILD_PATH=$(BUILD_PATH) \
		MODULE_NAME=$(MODULE_NAME) COMBINED_PO=combined.po all
	
clean-$(MODULE_NAME).ko: targets/build-mod.mk-clean
	@$(MAKE) -f targets/build-mod.mk BASE_LIST=$(BASE_LIST) BUILD_PATH=$(BUILD_PATH) \
		MODULE_NAME=$(MODULE_NAME) COMBINED_PO=combined.po clean
	@$(DELETE) $(KBUILD_MAKEFILE)
	@$(MAKE) -f $(firstword $(MAKEFILE_LIST)) clean-$(BASE_LIST)
	