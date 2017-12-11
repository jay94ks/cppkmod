/*
 * protocol.c
 *
 *  Created on: 2017. 12. 11.
 *      Author: jaehoon
 */

#include <linux/net.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/packet_diag.h>
#include <modcpp/device/network/packet_type_t.hpp>

struct packet_type* ptt_create(void) {
	struct packet_type* pt = kmalloc(sizeof(struct packet_type), GFP_KERNEL);
	memset(pt, 0, sizeof(struct packet_type));
	return pt;
}

void ptt_destroy(struct packet_type* pt) {
	if(pt) kfree(pt);
}

/*
 * below pack and unpack functions rely on the linux implementation of packet_type structure.
 * so, someone notice that modified, please notify me or pull modification through GitHub.
 * */
void ptt_pack(struct packet_type* pt, struct packet_type_t* ptt) {
	pt->type = ptt->type;
	pt->dev = ptt->device;
	*((void**)(&pt->func)) = (void*) ptt->handler;
	pt->af_packet_priv = ptt->af_packet_priv;

	pt->list.prev = ptt->prev;
	pt->list.next = ptt->next;
}

void ptt_unpack(struct packet_type_t* ptt, struct packet_type* pt) {
	ptt->type = pt->type;
	ptt->device = pt->dev;
	*((void**)(&ptt->handler)) = (void*) pt->func;
	ptt->af_packet_priv = pt->af_packet_priv;

	ptt->prev = pt->list.prev;
	ptt->next = pt->list.next;
}
