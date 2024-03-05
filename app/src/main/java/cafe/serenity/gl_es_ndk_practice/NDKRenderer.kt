package cafe.serenity.gl_es_ndk_practice

import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class NDKRenderer: GLSurfaceView.Renderer {

    val nativeLib = GLES3JNILib()
    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        nativeLib.init()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        nativeLib.resize(width, height)
    }

    override fun onDrawFrame(gl: GL10?) {
        nativeLib.step()
    }
}

