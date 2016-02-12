using UnityEngine;
using System.Collections;

public class CameraCapture : MonoBehaviour {

	public Texture		mTarget;
	public string		mFilename = "device:*";
	public PopMovie		mMovie;

	void Update () {
	
		if (mMovie == null) {
			var Params = new PopMovieParams ();
			mMovie = new PopMovie (mFilename, Params, true);
			mMovie.AddDebugCallback( Debug.Log );
		}

		if (mMovie != null) {

			mMovie.Update ();

			if ( mTarget != null )
			{
				mMovie.UpdateTexture( mTarget, 0 );
			}
		}

	}
}
