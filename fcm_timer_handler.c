// -------------------------------------------------------------------------------------------------
// fcm_timer_handler.c
// Timer Handler component.
// -------------------------------------------------------------------------------------------------

#include "fcm_timer_handler.h"

// -------------------------------------------------------------------------------------------------
static uint32_t m_ticks_old;
static uint64_t m_ticks;

FCM_COMPONENT_TYPE* m_p_component;

// -------------------------------------------------------------------------------------------------
// Definition of local functions.
// -------------------------------------------------------------------------------------------------
static bool delete_timeout_indication( fcm_message_queue_t* p_message_queue, fcm_timer_t* p_interface, app_timer_id_t timer_id );
static void clear_timeout_entry( FCM_COMPONENT_TYPE* p_component, fcm_timeout_entry_t* p_timeout_entry );
static bool fcm_set_timer ( fcm_component_t* p_component, app_timer_id_t timer_id, uint32_t interval, bool repeating );

// -------------------------------------------------------------------------------------------------
// fcm_timer_handler_init
// -------------------------------------------------------------------------------------------------
void fcm_timer_handler_init(FCM_COMPONENT_TYPE* p_component, fcm_message_queue_t* p_message_queue)
{
	// -- Initialize interfaces --
	FCM_SET_INTERFACE(timer);
	FCM_SET_MESSAGE_QUEUE(p_message_queue);

	// Initialize state variables.
	p_component->num_timers = 0;
	for( uint32_t i = 0; i < FCM_MAX_TIMERS; i++)
	{
		clear_timeout_entry( p_component, &p_component->timeout[i] );
	}

    // Initialize timer module, making it use the scheduler
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    m_p_component = p_component;
}

// -------------------------------------------------------------------------------------------------
static void clear_timeout_entry( FCM_COMPONENT_TYPE* p_component, fcm_timeout_entry_t* p_timeout_entry )
{
	p_timeout_entry->p_component = NULL;
	p_timeout_entry->timer_id   = NULL;
	if( p_component->num_timers > 0 ) p_component->num_timers--;
}

// -------------------------------------------------------------------------------------------------
void fcm_timeout_handler( void* p_context )
{
  fcm_timeout_entry_t* p_timeout_data = p_context;

  fcm_timer_handler_t* p_component =
    ((fcm_interface_t*)p_timeout_data->p_component->timer.p_remote_interface)->p_component;

  FCM_PREPARE_MESSAGE(timer,timeout_ind);
  p_component->timer.timeout_ind->timer_id = p_timeout_data->timer_id;

  // Expand SEND_MESSAGE to determine remote interface explicitly.
  p_component->p_message_queue->p_write->system_time = fcm_get_system_time();
  p_component->p_message_queue->p_write->p_interface = (fcm_interface_t*)&p_timeout_data->p_component->timer;

  fcm_shift_write_pointer(p_component->p_message_queue);
  p_component->timer.timeout_ind = NULL;

  if( !p_timeout_data->repeating ) clear_timeout_entry( p_component, p_timeout_data);
}

// -------------------------------------------------------------------------------------------------
static bool delete_timeout_indication( fcm_message_queue_t* p_message_queue, fcm_timer_t* p_interface, app_timer_id_t timer_id )
{
    fcm_message_t* p_message = p_message_queue->p_read;

    while( p_message != p_message_queue->p_write )
    {
        if( p_message->p_interface == (fcm_interface_t *)p_interface &&
            p_interface->timeout_ind->p_message_id == p_message->p_message_id &&
            p_interface->timeout_ind->timer_id == timer_id )
        {
            // Message found, 'delete' it.
            p_message->p_interface = NULL;
            return true;
        }

        // Shift to next message.
        p_message = (fcm_message_t *)((uint8_t*)&p_message->p_message_id+p_message->message_size);
        if( (p_message != p_message_queue->p_write) && (p_message == p_message_queue->p_wrap_around) ) p_message = p_message_queue->p_message;
    }
    return false;
}


// -------------------------------------------------------------------------------------------------
static bool fcm_set_timer ( fcm_component_t* p_component, app_timer_id_t timer_id, uint32_t interval, bool repeating )
{
    ret_code_t err_code;

    if( interval < FCM_MIN_INTERVAL ) return NULL;

    // Get the pointer to the Timer Handler connected to the calling component.
    fcm_timer_handler_t*  p_timer_handler = ((fcm_interface_t*)p_component->timer.p_remote_interface)->p_component;

    if( p_timer_handler->num_timers == FCM_MAX_TIMERS ) return false;

    // -- Find the empty entry in the timeout array --

    for( uint32_t i = 0; i < FCM_MAX_TIMERS; i++ )
    {
        if( !p_timer_handler->timeout[i].timer_id )
        {
            // Empty element found.
            err_code = app_timer_start(timer_id, APP_TIMER_TICKS(interval), &p_timer_handler->timeout[i]);
            if( err_code == NRF_SUCCESS )
            {
                p_timer_handler->timeout[i].timer_id    = timer_id;
                p_timer_handler->timeout[i].p_component = p_component;
                p_timer_handler->timeout[i].repeating   = repeating;
                p_timer_handler->num_timers++;
                return true;
            }
            break;
        }
    }

    // Starting the timer failed (not probable).

    NRF_LOG_ERROR( "Failed starting timer!" );
    NRF_LOG_FLUSH();

    return false;
}

// ---- Timer interface functions ----

// -------------------------------------------------------------------------------------------------
// fcm_start_timer
// -------------------------------------------------------------------------------------------------
// Start a timer. Returns true when successful.
// Failure can occur when no more timers can be add or interval below MIN_INTERVAL.
// -------------------------------------------------------------------------------------------------
// p_component : pointer to the component that requested the timer.
// timer_id    : the created timer_id.
// interval    : number of milliseconds after the currentTime at which the timer needs to expire.
// -------------------------------------------------------------------------------------------------
bool fcm_start_timer ( fcm_component_t* p_component, app_timer_id_t timer_id, uint32_t interval )
{
    return fcm_set_timer( p_component, timer_id, interval, false );
}
// -------------------------------------------------------------------------------------------------
bool fcm_start_repeating_timer ( fcm_component_t* p_component, app_timer_id_t timer_id, uint32_t interval )
{
    return fcm_set_timer( p_component, timer_id, interval, true );
}

// -------------------------------------------------------------------------------------------------
void fcm_cancel_timer( fcm_component_t* p_component, app_timer_id_t timer_id )
{
	// First, stop the timer.
	ret_code_t err_code = app_timer_stop( timer_id );
	APP_ERROR_CHECK(err_code);

	fcm_timer_handler_t* p_timer_handler = ((fcm_interface_t*)p_component->timer.p_remote_interface)->p_component;

	// Find the indicated timer id.
	for( uint32_t i = 0; i < FCM_MAX_TIMERS; i++ )
	{
		if( p_timer_handler->timeout[i].timer_id == timer_id )
		{
			// Clear the timeout entry.
			clear_timeout_entry( p_timer_handler, &p_timer_handler->timeout[i] );
			return;
		}
	}

	// Timer already expired.
	if( delete_timeout_indication(p_timer_handler->p_message_queue, &p_component->timer, timer_id) ) return;
	delete_timeout_indication(p_component->p_message_queue, &p_component->timer, timer_id);
}

// -------------------------------------------------------------------------------------------------
bool fcm_timer_active( app_timer_id_t timer_id )
{
    timer_node_t * p_node = (timer_node_t*)timer_id;

    if ((timer_id == NULL) || (p_node->p_timeout_handler == NULL))
    {
        NRF_LOG_ERROR( "Invalid timer!" );
        NRF_LOG_FLUSH();

        return false;
    }

    return p_node->is_running;
}

// -------------------------------------------------------------------------------------------------
void system_timer_timeout_handler( void* p_context )
{
    uint32_t ticks_now = app_timer_cnt_get();
    uint32_t diff = app_timer_cnt_diff_compute( ticks_now, m_ticks_old );

    m_ticks += diff;
    m_ticks_old = ticks_now;

    NRF_LOG_INFO("System time: %li", fcm_get_system_time() );
    NRF_LOG_FLUSH();
}

// -------------------------------------------------------------------------------------------------
uint64_t fcm_get_system_time()
{
    uint32_t ticks_now = app_timer_cnt_get();
    uint32_t diff = app_timer_cnt_diff_compute( ticks_now, m_ticks_old );

    m_ticks += diff;
    m_ticks_old = ticks_now;

    uint64_t time_ms = (uint64_t) ( (1000*m_ticks) / 32768 );

    return time_ms;
}

