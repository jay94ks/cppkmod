/*
 * protocol.hpp
 *
 *  Created on: 2017. 12. 11.
 *      Author: jaehoon
 */

#ifndef HEADERS_MODCPP_DEVICE_NETWORK_PROTOCOL_HPP_
#define HEADERS_MODCPP_DEVICE_NETWORK_PROTOCOL_HPP_

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

	int32_t					(* handler) (struct sk_buff*, struct net_device*, struct protocol_type_t*);
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
#endif

#endif /* HEADERS_MODCPP_DEVICE_NETWORK_PROTOCOL_HPP_ */
