#include <modcpp.h>
#include <modcpp/kthread.hpp>
#include <modcpp/containers.hpp>
#include <modcpp/device/network.hpp>

kthread* theThread = 0;
class A {
public:
	A() { }
	A(int) { printk(KERN_INFO "%s: hello~! 0x%x", __module_name, this);}
	A(const A& cl) {
		printk(KERN_INFO "%s: clone~! 0x%x", __module_name, this);
	}
	~A() { printk(KERN_INFO "%s: bye~! 0x%x", __module_name, this); }
};

int threadWork(kthread* thread, void* arg);

vector<A> a;

KMOD_FORWARD int koMain(void) {
	theThread = new kthread(threadWork);
	theThread->start("My Thread");

	a.setStrategy(E_ALLOC_QUADRA);

	a.add (100);
	a.add (100);
	a.add (100);
	a.add (100);
	a.add (100);
	a.add (100);


	return 0;
}

KMOD_FORWARD int koExit(void) {
	for(size_t i = 0; i < a.size(); i++)
		printk(KERN_INFO "%d - %d\n", i, a[i]);

	a.clear();
	a.collapse();

	theThread->stop();
	return 0;
}

int threadWork(kthread* thread, void* arg) {
	while(kthread::shouldStop()) {

		yield();
	}

	return 0;
}
