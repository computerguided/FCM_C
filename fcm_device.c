// -------------------------------------------------------------------------------------------------
// fcm_device.c
// -------------------------------------------------------------------------------------------------

#include "fcm_device.h"

// TEST
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

// -------------------------------------------------------------------------------------------------
// fcm_process_message
// -------------------------------------------------------------------------------------------------
// Called every time the device is ready to process any pending messages in the message queue.
// -------------------------------------------------------------------------------------------------
// - p_device : pointer to the device.
// -------------------------------------------------------------------------------------------------
bool fcm_process_message(fcm_device_t* p_device)
{
	// -- Copy messages out of sub message queue(s) --
	fcm_message_queue_t* p_message_queue;
	for( uint32_t i=1; i < p_device->num_message_queues; i++ )
	{
		p_message_queue = &p_device->p_message_queue[i];
		while( p_message_queue->p_read != p_message_queue->p_write )
		{
			// At least one message pending in this sub-queue.
		    if( p_message_queue->p_read->p_interface != NULL ) // Was message deleted?
		    {
				fcm_copy_messages( p_message_queue, p_device->p_message_queue );
			}

		    FCM_NEXT_MESSAGE(p_message_queue);
		}
	}

	bool result = false;

	// There is at least one message pending when the read pointer is not equal to the write pointer.
	while( p_device->p_message_queue->p_read != p_device->p_message_queue->p_write )
	{
		if( p_device->p_message_queue->p_read->p_interface != NULL ) // Was message deleted?
		{
			//fcm_component_t* component = (fcm_component_t*)p_device->p_message_queue->p_read->p_interface->p_component;
			//fcm_interface_t* interface = p_device->p_message_queue->p_read->p_interface;
            //
			//NRF_LOG_INFO ("Processing message '%s' on interface '%s' for component '%s' in state '%s'", p_device->p_message_queue->p_read->p_message_id, interface->p_name, component->p_name, ((fcm_state_t *)component->p_current_state->p_reference)->p_name);
			//NRF_LOG_FLUSH ();

			// Handle pending message by calling the state machine engine and
			// supplying the component, interface and message.
			result = (fcm_state_machine_engine(
					(fcm_component_t*)p_device->p_message_queue->p_read->p_interface->p_component,
					p_device->p_message_queue->p_read->p_interface,
					p_device->p_message_queue->p_read->p_message_id ) == FCM_SMR_OK );

			FCM_NEXT_MESSAGE(p_device->p_message_queue);
			return result; // Handle one message per loop.
		}
		FCM_NEXT_MESSAGE(p_device->p_message_queue);
	}

	return result;
}
