package cafe.serenity.gl_es_ndk_practice

object GLES3JNILib {
    external fun init(vShaderCode: String, fShaderCode: String) // WIth shaders ???
    external fun resize(width: Int, height: Int)
    external fun updateModel(points: FloatArray, rotationDegrees: Float, color: Int)
    external fun draw(vShaderCode: String, fShaderCode: String)

    init {
        System.loadLibrary("gl_es_ndk_practice")
    }
}