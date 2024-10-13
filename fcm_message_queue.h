// -------------------------------------------------------------------------------------------------
// fcm_message_queue.h
// -------------------------------------------------------------------------------------------------

#ifndef FCM_MESSAGE_QUEUE_H
#define FCM_MESSAGE_QUEUE_H

#include "fcm_interface.h"
#include "fcm_types.h"
#include "fcm_utils.h"

// -------------------------------------------------------------------------------------------------
// Interface messages are placed in message queue memory directly behind an fcm_message_t 'by hand', as such the compiler cannot guarantee
// proper alignment of messages when obtaining these by casting from a 'byte' pointer into the message queue data
// (note: that memory was reserved by means of an array of fcm_message_t's, but is NOT being used as such!)
// make sure these 'emplaced' message structs are properly aligned by aligning this struct to 8.
// Using an alignment of 8 so message parameters of 8 bytes size (int64_t, double) will be properly aligned.
// When changing this struct 8 alignment MUST be preserved!
// When changing this struct NO padding bytes may be introduced behind the p_message_id member!
// Note: ALL message structs also need to be 8 aligned starting from the first parameter!
// (their p_message_id is overlapped onto the fcm_message_t one)

// TODO: maybe this design of the message queue could be reconsidered? non-adherence causes subtle, difficult to trace bugs & possibly different behavior
// for different compiler optimisation settings(!)
typedef struct
{
	uint32_t         system_time;
	fcm_interface_t* p_interface;
	int32_t              message_size;
	char*            p_message_id;
} __attribute__ ((aligned(8))) fcm_message_t;

// TODO: change to 8 always
// Utility macro to check proper alignment of message queue & messages
// Note: the intermediate cast to void* is mandatory! UB otherwise
#define MESSAGE_IS_ALIGNED(POINTER) \
	(((uintptr_t)(const void *)(POINTER)) % 8u == 0)

// -------------------------------------------------------------------------------------------------
typedef struct
{
	fcm_message_t* p_message;
	fcm_message_t* p_write;
	fcm_message_t* p_read;
	void*          p_wrap_around;
	void*          p_end_of_queue;
} fcm_message_queue_t;

// -------------------------------------------------------------------------------------------------
#define FCM_NEXT_MESSAGE(q) \
		q->p_read = (fcm_message_t*)((uint8_t*)(&q->p_read->p_message_id) + q->p_read->message_size); \
		if( q->p_read == q->p_wrap_around && q->p_write != q->p_wrap_around) q->p_read = q->p_message


// -------------------------------------------------------------------------------------------------
void fcm_message_queue_init(fcm_message_queue_t* p_message_queue, int queueSize );

// -------------------------------------------------------------------------------------------------
void* fcm_prepare_message(fcm_message_queue_t* pMsgQueu, char* p_message_id, int message_size);

// -------------------------------------------------------------------------------------------------
void fcm_shift_write_pointer(fcm_message_queue_t* p_message_queue);

// -------------------------------------------------------------------------------------------------
void fcm_send_message(fcm_message_queue_t* p_message_queue, fcm_interface_t* p_interface );

// -------------------------------------------------------------------------------------------------
void fcm_copy_messages(fcm_message_queue_t* p_sub_message_queue, fcm_message_queue_t* p_main_message_queue);

#endif
