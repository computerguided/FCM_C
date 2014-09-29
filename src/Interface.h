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
			char* pMsgId; \
			__VA_ARGS__ \
			char eom; \
		}* m; \
		char* p##m##_id

#define SET_MESSAGE_ID(m) \
		pInterface->p##m##_id = #m

// -------------------------------------------------------------------------------------------------
#define MESSAGE_SIZE(m) \
		(address_t)(&m->eom)-(address_t)(&m->pMsgId)

// -------------------------------------------------------------------------------------------------
void ConnectInterfaces(Interface_t* pLeft, Interface_t* pRight);



#endif




