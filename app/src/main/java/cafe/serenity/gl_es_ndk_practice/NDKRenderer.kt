package cafe.serenity.gl_es_ndk_practice

import android.graphics.Color
import android.graphics.PointF
import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10
import kotlin.math.PI
import kotlin.math.cos
import kotlin.math.sin

class NDKRenderer(val descriptor: ShaderDescriptor) : GLSurfaceView.Renderer {
    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        GLES3JNILib.init(descriptor.vShaderCode, descriptor.fShaderCode)
        updateShape(descriptor)
    }

    fun updateShape(descriptor: ShaderDescriptor) {
        GLES3JNILib.updateModel(
            floatArrayOf(
                -.5f, -.5f,
                .0f, .7f,
                .5f, -.5f
            ), descriptor.radians, Color.DKGRAY
        )
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        GLES3JNILib.resize(width, height)
    }

    override fun onDrawFrame(gl: GL10?) {
        GLES3JNILib.draw(descriptor.vShaderCode, descriptor.fShaderCode)
    }
}

class MonochromaticTriangleGLDescriptor(
    override val vShaderCode: String,
    override val fShaderCode: String,
    coords: FloatArray,
    val color: Color
) : ShaderDescriptor {
    override val attributes = arrayOf(Attribute.Coordinates2D(coords) as Attribute)
    override val uniforms
        get() = arrayOf(
            Uniform.RotationMatrixUniform(transformationMatrix) as Uniform,
            Uniform.ColorUniform(color) as Uniform,
        )

    override val radians: Float
        get() = _radians

    private var _radians = 0f

    private var transformationMatrix = floatArrayOf(
        1f, 0f,
        0f, 1f,
    ) // Identity matrix

    fun setRotation(degrees: Float) {
        _radians = degrees

//        transformationMatrix = floatArrayOf(
//            cos(radians), -sin(radians),
//            sin(radians), cos(radians),
//        )
    }

    companion object {
        const val vShaderAssetName = "triangle_vertex_shader.glsl"
        const val fShaderAssetName = "triangle_fragment_shader.glsl"
    }
}

interface ShaderDescriptor {
    val vShaderCode: String
    val fShaderCode: String
    val attributes: Array<Attribute>
    val uniforms: Array<Uniform>
    val radians: Float
}

sealed class Attribute(val attributeName: String, val stride: Int, val size: Int) {
    abstract fun toByteArray(): ByteArray
    class Coordinates2D(val points: FloatArray) : Attribute("position", 8, 24) {
        override fun toByteArray(): ByteArray {
            TODO("Not yet implemented")
        }
    }
}

sealed class Uniform(val uniformName: String) {
    abstract fun toByteArray(): ByteArray
    class ColorUniform(val color: Color) : Uniform("color") {
        override fun toByteArray(): ByteArray {
            TODO("Not yet implemented")
        }
    }

    class RotationMatrixUniform(val rotationMatrixArray: FloatArray) : Uniform("rotation") {
        override fun toByteArray(): ByteArray {
            TODO("Not yet implemented")
        }
    }
}


