// -------------------------------------------------------------------------------------------------
// Interface.h
// -------------------------------------------------------------------------------------------------

#ifndef INTERFACE_H
#define INTERFACE_H

#include "Types.h"

typedef struct
{
	void* pRemoteInterface;
	void* pComponent;
} Interface_t;

#define INTERFACE_BASETYPE_FIELDS \
		void* pRemoteInterface; \
		void* pComponent

#define MESSAGE_DEF(m,...) \
		struct \
		{ \
			char* pId; \
			__VA_ARGS__ \
			char eom; \
		}* m; \
		char* p##m##_id

#define SET_MESSAGE_ID(m) \
		pInterface->p##m##_id = #m

#endif




