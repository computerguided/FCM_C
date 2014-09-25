// -------------------------------------------------------------------------------------------------
// Interface.c
// -------------------------------------------------------------------------------------------------

#include "Interface.h"

void ConnectInterfaces(Interface_t* pLeft, Interface_t* pRight)
{
  pLeft->pRemoteInterface = pRight;
  pRight->pRemoteInterface = pLeft;
}
