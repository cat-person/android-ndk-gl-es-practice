//
// Created by Aleksei Klyshnniov on 07.03.2024.
//
#include <android/log.h>

#define LOG_TAG "NATIVE_RENDERER"

bool checkGlError(const char *funcName) {
    GLint err = glGetError();
    if (err != GL_NO_ERROR) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "GL error after %s(): 0x%08x\n", funcName,
                            err);
        return true;
    }
    return false;
}

GLuint compileShader(GLenum shaderType, const char *src) { // rename to compile shader
    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
        checkGlError("glCreateShader");
        return 0;
    }
    glShaderSource(shader, 1, &src, NULL);

    GLint compiled = GL_FALSE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen > 0) {
            GLchar *infoLog = (GLchar *) malloc(infoLogLen);
            if (infoLog) {
                glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
                __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,
                                    "Could not compile %s shader:\n%s\n",
                                    shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment",
                                    infoLog);
                free(infoLog);
            }
        }
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint createProgram(const char *vShaderCode, const char *fShaderCode) {
    GLuint vShaderHandler = 0;
    GLuint fShaderHandler = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;

    vShaderHandler = compileShader(GL_VERTEX_SHADER, vShaderCode);

    if (!vShaderHandler) goto exit;

    fShaderHandler = compileShader(GL_FRAGMENT_SHADER, fShaderCode);
    if (!fShaderHandler) goto exit;

    program = glCreateProgram();
    if (!program) {
        checkGlError("glCreateProgram");
        goto exit;
    }
    glAttachShader(program, vShaderHandler);
    glAttachShader(program, fShaderHandler);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Could not link program");
        GLint infoLogLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen) {
            GLchar *infoLog = (GLchar *) malloc(infoLogLen);
            if (infoLog) {
                glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
                __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Could not link program:\n%s\n",
                                    infoLog);
                free(infoLog);
            }
        }
        glDeleteProgram(program);
        program = 0;
    }

    exit:
    glDeleteShader(vShaderHandler);
    glDeleteShader(fShaderHandler);
    return program;
}
