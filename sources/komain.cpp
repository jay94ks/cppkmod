#include <modcpp.h>
#include <modcpp/kthread.hpp>

kthread* theThread = 0;
class A {
public:
	A() { printk(KERN_INFO "%s: hello~! 0x%x", __module_name, this);}
	~A() { printk(KERN_INFO "%s: bye~! 0x%x", __module_name, this); }
};

int threadWork(kthread* thread, void* arg);

A greetings;

KMOD_FORWARD int koMain(void) {
	theThread = new kthread(threadWork);
	theThread->start("My Thread");
	return 0;
}

KMOD_FORWARD int koExit(void) {
	theThread->stop();
	return 0;
}

int threadWork(kthread* thread, void* arg) {
	while(kthread::shouldStop()) {

		yield();
	}

	return 0;
}
