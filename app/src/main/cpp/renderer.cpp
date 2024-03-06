//
// Created by Aleksei Klyshnniov on 05.03.2024.
//

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <cstdlib>
#include <android/log.h>

#define LOG_TAG "GLES3JNI"

class ShapeRenderer {
public:
    ShapeRenderer(const char *vShaderCode, const char *fShaderCode);

    virtual ~ShapeRenderer();

    void updateShapeParams(GLfloat *vertexCoordinates, GLfloat angleDegrees, GLint color);

    void updateViewPort(GLuint width, GLuint height);

    void render();

private:
    GLuint createProgram(const char *vShaderCode, const char *fShaderCode);

    GLuint createShader(GLenum shaderType, const char *src);

    GLuint program;

    // Attributes
    GLint coordinatesAttributeLocation;
    // Uniforms
    GLint colorUniformLocation;
    GLint rotationMatrixUniformLocation;

    GLfloat *vertexCoordinates;
    GLfloat angleDegrees;
    GLint color;

    GLfloat scaleX = 1.0;
    GLfloat scaleY = 1.0;

    GLfloat *gpuRotationMatrix;
    GLfloat *gpuColor;
};

ShapeRenderer::ShapeRenderer(const char *vShaderCode, const char *fShaderCode) {
    this->program = createProgram(vShaderCode, fShaderCode);
    this->coordinatesAttributeLocation = glGetAttribLocation(program, "vPosition");
    this->rotationMatrixUniformLocation = glGetUniformLocation(program, "rotationMatrix");
    this->colorUniformLocation = glGetUniformLocation(program, "color");
}

ShapeRenderer::~ShapeRenderer() {
    /* The destructor may be called after the context has already been
     * destroyed, in which case our objects have already been destroyed.
     *
     * If the context exists, it must be current. This only happens when we're
     * cleaning up after a failed init().
     */

    glDeleteBuffers(1, reinterpret_cast<const GLuint *>(&coordinatesAttributeLocation));
    glDeleteProgram(program);
}

void
ShapeRenderer::updateShapeParams(GLfloat *vertexCoordinates, GLfloat angleDegrees, GLint color) {
    this->vertexCoordinates = vertexCoordinates;
    this->angleDegrees = angleDegrees;
    this->color = color;
    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "updateShapeParams :: Vertex coordinates %s",
                        vertexCoordinates);

    float angleRadians = (angleDegrees * M_PI) / 180.0;

    gpuRotationMatrix = new GLfloat[]{
            scaleX * cos(angleRadians), scaleY * -sin(angleRadians),
            scaleX * sin(angleRadians), scaleY * cos(angleRadians)
    };
    gpuColor = new GLfloat[]{1.0, 0.0, 0.0, 1.0};
}

void ShapeRenderer::updateViewPort(GLuint width, GLuint height) {
    if (height < width) {
        this->scaleX = static_cast<float>(height) / static_cast<float>(width);
        this->scaleY = 1.0;
    } else {
        this->scaleX = 1.0;
        this->scaleY = static_cast<float>(width) / static_cast<float>(height);
    }
    updateShapeParams(vertexCoordinates, angleDegrees, color);
}

void ShapeRenderer::render() {
    glUseProgram(program);

    glEnableVertexAttribArray(coordinatesAttributeLocation);

    glVertexAttribPointer(coordinatesAttributeLocation, 2, GL_FLOAT, GL_FALSE, 8,
                          vertexCoordinates);

    glUniform4fv(colorUniformLocation, 1, gpuColor);
    glUniformMatrix2fv(rotationMatrixUniformLocation, 1, false, gpuRotationMatrix);

    // Draw the triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Disable vertex array
    glDisableVertexAttribArray(coordinatesAttributeLocation);
}

bool checkGlError(const char *funcName) {
    GLint err = glGetError();
    if (err != GL_NO_ERROR) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "GL error after %s(): 0x%08x\n", funcName,
                            err);
        return true;
    }
    return false;
}

GLuint ShapeRenderer::createShader(GLenum shaderType, const char *src) { // rename to compile shader
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

GLuint ShapeRenderer::createProgram(const char *vtxSrc, const char *fragSrc) {
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLuint resultProgram = 0;
    GLint linked = GL_FALSE;

    vtxShader = createShader(GL_VERTEX_SHADER, vtxSrc);

    if (!vtxShader) goto exit;

    fragShader = createShader(GL_FRAGMENT_SHADER, fragSrc);
    if (!fragShader) goto exit;

    resultProgram = glCreateProgram();
    if (!resultProgram) {
        checkGlError("glCreateProgram");
        goto exit;
    }
    glAttachShader(resultProgram, vtxShader);
    glAttachShader(resultProgram, fragShader);

    glLinkProgram(resultProgram);
    glGetProgramiv(resultProgram, GL_LINK_STATUS, &linked);
    if (!linked) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Could not link program");
        GLint infoLogLen = 0;
        glGetProgramiv(resultProgram, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen) {
            GLchar *infoLog = (GLchar *) malloc(infoLogLen);
            if (infoLog) {
                glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
                __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Could not link program:\n%s\n",
                                    infoLog);
                free(infoLog);
            }
        }
        glDeleteProgram(resultProgram);
        resultProgram = 0;
    }

    exit:
    glDeleteShader(vtxShader);
    glDeleteShader(fragShader);
    return resultProgram;
}