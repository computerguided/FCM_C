// -------------------------------------------------------------------------------------------------
// Utils.c
// -------------------------------------------------------------------------------------------------

#include <Types.h>
#include <Utils.h>


uint32_t GetSystemTime()
{
	// Place platform specific code here...


	static uint32_t t=0;
	t++;


	return (uint32_t)t;
}
