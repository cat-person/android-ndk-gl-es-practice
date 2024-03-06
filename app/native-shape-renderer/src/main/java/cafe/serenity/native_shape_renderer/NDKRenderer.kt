package cafe.serenity.native_shape_renderer
import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class NDKRenderer(private var descriptor: MonochromaticTriangleGLDescriptor) :
    GLSurfaceView.Renderer {
    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        RendererJNI.init(descriptor.vShaderCode, descriptor.fShaderCode)
        updateShape(descriptor)
    }

    fun updateShape(newDescriptor: MonochromaticTriangleGLDescriptor) {
        descriptor = newDescriptor
        RendererJNI.updateModel(descriptor.vCoordinates, descriptor.angleDegrees, descriptor.color)
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        RendererJNI.resize(width, height)
    }

    override fun onDrawFrame(gl: GL10?) {
        RendererJNI.draw()
    }
}

class MonochromaticTriangleGLDescriptor(
    val vShaderCode: String,
    val fShaderCode: String,
    val vCoordinates: FloatArray,
    val color: Int
) {
    val angleDegrees: Float
        get() = mutAngleDegrees

    private var mutAngleDegrees = 0f

    fun setRotation(degrees: Float) {
        mutAngleDegrees = degrees
    }

    companion object {
        const val vShaderAssetName = "triangle_vertex_shader.glsl"
        const val fShaderAssetName = "triangle_fragment_shader.glsl"
    }
}

object RendererJNI {
    external fun init(vShaderCode: String, fShaderCode: String) // WIth shaders ???
    external fun resize(width: Int, height: Int)
    external fun updateModel(points: FloatArray, rotationDegrees: Float, color: Int)
    external fun draw()

    init {
        System.loadLibrary("native_shape_renderer")
    }
}


