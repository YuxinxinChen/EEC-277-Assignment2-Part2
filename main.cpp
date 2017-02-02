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
    float x,y,z,w;

} vertices[68] =
{
        0.0f, 0.0f, 0.0f, 1.0f , //0

        -0.2f, 0.8f, 0.0f, 1.0f, //1
        0.2f, 0.8f, 0.0f, 1.0f, //2
        0.0f, 0.8f, 0.0f, 1.0f, //3
        0.0f, 1.0f, 0.0f, 1.0f, //4

        -0.2f, -0.8f, 0.0f, 1.0f, //5
        0.2f, -0.8f, 0.0f, 1.0f, //6
        0.0f, -0.8f, 0.0f, 1.0f, //7
        0.0f, -1.0f, 0.0f, 1.0f, //8

        -0.8f, -0.2f, 0.0f, 1.0f, //9
        -0.8f, 0.2f, 0.0f, 1.0f, //10
        -0.8f, 0.0f, 0.0f, 1.0f, //11
        -1.0f, 0.0f, 0.0f, 1.0f, //12

        0.8f, -0.2f, 0.0f, 1.0f, //13
        0.8f, 0.2f, 0.0f, 1.0f, //14
        0.8f, 0.0f, 0.0f, 1.0f, //15
        1.0f, 0.0f, 0.0f, 1.0f //16

};

static const struct
{
    float r,g,b,alpha;
} colors[68] =
        {
                1.0f, 1.0f, 1.0f, 1.0f,

                0.0f, 1.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f, 1.0f,
                0.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 0.0f, 0.0f, 1.0f,

                0.0f, 0.0f, 1.0f, 1.0f,
                0.0f, 1.0f, 0.0f, 1.0f,
                0.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 0.0f, 0.0f, 1.0f,

                0.0f, 1.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f, 1.0f,
                0.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 0.0f, 0.0f, 1.0f,

                0.0f, 0.0f, 1.0f, 1.0f,
                0.0f, 1.0f, 0.0f, 1.0f,
                0.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 0.0f, 0.0f, 1.0f
        };

GLuint Indices[48] = {
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
"attribute vec4 vCol;\n"
"attribute vec4 vPos;\n"
"varying vec4 color;\n"
"void main()\n"
"{\n"
"    gl_Position = vPos;\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"varying vec4 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = color;\n"
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
    GLuint vertex_buffer, color_buffer, vertex_shader, fragment_shader, program, index_buffer;
    GLint vpos_location, vcol_location;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(800, 800, "Simple example", NULL, NULL);
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

    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");
    
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 0, (void*) 0);

    glGenBuffers(1, &color_buffer );
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 0, (void*) (sizeof(float) * 0));

    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);


    int counter=0;
    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        double startTime, endTime, elapsedTimeSeconds;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(program);
        startTime = endTime = glfwGetTime();
        glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, 0);
        endTime = glfwGetTime();
        elapsedTimeSeconds = endTime - startTime;
        printf("time:%f\n",elapsedTimeSeconds);
        glfwSwapBuffers(window);
        glfwPollEvents();
        counter++;
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

//! [code]
