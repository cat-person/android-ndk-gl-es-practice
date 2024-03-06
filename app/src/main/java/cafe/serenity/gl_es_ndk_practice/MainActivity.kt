package cafe.serenity.gl_es_ndk_practice

import android.graphics.Color
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
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
            Color.LTGRAY
        )
    }

    private val renderer by lazy {
        NDKRenderer(triangleDescriptor)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.surfaceView.setEGLContextClientVersion(2)
        binding.surfaceView.setRenderer(renderer)

        binding.angleTxt.text = getString(R.string.degrees_txt, binding.slider.value)

        binding.slider.addOnChangeListener { _, value, _ ->

            triangleDescriptor.setRotation(value)
            renderer.updateShape(triangleDescriptor)
            binding.angleTxt.text = getString(R.string.degrees_txt, value)
        }
    }

    private val shaderMap = mapOf<String, String>()
    private fun loadShader(assetName: String): String {
        return if (shaderMap.containsKey(assetName)) {
            shaderMap[assetName]!!
        } else {
            val stream = assets.open(assetName)
            val size = stream.available()
            val buffer = ByteArray(size)
            stream.read(buffer)
            stream.close()
            String(buffer)
        }
    }
}