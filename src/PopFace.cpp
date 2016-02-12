#include "PopFace.h"
#include "PopUnity.h"
#include "TFaceDetector.h"

#if defined(TARGET_OSX) || defined(TARGET_IOS)
#include "CoreImageFaceDetector.h"
#endif

namespace PopFace
{
	std::mutex gInstancesLock;
	std::vector<std::shared_ptr<PopFace::TInstance> >	gInstances;
};





__export Unity::ulong	PopFace_Alloc()
{
	TFaceDetectorParams Params;
	try
	{
		auto OpenglContext = Unity::GetOpenglContextPtr();
		
		auto Instance = PopFace::Alloc( Params, OpenglContext );
		if ( !Instance )
			return 0;
		return Instance->GetRef();
	}
	catch ( std::exception& e )
	{
		std::Debug << "Failed to allocate movie: " << e.what() << std::endl;
		return 0;
	}
	/*
	catch (...)
	{
		std::Debug << "Failed to allocate movie: Unknown exception in " << __func__ << std::endl;
		return 0;
	}
	 */
	
	return 0;
}

__export bool	PopFace_Free(Unity::ulong Instance)
{
	ofScopeTimerWarning Timer(__func__, Unity::mMinTimerMs );
	return PopFace::Free( Instance );
}



__export bool	PopFace_PushRenderTexture(Unity::ulong Instance,Unity::NativeTexturePtr TextureId,Unity::sint Width,Unity::sint Height,Unity::RenderTexturePixelFormat::Type PixelFormat)
{
	auto pInstance = PopFace::GetInstance( Instance );
	if ( !pInstance )
		return false;

	try
	{
		//	assuming type atm... maybe we can extract it via opengl?
		SoyPixelsMeta Meta( Width, Height, Unity::GetPixelFormat( PixelFormat ) );
		GLenum Type = GL_TEXTURE_2D;
		Opengl::TTexture Texture( TextureId, Meta, Type );
		pInstance->PushTexture( Texture );
		return true;
	}
	catch(std::exception& e)
	{
		std::Debug << __func__ << " failed: " << e.what() << std::endl;
		return false;
	}
}


__export bool	PopFace_PushTexture2D(Unity::ulong Instance,Unity::NativeTexturePtr TextureId,Unity::sint Width,Unity::sint Height,Unity::Texture2DPixelFormat::Type PixelFormat)
{
	auto pInstance = PopFace::GetInstance( Instance );
	if ( !pInstance )
		return false;
	
	try
	{
		//	assuming type atm... maybe we can extract it via opengl?
		SoyPixelsMeta Meta( Width, Height, Unity::GetPixelFormat( PixelFormat ) );
		GLenum Type = GL_TEXTURE_2D;
		Opengl::TTexture Texture( TextureId, Meta, Type );
		pInstance->PushTexture( Texture );
		return true;
	}
	catch(std::exception& e)
	{
		std::Debug << __func__ << " failed: " << e.what() << std::endl;
		return false;
	}
}

__export Unity::sint		PopFace_PopData(Unity::ulong Instance,char* Buffer,Unity::uint BufferSize)
{
	auto pInstance = PopFace::GetInstance( Instance );
	if ( !pInstance )
		return -1;

	try
	{
		std::stringstream Data;
		pInstance->PopData( Data );
		
		Soy::StringToBuffer( Data.str(), Buffer, BufferSize );
		
		return Data.str().length();
	}
	catch(std::exception& e)
	{
		std::Debug << __func__ << " failed: " << e.what() << std::endl;
		return -1;
	}
}


std::shared_ptr<PopFace::TInstance> PopFace::Alloc(TFaceDetectorParams Params,std::shared_ptr<Opengl::TContext> OpenglContext)
{
	gInstancesLock.lock();
	static TInstanceRef gInstanceRefCounter(1000);
	auto InstanceRef = gInstanceRefCounter++;
	gInstancesLock.unlock();

	if ( !OpenglContext )
		OpenglContext = Unity::GetOpenglContextPtr();
	
	std::shared_ptr<TInstance> pInstance( new TInstance(InstanceRef,Params,OpenglContext) );
	
	gInstancesLock.lock();
	gInstances.push_back( pInstance );
	gInstancesLock.unlock();

	return pInstance;
}

std::shared_ptr<PopFace::TInstance> PopFace::GetInstance(TInstanceRef Instance)
{
	for ( int i=0;	i<gInstances.size();	i++ )
	{
		auto& pInstance = gInstances[i];
		if ( pInstance->GetRef() == Instance )
			return pInstance;
	}
	return std::shared_ptr<PopFace::TInstance>();
}

bool PopFace::Free(TInstanceRef Instance)
{
	gInstancesLock.lock();
	for ( int i=0;	i<gInstances.size();	i++ )
	{
		auto& pInstance = gInstances[i];
		if ( pInstance->GetRef() != Instance )
			continue;
		
		if ( pInstance )
		{
			pInstance.reset();
		}
		gInstances.erase( gInstances.begin()+ i );
		gInstancesLock.unlock();
		return true;
	}
	gInstancesLock.unlock();
	return false;
}



PopFace::TInstance::TInstance(const TInstanceRef& Ref,TFaceDetectorParams Params,std::shared_ptr<Opengl::TContext> OpenglContext) :
	mRef			( Ref ),
	mOpenglContext	( OpenglContext )
{
#if defined(TARGET_OSX) || defined(TARGET_IOS)
	mDetector.reset( new CoreImageFaceDetector( Params, OpenglContext ) );
#else
	throw Soy::AssertException("unsupported on this platform");
#endif
}

void PopFace::TInstance::PushTexture(Opengl::TTexture Texture)
{
	auto Detector = mDetector;
	Soy::Assert( Detector != nullptr, "No detector");
	
	Detector->PushTexture( Texture );
}


void PopFace::TInstance::PopData(std::stringstream& Data)
{
	auto Detector = mDetector;
	Soy::Assert( Detector != nullptr, "No detector");
	
	Detector->PopData( Data );
}


