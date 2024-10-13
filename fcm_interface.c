// -------------------------------------------------------------------------------------------------
// fcm_interface.c
// -------------------------------------------------------------------------------------------------

#include "fcm_interface.h"

void fcm_connect_interfaces(fcm_interface_t* p_left, fcm_interface_t* p_right)
{
  p_left->p_remote_interface = p_right;
  p_right->p_remote_interface = p_left;
}
