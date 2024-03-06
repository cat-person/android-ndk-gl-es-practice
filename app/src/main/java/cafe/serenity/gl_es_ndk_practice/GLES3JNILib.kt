package cafe.serenity.gl_es_ndk_practice

object RendererJNI {
    external fun init(vShaderCode: String, fShaderCode: String) // WIth shaders ???
    external fun resize(width: Int, height: Int)
    external fun updateModel(points: FloatArray, rotationDegrees: Float, color: Int)
    external fun draw()

    init {
        System.loadLibrary("gl_es_ndk_practice")
    }
}