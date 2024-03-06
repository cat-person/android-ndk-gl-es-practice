#include <jni.h>
#include <string>
#include <GLES2/gl2.h>
#include "renderer.cpp"

ShapeRenderer *renderer;

extern "C"
JNIEXPORT void JNICALL Java_cafe_serenity_gl_1es_1ndk_1practice_GLES3JNILib_init(JNIEnv* env, jobject obj, jstring givenVShaderCode, jstring givenFShaderCode) {
    const char *vShaderCode = env->GetStringUTFChars(givenVShaderCode, 0);
    const char *fShaderCode = env->GetStringUTFChars(givenFShaderCode, 0);

    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "vShader:\n%s\n", vShaderCode);
    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "fShader:\n%s\n", fShaderCode);

    renderer = new ShapeRenderer(vShaderCode, fShaderCode);
}

extern "C"
JNIEXPORT void JNICALL Java_cafe_serenity_gl_1es_1ndk_1practice_GLES3JNILib_resize( JNIEnv* env, jobject obj, jint width, jint height) {
    renderer->updateViewPort(width, height);
}

extern "C"
JNIEXPORT void JNICALL Java_cafe_serenity_gl_1es_1ndk_1practice_GLES3JNILib_updateModel(
        JNIEnv* env,
        jobject obj,
        jfloatArray points,
        jfloat rotationDegrees,
        jint color) {

    GLfloat* coordinates = env->GetFloatArrayElements(points, 0);
    renderer->updateShapeParams(coordinates, rotationDegrees, color);
}

extern "C"
JNIEXPORT void JNICALL Java_cafe_serenity_gl_1es_1ndk_1practice_GLES3JNILib_draw(JNIEnv* env, jobject obj, jstring vShaderCode, jstring fShaderCode) {
    renderer->render();
}



// create renderer