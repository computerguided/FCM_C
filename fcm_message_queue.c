// -------------------------------------------------------------------------------------------------
// fcm_message_queue.c
// -------------------------------------------------------------------------------------------------

#include <string.h> // For memcpy
#include "fcm_message_queue.h"
#include "fcm_timer_handler.h" // For system time

#include "error_handler.h"

// TEST
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

// -------------------------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------------------
// fcm_message_queue_init
// -------------------------------------------------------------------------------------------------
// Initialize the message queue administration.
// -------------------------------------------------------------------------------------------------
// - p_message_queue : pointer to the created message queue administration.
// - queue_size : the size of the message queue expressed in number of data-less messages.
// -------------------------------------------------------------------------------------------------
void fcm_message_queue_init(fcm_message_queue_t* p_message_queue, int queue_size )
{
	p_message_queue->p_write 		= p_message_queue->p_message;
	p_message_queue->p_read 			= p_message_queue->p_write;
	p_message_queue->p_wrap_around 	= &p_message_queue->p_message[queue_size-1]+1;
	p_message_queue->p_end_of_queue 	= &p_message_queue->p_message[queue_size-1]+1;
}

// -------------------------------------------------------------------------------------------------
// fcm_prepare_message
// -------------------------------------------------------------------------------------------------
// Make room for the new message and get the location in the new message in the message queue
// and set the fields already.
// -------------------------------------------------------------------------------------------------
// - p_message_queue : pointer to the message queue.
// - p_message_id : message id, implemented as a pointer to a literal string.
// - message_size : size of the message.
// -------------------------------------------------------------------------------------------------
void* fcm_prepare_message(fcm_message_queue_t* p_message_queue, char* p_message_id, int message_size)
{

	// Wrap around when there is not enough room.
	if( sizeof(fcm_message_t)+message_size-sizeof(char*) >
		(uint8_t*)p_message_queue->p_end_of_queue-(uint8_t*)p_message_queue->p_write)
	{
		// Note that when the message is not sent in the transition,
		// this wrap around is done for nothing.
		p_message_queue->p_wrap_around = p_message_queue->p_write;
		p_message_queue->p_write = p_message_queue->p_message;

		// Read pointer
		if( p_message_queue->p_read == p_message_queue->p_wrap_around )
		  p_message_queue->p_read = p_message_queue->p_message;

	}
	p_message_queue->p_write->p_message_id = p_message_id;
	p_message_queue->p_write->message_size = message_size;

	//NRF_LOG_INFO ("Preparing message '%s' of size 12+%d at message queue %d", p_message_id, message_size, (uint8_t*)(p_message_queue->p_write) - (uint8_t*)(p_message_queue->p_message));
	//NRF_LOG_FLUSH ();

	return &p_message_queue->p_write->p_message_id;
}

// -------------------------------------------------------------------------------------------------
// fcm_shift_write_pointer
// -------------------------------------------------------------------------------------------------
// Shift the write-pointer.
// -------------------------------------------------------------------------------------------------
// - p_message_queue : pointer to the message queue.
// -------------------------------------------------------------------------------------------------
void fcm_shift_write_pointer(fcm_message_queue_t* p_message_queue)
{
	uint8_t* old_write = (uint8_t*) p_message_queue->p_write;

	p_message_queue->p_write = (fcm_message_t *)((uint8_t*)&p_message_queue->p_write->p_message_id + p_message_queue->p_write->message_size);
	if( (void *)p_message_queue->p_write > (void *)p_message_queue->p_wrap_around )
		// 'Reset' the wrap-around pointer.
		p_message_queue->p_wrap_around = p_message_queue->p_end_of_queue;
	// If the write pointer is now pointing at the end of the queue.
	if( p_message_queue->p_write == p_message_queue->p_end_of_queue )
		p_message_queue->p_write = p_message_queue->p_message;

	// check for overflow of queue (write pointer passing read pointer)
	if ((old_write < (uint8_t*)(p_message_queue->p_read)) && ((uint8_t*)(p_message_queue->p_write) >= (uint8_t*)(p_message_queue->p_read)))
	{
		NRF_LOG_ERROR("Overflow of message queue 0x%x, message(s) have been lost!", p_message_queue)
		NRF_LOG_FLUSH();

		error_handler_set_error(VP_ERROR_CATEGORY_FCM, VP_FCM_MESSAGE_QUEUE_OVERFLOW);
	}
}

// -------------------------------------------------------------------------------------------------
// fcm_send_message
// -------------------------------------------------------------------------------------------------
// Set the last fields and shift the write-pointer.
// -------------------------------------------------------------------------------------------------
// - p_message_queue : pointer to the message queue.
// - p_interface : pointer to the interface on which the message is to be sent.
// -------------------------------------------------------------------------------------------------
void fcm_send_message(fcm_message_queue_t* p_message_queue, fcm_interface_t* p_interface )
{
	p_message_queue->p_write->system_time = fcm_get_system_time();
	p_message_queue->p_write->p_interface = p_interface->p_remote_interface;

	fcm_shift_write_pointer( p_message_queue );

	//NRF_LOG_INFO ("Sending message on interface 0x%x (%s) of component '%s' (message queue now at %d)", p_interface->p_remote_interface, ((fcm_interface_t*)(p_interface->p_remote_interface))->p_name, ((fcm_component_t*)(((fcm_interface_t*)(p_interface->p_remote_interface))->p_component))->p_name, (uint8_t*)(p_message_queue->p_write) - (uint8_t*)(p_message_queue->p_message));
	//NRF_LOG_FLUSH ();
}

// -------------------------------------------------------------------------------------------------
// fcm_copy_messages
// -------------------------------------------------------------------------------------------------
// Copy all the messages from one message queue into the other.
// -------------------------------------------------------------------------------------------------
// - p_sub_message_queue : pointer to the message queue from which the message must be copied.
// - p_main_message_queue : pointer to the message queue to which the message must be copied.
// -------------------------------------------------------------------------------------------------
void fcm_copy_messages(fcm_message_queue_t* p_sub_message_queue, fcm_message_queue_t* p_main_message_queue)
{
	// -- Copy message --

	// Create room in destination message queue.
	fcm_prepare_message(p_main_message_queue, p_sub_message_queue->p_read->p_message_id, p_sub_message_queue->p_read->message_size );
	
    p_main_message_queue->p_write->p_interface  = p_sub_message_queue->p_read->p_interface;
    p_main_message_queue->p_write->system_time  = fcm_get_system_time();

	// Copy the message parameters (includes message id).
	memcpy( &p_main_message_queue->p_write->p_message_id, &p_sub_message_queue->p_read->p_message_id, p_sub_message_queue->p_read->message_size );

	// Now 'send' the message by shifting the write-pointer.
	fcm_shift_write_pointer(p_main_message_queue);
}


