
include ModuleInfo.mk

all:
	@cd build && make -f build.mk all
	
clean:
	@cd build && make -f build.mk clean
	
clean-all:
	@cd build && make -f build.mk clean-all

rebuild:
	@make clean
	@make all
	
rebuild-all:
	@make clean-all
	@make all
	
insmod: all
	@sudo insmod build/$(MODULE_NAME).ko 
	
rmmod: all
	@sudo rmmod $(MODULE_NAME)