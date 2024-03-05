package cafe.serenity.gl_es_ndk_practice

import android.graphics.Color
import android.graphics.PointF
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.core.graphics.toColor
import cafe.serenity.gl_es_ndk_practice.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    private val triangleDescriptor by lazy {
        MonochromaticTriangleGLDescriptor(
            loadShader(MonochromaticTriangleGLDescriptor.vShaderAssetName),
            loadShader(MonochromaticTriangleGLDescriptor.fShaderAssetName),
            floatArrayOf(
                -.5f, -.5f,
                .0f, .7f,
                .5f, -.5f,
            ),
            Color.DKGRAY.toColor()
        )
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.surfaceView.setRenderer(NDKRenderer(triangleDescriptor))

        binding.angleTxt.text = getString(R.string.degrees_txt, binding.slider.value)

        binding.slider.addOnChangeListener{ _, value, _ ->
            triangleDescriptor.setRotation(value)
            binding.angleTxt.text = getString(R.string.degrees_txt, value)
        }
    }
    private val shaderMap = mapOf<String, String>()
    private fun loadShader(assetName: String): String {
        return if(shaderMap.containsKey(assetName)) {
            shaderMap[assetName]!!
        } else {
            val stream = assets.open("triangle_vertex_shader.glsl")
            val size = stream.available()
            val buffer = ByteArray(size)
            stream.read(buffer)
            stream.close()
            String(buffer)
        }
    }
}