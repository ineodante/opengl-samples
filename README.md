# opengl-samples
OpenGL Samples
<hr>
<h3>Samples and Description</h3>
<ol>
    <li>
      <h4>OpenGL-001-BasicWindow</h4>
      <ul>
        <li>GLFW3 Library is used to create the window and control events</li>
        <li>GLFW3 Library error callback usage</li>
        <li>GLFW3 Library keyboard controls usage</li>
        <li>GLEW library is included</li>
      </ul>
    </li>
    <li>
        <h4>OpenGL-002-BasicTriangleWithShaders</h4>
        <ul>
            <li>Basic Triangle with VAO and VBO</li>
            <li>Vertex Shader Sample for position</li>
            <li>Fragment Shader Sample for Color</li>
        </ul>
    </li>
    <li>
        <h4>OpenGL-003-CameraView</h4>
        <ul>
            <li>Basic Triangle</li>
            <li>Camera View implemented using glm library</li>
            <li>Vertex Shader Uniforms implementation</li>
        </ul>
    </li>
    <li>
        <h4>OpenGL-004-MultiTriangle</li>
        <ul>
            <li>Multiple triangles implemented using multiple VAO and VBO</li>
            <li>Individual VBO containing Position and Color Information</li>
            <li>Camera Implementation using GLM Library Perspective Mode</li>
        </ul>
    </li>
    <li>
        <h4>OpenGL-005-ColoredCube</li>
        <ul>
            <li>Cube Rendered using individual VBO linked with VAO</li>
            <li>One VBO for Vertex Positions and another for Color information</li>
            <li>Z-Buffer implementation using GL_DEPTH_TEST</li>
            <li>GL_LESS to implement near object view and far object clipping</li>
        </ul>
    </li>
    <li>
        <h4>OpenGL-006-TexturedTriangle</h4>
        <ul>
            <li>Multiple Triangles generated with VAO and VBO</li>
            <li>Texture loaded with STB_IMAGE</li>
            <li>Uniforms used to assign texture and mix vertex colors with Texture</li>
            <li>GL_DEPTH_TEST and GL_LESS to implement Z Buffer</li>
        </ul>
    </li>
    <li>
        <h4>OpenGL-007-EBO-CUBE</h4>
        <ul>
            <li>Element Buffer Object used to improve the performance of model loading</li>
        </ul>
    </li>
    <li>
        <h4>OpenGL-008-OBJ-Parser</h4>
        <ul>
            <li>Wavefront OBJ File Format custom parser to load wavefront obj files</li>
            <li>Only Vertices and Indices reading to implement via VBO and EBO </li>
            <li>Sample doesn't contain texture coordinates implementation</li>
        </ul>
    </li>
    <li>
        <h4>OpenGL-009-Camera-Rotation</h4>
        <ul>
            <li>Change of MVP Matrices at runtime to implement circular hover of the camera around the objects</li>
        </ul>
    </li>
    <li>
        <h4>OpenGL-010-FirstPersonCamera</h4>
        <ul>
            <li>Camera implemented in FPS Style with WASD Controls</li>
        </ul>
    </li>
</ol>
