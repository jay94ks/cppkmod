/*
 * protocol.hpp
 *
 *  Created on: 2017. 12. 11.
 *      Author: jaehoon
 */

#ifndef HEADERS_MODCPP_DEVICE_NETWORK_PROTOCOL_HPP_
#define HEADERS_MODCPP_DEVICE_NETWORK_PROTOCOL_HPP_

#ifdef __cplusplus
#include <modcpp/containers/vector.hpp>
#include <modcpp/kthread/mutex.hpp>
#endif

/*
 * WARNING: this header file will be included by C/C++ source codes.
 * Be careful when you try to append more codes.
 *
 * the forward type for the structure, protocol_type.
 * other fields excluding below will be ignored or skipped.
 * and the implementation of cppkmod can cause software overhead when it perform adaption.
 * */
typedef struct protocol_type_t {

	uint16_t				type;			// protocol type
	struct net_device*		device;			// device, if this field filled as NULL, it will point all.

	int32_t					(* handler) (struct sk_buff*, struct net_device*, struct packet_type*);
	/*
	 * this handler will be called by the receive routine.
	 * when the type field is same with proto.type == type.
	 * */

	void*					af_packet_priv;

	struct list_head*		prev;
	struct list_head*		next;

} protocol_type_t;

/*
 * below functions are cppkmod framework specific except dev_*_pack.
 * you cannot access the structure, packet_type, so it encapsulate them.
 * even more, you need to allocate the packet_type structure for serving them,
 * 	but, your C++ compiler cannot calculate the size of packet_type structure.
 *
 * for installation of protocol handler,
 * 		struct packet_type* pt = ptt_create();
 * 		protocol_type_t ptt = {0, };
 *
 * 		if(pt) {
 *				... build ptt ...
 *
 *				ptt_pack(pt, &ptt);
 *				dev_add_pack(pt);
 * 		}
 *
 * for uninstallation of protocol handler,
 * 		struct packet_type* pt = ....
 *
 * 		if(pt) {
 *				dev_remove_pack(pt);
 * 		}
 *
 * when you want to modify the handler after registration,
 * 		struct packet_type* pt = ...;
 * 		protocol_type_t ptt;
 *
 * 		if(pt) {
 * 			dev_remove_pack(pt);
 * 			ptt_unpack(&ptt, pt);
 *
 * 			.... modify ....
 *
 * 			ptt_pack(pt, &ptt);
 * 			dev_add_pack(pt);
 * 		}
 *
 *	WARNING: DO NOT call dev_*_pack frequently.
 *
 * for fetching the packet_type structure into protocol_handler_t,
 *
 * 		int32_t ptthandler (struct sk_buff* p, struct net_device* dev, struct packet_type* pt) {
 * 			protocol_type_t ptt;
 * 			ptt_unpack(&ptt, pt);
 *
 *			... do something ...
 *
 * 			return ...;
 * 		}
 *
 * 		... ptt.handler = ptthandler; ...
 *
 * the function, ptt_create, uses kmalloc(sizeof(...), GFP_KERNEL).
 * and ptt_destroy uses kfree(...). so, be careful when you use that function.
 * */

#ifdef __cplusplus
KMOD_FORWARD struct packet_type* ptt_create(void);
KMOD_FORWARD void ptt_destroy(struct packet_type* pt);
KMOD_FORWARD void ptt_pack(struct packet_type* pt, struct protocol_type_t* ptt);
KMOD_FORWARD void ptt_unpack(struct protocol_type_t* ptt, struct packet_type* pt);
KMOD_FORWARD void dev_add_pack(struct packet_type* pt);
KMOD_FORWARD void dev_remove_pack(struct packet_type* pt);

/*
 * experimental implementation of capsulation for protocol handler.
 * */
class protocol_handler {
public:
	protocol_handler(uint16_t type, struct net_device* target = 0)
	: installed(false)
	{
		pt = ptt_create();
		ptt.handler = onProxy;
		ptt.device = target;
		ptt.type = type;

		/*
		 * below field must be filled....
		 * but, how can i....???
		 * */
		ptt.af_packet_priv = 0;
	}

	virtual ~protocol_handler() {
		if(installed) this->uninstall();
		if(pt) ptt_destroy(pt);
	}

private:
	struct packet_type*		pt;
	protocol_type_t			ptt;
	bool					installed;

protected:
	// this method must be implemented by inherited class.
	virtual int32_t handle(struct sk_buff* skb, struct net_device* netdev) = 0;

private:
	typedef vector<protocol_handler*, kmalloc_allocator<protocol_handler*, GFP_KERNEL> > ptt_vector;

	inline static kmutex* getMutex() {
		static kmutex mutex;
		return &mutex;
	}

	inline static ptt_vector* getPTTList() {
		static ptt_vector _pttlist;
		/* enforce the strategy of pttlist as double allocation. */
		_pttlist.setStrategy(E_ALLOC_DOUBLE);
		return &_pttlist;
	}

public:
	inline bool install() {
		kmutex* mutex = getMutex();

		if(installed)
			return false;

		ptt_pack(pt, &ptt);
		dev_add_pack(pt);
		mutex->lock();

		// register the mapping information of proxy.
		getPTTList()->add(this);

		mutex->unlock();
		installed = true;

		return true;
	}

	inline bool uninstall() {
		ptt_vector* pttlist = getPTTList();
		kmutex* mutex = getMutex();
		if(!installed)
			return false;

		dev_remove_pack(pt);
		ptt_unpack(&ptt, pt);
		mutex->lock();

		for(size_t i = 0; i < pttlist->size(); i++) {
			protocol_handler* ph = *(pttlist)[i];
			if(ph->pt == pt) {
				// unregister the mapping information of proxy.
				pttlist->remove(i);
				break;
			}
		}

		mutex->unlock();
		installed = false;

		return true;
	}

private:
	inline static int32_t onProxy(struct sk_buff* skb, struct net_device* netdev, struct packet_type* pt) {
		ptt_vector* pttlist = getPTTList();
		kmutex* mutex = getMutex();

		// lock the list mutex.
		mutex->lock();

		/*
		 * finding the protocol handler what packet_type fit on.
		 * please DO NOT replace this implementation to be using iterator.
		 * it will cause a lot of overhead. because this method will be called frequently.
		 * */
		for(size_t i = 0; i < pttlist->size(); i++) {
			protocol_handler* ph = *(pttlist)[i];
			if(ph->pt == pt) {
				return ph->handle(skb, netdev);
			}
		}

		// unlock the list mutex.
		mutex->unlock();
		return -1;
	}
};

#endif



#endif /* HEADERS_MODCPP_DEVICE_NETWORK_PROTOCOL_HPP_ */
