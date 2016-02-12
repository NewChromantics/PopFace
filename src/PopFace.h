#pragma once

#include "PopUnity.h"


class TFaceDetector;
class TFaceDetectorParams;


__export Unity::ulong	PopFace_Alloc();
__export bool			PopFace_Free(Unity::ulong Instance);
__export bool			PopFace_PushRenderTexture(Unity::ulong Instance,Unity::NativeTexturePtr TextureId,Unity::sint Width,Unity::sint Height,Unity::RenderTexturePixelFormat::Type PixelFormat);
__export bool			PopFace_PushTexture2D(Unity::ulong Instance,Unity::NativeTexturePtr TextureId,Unity::sint Width,Unity::sint Height,Unity::Texture2DPixelFormat::Type PixelFormat);
__export Unity::sint	PopFace_PopData(Unity::ulong Instance,char* Buffer,Unity::uint BufferSize);



namespace PopFace
{
	class TInstance;
	typedef Unity::ulong	TInstanceRef;

	std::shared_ptr<TInstance>	Alloc(TFaceDetectorParams Params,std::shared_ptr<Opengl::TContext> OpenglContext);
	std::shared_ptr<TInstance>	GetInstance(TInstanceRef Instance);
	bool						Free(TInstanceRef Instance);
};



class PopFace::TInstance
{
public:
	TInstance()=delete;
	TInstance(const TInstance& Copy)=delete;
public:
	explicit TInstance(const TInstanceRef& Ref,TFaceDetectorParams Params,std::shared_ptr<Opengl::TContext> OpenglContext);
	
	TInstanceRef	GetRef() const		{	return mRef;	}

	void			PushTexture(Opengl::TTexture Texture);
	void			PopData(std::stringstream& Data);
	
public:
	std::shared_ptr<Opengl::TContext>	mOpenglContext;
	std::shared_ptr<TFaceDetector>		mDetector;

private:
	TInstanceRef	mRef;
};


