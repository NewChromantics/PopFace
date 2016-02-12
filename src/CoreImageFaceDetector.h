#pragma once

#include "TFaceDetector.h"


namespace CoreImage
{
	class TContext;
	class TDetector;
}


class CoreImageFaceDetector : public TFaceDetector
{
public:
	CoreImageFaceDetector(TFaceDetectorParams Params,std::shared_ptr<Opengl::TContext> OpenglContext);

	virtual void		PushTexture(Opengl::TTexture& Texture) override;

public:
	std::shared_ptr<CoreImage::TContext>	mCoreImageContext;
	std::shared_ptr<CoreImage::TDetector>	mCoreImageDetector;
	std::shared_ptr<Opengl::TContext>		mOpenglContext;
};
