#pragma once

#include <SoyOpengl.h>

class TFaceDetectorParams
{
	
};

class TFaceDetector
{
public:
	TFaceDetector(TFaceDetectorParams Params);

	virtual void		PushTexture(Opengl::TTexture& Texture)=0;
	void				PopData(std::stringstream& Data);
	void				OnData(const std::string& Data);
	
public:
	std::mutex			mDataLock;
	std::string			mData;
};
