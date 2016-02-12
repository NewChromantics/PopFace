#include "PopUnity.h"




__export Unity::sint PopFace_GetPluginEventId()
{
	return Unity::GetPluginEventId();
}

int Unity::GetPluginEventId()
{
	return mEventId;
}


