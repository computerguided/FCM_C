// -------------------------------------------------------------------------------------------------
// fcm_timer_handler.h
//
// -------------------------------------------------------------------------------------------------

#ifndef FCM_TIMER_HANDLER_H
#define FCM_TIMER_HANDLER_H

#include "app_timer.h"
#include "fcm_component.h"
#include "fcm_types.h"

// -------------------------------------------------------------------------------------------------
#undef FCM_COMPONENT_TYPE
#define FCM_COMPONENT_TYPE fcm_timer_handler_t

// -------------------------------------------------------------------------------------------------
#define FCM_MAX_TIMERS   6 // Adjust when more timers are required.
#define FCM_MIN_INTERVAL 5 // Minimum interval.

// -------------------------------------------------------------------------------------------------
#define FCM_TIMER_INIT(timer_id) 			\
	APP_TIMER_DEF(timer_id);				\
	ret_code_t err_code = 				\
		app_timer_create					\
		(								\
			&timer_id,					\
			APP_TIMER_MODE_SINGLE_SHOT,	\
			fcm_timeout_handler			\
		);								\
	APP_ERROR_CHECK(err_code)

#define FCM_REPEATING_TIMER_INIT(timer_id)  \
    APP_TIMER_DEF(timer_id);                \
    ret_code_t err_code =                   \
        app_timer_create                    \
        (                                   \
            &timer_id,                      \
            APP_TIMER_MODE_REPEATED,        \
            fcm_timeout_handler             \
        );                                  \
    APP_ERROR_CHECK(err_code)
// -------------------------------------------------------------------------------------------------
typedef struct
{
	app_timer_id_t   timer_id;
	fcm_component_t* p_component;
	bool             repeating;
} fcm_timeout_entry_t;

// -------------------------------------------------------------------------------------------------
typedef struct
{
	fcm_message_queue_t* p_message_queue;
	fcm_timer_t          timer;
	fcm_timeout_entry_t  timeout[FCM_MAX_TIMERS];
	uint32_t             num_timers;
} FCM_COMPONENT_TYPE;

// -------------------------------------------------------------------------------------------------
void fcm_timer_handler_init(FCM_COMPONENT_TYPE* p_component, fcm_message_queue_t* p_message_queue);

// -------------------------------------------------------------------------------------------------
void fcm_timeout_handler( void* p_context );
bool fcm_start_timer ( fcm_component_t* p_component, app_timer_id_t timer_id, uint32_t interval );
bool fcm_start_repeating_timer ( fcm_component_t* p_component, app_timer_id_t timer_id, uint32_t interval );
void fcm_cancel_timer( fcm_component_t* p_component, app_timer_id_t timer_id );
bool fcm_timer_active( app_timer_id_t timer_id );

// -------------------------------------------------------------------------------------------------
uint64_t fcm_get_system_time(void);

// -------------------------------------------------------------------------------------------------
#endif
