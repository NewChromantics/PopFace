using UnityEngine;
using System.Collections;

public class FaceDetector : MonoBehaviour {


	public PopFace		mFaceDetector;
	public Texture		mInput;
	public UnityEngine.UI.Text	mOutput;

	void Update () {

		
		if (mFaceDetector == null) {
			mFaceDetector = new PopFace ();
			mFaceDetector.AddDebugCallback( Debug.Log );
		}
		
		if (mFaceDetector != null) {
			
			if ( mInput != null )
			{
				mFaceDetector.UpdateTexture( mInput );
			}

			if ( mOutput != null )
			{
				string Data = mFaceDetector.PopData();
				if ( Data != null )
					mOutput.text = Data;
			}
		}
	
	}
}
