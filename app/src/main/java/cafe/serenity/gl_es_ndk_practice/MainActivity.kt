package cafe.serenity.gl_es_ndk_practice

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import cafe.serenity.gl_es_ndk_practice.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    private val renderer = NDKRenderer()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.surfaceView.setRenderer(renderer)

        binding.angleTxt.text = getString(R.string.degrees_txt, binding.slider.value)

        binding.slider.addOnChangeListener{ _, value, _ ->
            binding.angleTxt.text = getString(R.string.degrees_txt, value)
        }
    }
}