uniform mat2 rotationMatrix;
uniform float scaleX;
uniform float scaleY;

attribute vec2 vPosition;
void main() {
    vec2 rotatedPosition = rotationMatrix * vPosition;
    gl_Position = vec4(rotatedPosition.x, rotatedPosition.y, 0.0, 1.0);
}