using UnityEngine;
using System.Collections;					// required for Coroutines
using System.Runtime.InteropServices;		// required for DllImport
using System;								// requred for IntPtr
using System.Text;							//	required for string builder


public class PopFace
{
#if UNITY_STANDALONE_OSX || UNITY_EDITOR_OSX
	private const string PluginName = "PopFaceOsx";
#elif UNITY_ANDROID
	private const string PluginName = "PopFace";
#elif UNITY_IOS
	//private const string PluginName = "PopFaceIos";
	private const string PluginName = "__Internal";
#elif UNITY_STANDALONE_WIN || UNITY_EDITOR_WIN
	private const string PluginName = "PopFace";
#endif

	private ulong	mInstance = 0;
	private static int	mPluginEventId = PopFace_GetPluginEventId();

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	public delegate void DebugLogDelegate(string str);
	private DebugLogDelegate	mDebugLogDelegate = null;
	
	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	public delegate void OpenglCallbackDelegate();

	public void AddDebugCallback(DebugLogDelegate Function)
	{
		if ( mDebugLogDelegate == null ) {
			mDebugLogDelegate = new DebugLogDelegate (Function);
		} else {
			mDebugLogDelegate += Function;
		}
	}
	
	public void RemoveDebugCallback(DebugLogDelegate Function)
	{
		if ( mDebugLogDelegate != null ) {
			mDebugLogDelegate -= Function;
		}
	}
	
	void DebugLog(string Message)
	{
		if ( mDebugLogDelegate != null )
			mDebugLogDelegate (Message);
	}

	public bool IsAllocated()
	{
		return mInstance != 0;
	}
	
	[DllImport (PluginName)]
	private static extern ulong		PopFace_Alloc();
	
	[DllImport (PluginName)]
	private static extern bool		PopFace_Free(ulong Instance);

	[DllImport (PluginName)]
	private static extern int		PopFace_GetPluginEventId();

	[DllImport (PluginName)]
	private static extern bool		FlushDebug([MarshalAs(UnmanagedType.FunctionPtr)]System.IntPtr FunctionPtr);

	[DllImport (PluginName)]
	private static extern bool		PopFace_PushTexture2D(ulong Instance,System.IntPtr TextureId,int Width,int Height,TextureFormat textureFormat);

	[DllImport (PluginName)]
	private static extern bool		PopFace_PushRenderTexture(ulong Instance,System.IntPtr TextureId,int Width,int Height,RenderTextureFormat textureFormat);

	[DllImport (PluginName,CallingConvention=CallingConvention.Cdecl)]
	private static extern int		PopFace_PopData(ulong Instance,StringBuilder Buffer,uint BufferSize);


	public PopFace()
	{
		mInstance = PopFace_Alloc ();

		//	if this fails, capture the flush and throw an exception
		if (mInstance == 0) {
			string AllocError = "";
			FlushDebug (
				(string Error) => {
				AllocError += Error; }
			);
			if ( AllocError.Length == 0 )
				AllocError = "No error detected";
			throw new System.Exception("PopFace failed: " + AllocError);
		}
	}
	
	~PopFace()
	{
		//	gr: don't quite get the destruction order here, but need to remove the [external] delegates in destructor. 
		//	Assuming external delegate has been deleted, and this garbage collection (despite being explicitly called) 
		//	is still deffered until after parent object[monobehaviour] has been destroyed (and external function no longer exists)
		mDebugLogDelegate = null;
		PopFace_Free (mInstance);
		FlushDebug ();
	}
	
	void FlushDebug()
	{
		FlushDebug (mDebugLogDelegate);
	}

	public static void FlushDebug(DebugLogDelegate Callback)
	{
		//	if we have no listeners, do fast flush
		bool HasListeners = (Callback != null) && (Callback.GetInvocationList ().Length > 0);
		if (HasListeners) {
			//	IOS (and aot-only platforms cannot get a function pointer. Find a workaround!
#if UNITY_IOS && !UNITY_EDITOR
			FlushDebug (System.IntPtr.Zero);
#else
			FlushDebug (Marshal.GetFunctionPointerForDelegate (Callback));
#endif
		} else {
			FlushDebug (System.IntPtr.Zero);
		}
	}

	public void UpdateTexture(Texture Target)
	{
		Update ();

		if (Target is RenderTexture) {
			RenderTexture Target_rt = Target as RenderTexture;
			PopFace_PushRenderTexture (mInstance, Target_rt.GetNativeTexturePtr(), Target.width, Target.height, Target_rt.format );
		}
		if (Target is Texture2D) {
			Texture2D Target_2d = Target as Texture2D;
			PopFace_PushTexture2D (mInstance, Target_2d.GetNativeTexturePtr(), Target.width, Target.height, Target_2d.format );
		}
		FlushDebug ();
	}

	private void Update()
	{
		GL.IssuePluginEvent (mPluginEventId);
		FlushDebug();
	}

	static public string GetVersion()
	{
		return "GIT_REVISION";
	}

}

