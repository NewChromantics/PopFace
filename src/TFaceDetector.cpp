#include "TFaceDetector.h"



TFaceDetector::TFaceDetector(TFaceDetectorParams Params)
{
	
}

void TFaceDetector::PopData(std::stringstream& Data)
{
	std::lock_guard<std::mutex> Lock( mDataLock );
	Data << mData;
	mData = std::string();
}


void TFaceDetector::OnData(const std::string& Data)
{
	std::lock_guard<std::mutex> Lock( mDataLock );
	mData = Data;
}


