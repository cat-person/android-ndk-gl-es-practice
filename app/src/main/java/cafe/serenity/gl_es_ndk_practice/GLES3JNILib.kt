package cafe.serenity.gl_es_ndk_practice

class GLES3JNILib {
    external fun init()
    external fun resize(width: Int, height: Int)
    external fun step()

    companion object {
        // Used to load the 'gl_es_ndk_practice' library on application startup.
        init {
            System.loadLibrary("gl_es_ndk_practice")
        }
    }
}