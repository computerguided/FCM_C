// -------------------------------------------------------------------------------------------------
// fcm_device.h
// -------------------------------------------------------------------------------------------------
#ifndef FCM_DEVICE_H
#define FCM_DEVICE_H
// -------------------------------------------------------------------------------------------------

#include "fcm_message_queue.h"
#include "fcm_state_machine_engine.h"
#include "fcm_types.h"
#include "fcm_timer_handler.h"

typedef struct
{
	uint32_t             num_message_queues;
	fcm_message_queue_t* p_message_queue;
} fcm_device_t;

#define FCM_DEVICE_BASETYPE_FIELDS \
		int num_message_queues; \
		fcm_message_queue_t* p_message_queue; \
		fcm_message_queue_t message_queue[FCM_NUM_MESSAGE_QUEUES]

// -------------------------------------------------------------------------------------------------
bool fcm_process_message(fcm_device_t *p_device);

// -------------------------------------------------------------------------------------------------
// Initialization macros
// -------------------------------------------------------------------------------------------------

#define FCM_DEVICE_INIT_FUNCTION(name) \
		void name##_init(name##_t* p_device)

#define FCM_SET_DEVICE_DEFAULT_FIELDS \
		p_device->num_message_queues = FCM_NUM_MESSAGE_QUEUES; \
		p_device->p_message_queue = p_device->message_queue

#define FCM_INIT_MESSAGE_QUEUE(i,s) \
		static fcm_message_t _messages_belonging_to_message_queue_##i[s]; \
		p_device->message_queue[i].p_message = _messages_belonging_to_message_queue_##i; \
		_Static_assert(MESSAGE_IS_ALIGNED(_messages_belonging_to_message_queue_##i), "Message queue must be 8 aligned!"); \
		fcm_message_queue_init(&p_device->message_queue[i], s)

#define FCM_INIT_COMPONENT(c,x) \
		c##_init(&p_device->c,&p_device->message_queue[x])

#define FCM_CONNECT_INTERFACES(i,c_left,c_right) \
		fcm_connect_interfaces((fcm_interface_t*)&p_device->c_left.i, (fcm_interface_t*)&p_device->c_right.i)

#define FCM_CONNECT_TIMER(c,t) \
		p_device->c.timer.p_remote_interface = (fcm_interface_t*)&p_device->t.timer

#endif
