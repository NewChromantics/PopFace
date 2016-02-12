#include "CoreImageFaceDetector.h"


class CoreImage::TContext
{
public:
	TContext(Opengl::TContext& OpenglContext);
	
	ObjcPtr<CIContext>	mContext;
};


class CoreImage::TDetector
{
public:
	TDetector();
	
	ObjcPtr<CIDetector>	mDetector;
};


CoreImageFaceDetector::CoreImageFaceDetector(TFaceDetectorParams Params,std::shared_ptr<Opengl::TContext> OpenglContext) :
	TFaceDetector	( Params ),
	mOpenglContext	( OpenglContext )
{
	Soy::Assert( OpenglContext!=nullptr, "CoreImageFaceDetector requires opengl context");
	
	//mCoreImageContext.reset( new CoreImage::TContext(*OpenglContext) );
	mCoreImageDetector.reset( new CoreImage::TDetector );
	
}



void CoreImageFaceDetector::PushTexture(Opengl::TTexture& Texture)
{
	/*
	//	make CGImage
	CIImage *image = [CIImage imageWithContentsOfURL:myURL];               // 2
CIFilter *filter = [CIFilter filterWithName:@"CISepiaTone"];           // 3
	[filter setValue:image forKey:kCIInputImageKey];
	[filter setValue:@0.8f forKey:kCIInputIntensityKey];
	CIImage *result = [filter valueForKey:kCIOutputImageKey];              // 4
	CGRect extent = [result extent];
	CGImageRef cgImage = [context createCGImage:result fromRect:extent];   // 5
*/
	Soy::Assert( mCoreImageDetector!=nullptr, "No face detector");

	CGColorSpaceRef cs = CGColorSpaceCreateDeviceRGB();
	auto AutoRelease = Soy::TScopeCall( nullptr, [&]{	CGColorSpaceRelease(cs);	} );
	
	CIImage* Image = [[CIImage alloc]  initWithTexture:Texture.mTexture.mName size:CGSizeMake(Texture.GetWidth(), Texture.GetHeight()) flipped:YES colorSpace:(CGColorSpaceRef)kCGColorSpaceModelRGB];
	Soy::Assert( Image!=nullptr, "Failed to make coreimage image");

	auto* Detector = mCoreImageDetector->mDetector.mObject;
	NSArray* Faces = [Detector featuresInImage:Image options:nil];

	auto FaceCount = Faces ? [Faces count] : -1;
	std::stringstream Result;
	Result << "Found " << FaceCount << " faces.";
	OnData( Result.str() );
}


CoreImage::TDetector::TDetector()
{
	CIDetector* detector = [CIDetector detectorOfType:CIDetectorTypeFace
											  context:nil
											  options:nil];
	Soy::Assert( detector !=nullptr, "Failed to create face detector");
	mDetector.Retain( detector );
}


CoreImage::TContext::TContext(Opengl::TContext& OpenglContext)
{
	/*
	//	gr: does this need to be done on the opengl thread?
	auto OpenglContextObj = CGLGetCurrentContext();

	//CGLPixelFormatObj PixelFormat = CGLGetPixelFormat( Context );
	
	const NSOpenGLPixelFormatAttribute PixelFormatAttribs[] = {
		NSOpenGLPFAAccelerated,
		NSOpenGLPFANoRecovery,		//	need this because if we create an incompatible context then we can't share textures
		NSOpenGLPFAColorSize, 32,
		0
	};
	NSDictionary<NSString *, id>* ContextOptions = nullptr;
	NSOpenGLPixelFormat* PixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:PixelFormatAttribs];
	CGColorSpace* ColourSpace = nullptr;
	auto* PixelFormatCgl = [PixelFormat CGLPixelFormatObj];
	CIContext* Context = [CIContext contextWithCGLContext: OpenglContextObj pixelFormat:PixelFormatCgl
											   colorSpace:ColourSpace
												  options: ContextOptions];
	mContext.Retain( Context );

	/*
	CIImage *image = [CIImage imageWithContentsOfURL:myURL];               // 2
	CIFilter *filter = [CIFilter filterWithName:@"CISepiaTone"];           // 3
	[filter setValue:image forKey:kCIInputImageKey];
	[filter setValue:@0.8f forKey:kCIInputIntensityKey];
	CIImage *result = [filter valueForKey:kCIOutputImageKey];              // 4
	CGRect extent = [result extent];
	CGImageRef cgImage = [context createCGImage:result fromRect:extent];   // 5
	 */
}


