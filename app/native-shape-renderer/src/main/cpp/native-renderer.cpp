//
// Created by Aleksei Klyshnniov on 05.03.2024.
//

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <cstdlib>
#include "shader-utils.cpp"

class ShapeRenderer {
public:
    ShapeRenderer(const char *vShaderCode, const char *fShaderCode);

    virtual ~ShapeRenderer();

    void updateShapeParams(GLfloat *vertexCoordinates, GLfloat angleDegrees, GLuint color);

    void updateViewPort(GLuint width, GLuint height);

    void render();

private:
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
ShapeRenderer::updateShapeParams(GLfloat *vertexCoordinates, GLfloat angleDegrees, GLuint color) {
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

    float alpha = static_cast<float>((color >> 24) & 0xFF) / 255.0f;
    float red = static_cast<float>((color >> 16) & 0xFF) / 255.0f;
    float green = static_cast<float>((color >> 8) & 0xFF) / 255.0f;
    float blue = static_cast<float>((color) & 0xFF) / 255.0f;

    gpuColor = new GLfloat[]{red, green, blue, alpha};
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