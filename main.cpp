//========================================================================
// Simple GLFW example
// Copyright (c) Camilla Berglund <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
//! [code]

#include <GLFW/glfw3.h>

#include "linmath.h"

#include <stdlib.h>
#include <stdio.h>

static const struct
{
    float x, y;
    float r, g, b;
} vertices[17] =
{
        { 0.0f, 0.0f,  1.0f, 1.0f, 1.0f }, // 0

        // Top
        { -0.2f, 0.8f,  0.0f, 1.0f, 0.0f}, // 1
        { 0.2f, 0.8f,   0.0f, 0.0f, 1.0f}, // 2
        { 0.0f, 0.8f,  0.0f, 1.0f, 1.0f}, //3
        { 0.0f, 1.0f,  1.0f, 0.0f, 0.0f},  // 4

        // Bottom
        { -0.2f, -0.8f,   0.0f, 0.0f, 1.0f}, // 5
        { 0.2f, -0.8f,  0.0f, 1.0f, 0.0f}, // 6
        { 0.0f, -0.8f,  0.0f, 1.0f, 1.0f }, //7
        { 0.0f, -1.0f,   1.0f, 0.0f, 0.0f },  // 8

        // Left
        { -0.8f, -0.2f,  0.0f, 1.0f, 0.0f}, // 9
        { -0.8f, 0.2f,  0.0f, 0.0f, 1.0f }, // 10
        { -0.8f, 0.0f,   0.0f, 1.0f, 1.0f }, //11
        { -1.0f, 0.0f,   1.0f, 0.0f, 0.0f },  // 12

        // Right
        { 0.8f, -0.2f,   0.0f, 0.0f, 1.0f }, // 13
        { 0.8f, 0.2f,   0.0f, 1.0f, 0.0f }, //14
        {  0.8f, 0.0f,  0.0f, 1.0f, 1.0f }, //15
        {  1.0f, 0.0f,  1.0f, 0.0f, 0.0f}  // 16
};

GLuint Indices[16] = {
        // Top
        0, 1, 3,
        0, 3, 2,
        3, 1, 4,
        3, 4, 2,

        // Bottom
        0, 5, 7,
        0, 7, 6,
        7, 5, 8,
        7, 8, 6,

        // Left
        0, 9, 11,
        0, 11, 10,
        11, 9, 12,
        11, 12, 10,

        // Right
        0, 13, 15,
        0, 15, 14,
        15, 13, 16,
        15, 16, 14
};


static const char* vertex_shader_text =
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void)
{
    GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program, index_buffer;
    GLint mvp_location, vpos_location, vcol_location;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
//    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(2, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 5, (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 5, (void*) (sizeof(float) * 2));

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        mat4x4 m, p, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        mat4x4_identity(m);
        mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, Indices);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

//! [code]
