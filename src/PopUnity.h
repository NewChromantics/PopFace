#pragma once

#include <SoyUnity.h>


//	some global params
namespace Unity
{
	static size_t		mMinTimerMs = 5;
	static bool			mDebugPluginEvent = false;
	static Unity::sint	mEventId = 0x6fff678;
	static bool			mVerboseUpdateFunctions = false;	//	debug errors if UpdateTexture, UpdatePerfGraph, UpdateAudio etc fails
	
	extern int			GetPluginEventId();
};


//	unique eventid so that other plugins don't trigger redundant opengl thread calls
__export Unity::sint PopFace_GetPluginEventId();


