MAKE=make
ECHO=echo
DELETE=rm -rf

obj-m += $(MODULE_NAME).o
EXTRA_CFLAGS += -I$(PWD)/../headers

include $(BASE_LIST)

$(MODULE_NAME)-objs += $(COMBINED_PO)

all:
	@$(MAKE) -C $(BUILD_PATH) M=$(PWD) modules

clean:
	@$(MAKE) -C $(BUILD_PATH) M=$(PWD) clean
	@$(DELETE) $($(MODULE_NAME)-objs)
