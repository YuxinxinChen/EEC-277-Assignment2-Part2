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
#include <string.h>
typedef struct
{
    GLfloat x,y;
} Vertex2D;

typedef struct
{
    GLfloat x,y,z;
} Vertex3D;

typedef struct
{
    GLfloat r,g,b;
} Color3D;

//static const struct
//{
//    float x, y;
////    float r, g, b;
//}
Vertex2D vertices[] =
{
//    { -0.6f, 0.4f },
//    {  0.6f, 0.4f },
//    {   -0.6f,  -0.4f },

 //   {   -0.6f,  -0.4f },
 //   {  0.6f, 0.4f },
//    { 0.6f, -0.4f}
                {-1, 0},
                {-0.6666, 0.745356},
                {-0.666667, -0.745356},
                {-0.333333, 0.942809},
                {-0.333333, -0.942809},
                {0,1},
                {0,-1},
                {0.333333, 0.942809},
                {0.333333, -0.942809},
                {0.666667,0.745356},
                {0.666667,-0.745356},
                {1,0},
                {0,0}
};

//static const struct
//{
//    float r, g, b;
//}
Color3D colors[] =
        {
//                {1.f, 0.f, 0.f },
//                {0.f, 1.f, 0.f },
//                {0.f, 0.f, 1.f},

//                {0.f, 0.f, 1.f},
//                {0.f, 1.f, 0.f },
//                {0.f, 0.f, 1.f}
//                {0,0,1},
                {0,0,1},
                {0.16, 0.16, 1},
                {0.16, 0.16, 1},
                {0.33, 0.33, 1},
                {0.33, 0.33, 1},
                {0.5, 0.5, 1},
                {0.5, 0.5, 1},
                {0.66, 0.66, 1},
                {0.66, 0.66, 1},
                {0.83, 0.83, 1},
                {0.83, 0.83, 1},
                {1,1,1},
                {0.5, 0.5, 1}
        };

GLuint Indicess[] = {
        // Top
//        0, 1, 2,
//        2, 1, 3
        0,1,12,
        1,3,12,
        3,5,12,
        5,7,12,
        7,9,12,
        9,11,12,
        11,10,12,
        10,8,12,
        8,6,12,
        6,4,12,
        4,2,12,
        2,0,12
};
static const char* vertex_shader_text =
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(vPos, 0.0, 1.0);\n"
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


void normalizeNormal(Vertex3D *n)
{
    double d = n->x*n->x + n->y*n->y + n->z*n->z;
    d = sqrt(d);
    if (d != 0.0)
        d = 1.0/d;
    n->x *= d;
    n->y *= d;
    n->z *= d;
}


void
buildBaseArray(int addN,  int *totalVertex, Vertex2D **baseVerts,
               Color3D **baseColors, Vertex3D **baseNormals)
{
    // all vertexes are within the range of -1 to 1
    // There are 4 default vertexes: (-1,0), (0,1), (1,0), (0,-1)
    if(addN%2!=0)
    {
        printf("addN has to be an even number\n");
        return;
    }

    float x;
    float r,g, dr, dg;
    Vertex2D *bv;
    Color3D *bc;
    Vertex3D *bn;

    *totalVertex = 5+2*addN;
    double spacing = 2.0/(addN+2.0);
    dr = dg = 1.0/(addN+2);

    // Now we can define the (x,y) for each vertex
    bv = *baseVerts = (Vertex2D*)malloc(sizeof(Vertex2D)*(*totalVertex));
    bc = *baseColors = (Color3D*)malloc(sizeof(Color3D)*(*totalVertex));
    bn = *baseNormals = (Vertex3D*)malloc(sizeof(Vertex3D)*(*totalVertex));

    int indx = 0;
    bv[indx].x = -1;
    bv[indx].y = 0;
    bc[indx].r = 0;
    bc[indx].g = 0;
    bc[indx].b = 1.0f;
    bn[indx].x = -1-1.0;
    bn[indx].y = 0 -1.0;
    bn[indx].z = 1.0f;
    normalizeNormal(bn+indx);
    x = -1.0+spacing;
    r = dr;
    g = dg;
    indx++;

    for(int i=0; i<addN+1;i++, x+=spacing, r+=dr, g+=dg)
    {
        // vertex 1
        bv[indx].x = x;
        bv[indx].y = sqrt( 1.0 - powf(x, 2.0));
        bc[indx].r = r;
        bc[indx].g = g;
        bc[indx].b = 1.0f;
        bn[indx].x = x-1.0;
        bn[indx].y = bv[indx].y-1.0;
        bn[indx].z = 1.0f;
        normalizeNormal(bn+indx);
        indx++;
        // vertex 2
        bv[indx].x = x;
        bv[indx].y = -sqrt(1.0-powf(x, 2.0));
        bc[indx].r = r;
        bc[indx].g = g;
        bc[indx].b = 1.0f;
        bn[indx].x = x-1.0;
        bn[indx].y = bv[indx].y-1.0;
        bn[indx].z = 1.0f;
        normalizeNormal(bn+indx);
        indx++;
    }
    bv[indx].x = 1;
    bv[indx].y = 0;
    bc[indx].r = 1.0f;
    bc[indx].g = 1.0f;
    bc[indx].b = 1.0f;
    bn[indx].x = 1.0-1.0;
    bn[indx].x = 0-1.0;
    bn[indx].z = 1.0f;
    normalizeNormal(bn+indx);
    indx++;
    bv[indx].x = 0;
    bv[indx].y = 0;
    bc[indx].r = 0.5;
    bc[indx].g = 0.5;
    bc[indx].b = 1.0f;
    bn[indx].x = 0-1.0;
    bn[indx].y = 0-1.0;
    bn[indx].z = 1.0f;
    normalizeNormal(bn+indx);
};

void
buildIndexedDisjointTriangleArrays(int addN, int totalVertex, int *totalIndices, GLuint **baseIndices)
{
    // Number of triangle = 4+2*addN
    int totalTriangle = 4+ 2*addN;
    GLuint *bi;
    *totalIndices = 3*(4+2*addN);
    bi = * baseIndices = (GLuint*)malloc(sizeof(GLuint)*(*totalIndices));
    int indx = 0, vindx=1;
    bi[indx]=0;
    indx++;
    bi[indx]=1;
    indx++;
    bi[indx]=totalVertex-1;
    indx++;
    for(int i=0; i<totalTriangle/2-1; i++, vindx+=2)
    {
        bi[indx] = vindx;
        indx++;
        bi[indx] = vindx+2;
        indx++;
        bi[indx] = totalVertex-1;
        indx++;
    }
    bi[indx] = vindx;
    indx++;
    bi[indx] = vindx-1;
    indx++;
    bi[indx] = totalVertex-1;
    indx++;
    vindx = vindx-1;
    for(int i=0; i<totalTriangle/2-1; i++, vindx-=2)
    {
        bi[indx] = vindx;
        indx++;
        bi[indx] = vindx -2;
        indx++;
        bi[indx] = totalVertex-1;
        indx++;
    }
}

int main(void)
{
    GLFWwindow* window;
    GLuint vertex_buffer, color_buffer, indices_buffer, vertex_shader, fragment_shader, program;
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

    Vertex2D * baseVertex;
    Color3D * baseColor;
    Vertex3D *baseNormals;
    GLuint *baseIndices, *bigIndices;
    int nVertex, nIndices, cycleTimes, addN;
    addN = 34000; cycleTimes = 4;
    buildBaseArray(addN, &nVertex, &baseVertex, &baseColor, &baseNormals);
    buildIndexedDisjointTriangleArrays(addN, nVertex, &nIndices, &baseIndices);

    bigIndices = (GLuint *)malloc(sizeof(GLuint)*nIndices*cycleTimes);
    for(int i=0; i<cycleTimes; i++)
    {
        memcpy(bigIndices+i*nIndices, baseIndices, sizeof(GLuint)*nIndices);
    }
//    for(int i=0; i<nVertex; i++)
//        printf("x = %f, y = %f, r = %f, g=%f, b=%f, nx = %f, ny = %f, nz =%f\n", baseVertex[i].x, baseVertex[i].y,
//               baseColor[i].r, baseColor[i].g, baseColor[i].b, baseNormals[i].x, baseNormals[i].y, baseNormals[i].z);
//    for(int i=0; i<nIndices*cycleTimes; i+=3)
//    printf("(x,y,z) = (%d,%d,%d)\n", bigIndices[i], bigIndices[i+1], bigIndices[i+2]);

//    printf("nVertex = %d, nIndices = %d\n", nVertex, nIndices);
//    printf("size of vertices: %d, size of baseVerts = %d\n", (long)sizeof(colors), (long)sizeof(baseColor));

    /* turn on OpenGL lighting stuff */
//    GLfloat lightPosition[] = {(float)640*0.1F, (float)480*0.9F, 200.0F, 1.0F};
    // Create light components
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { -1.5f, 1.0f, -4.0f, 1.0f };
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // Assign created components to GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    /* just use OpenGL's default light values: directional light source */
    glNormalPointer(GL_FLOAT, 0, (const GLvoid *)baseNormals);
    glEnableClientState(GL_NORMAL_ARRAY);

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

//    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D)*nVertex, baseVertex, GL_STATIC_DRAW);


    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 0, (void*) 0);

    glGenBuffers(1, &color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Color3D)*nVertex, baseColor, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 0, (void*) (sizeof(float) * 0));

    glGenBuffers(1, &indices_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indicess), Indicess, GL_STATIC_DRAW);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*nIndices, baseIndices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*nIndices*cycleTimes, bigIndices, GL_STATIC_DRAW);

    float start_time, end_time, eclips_time;
    int totalVertices=0;
    float MV=0;

    start_time = end_time = glfwGetTime();

    while (!glfwWindowShouldClose(window) && (end_time - start_time)<5)
    {
        float ratio;
        int width, height;
        mat4x4 m, p, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

 //       mat4x4_identity(m);
 //       mat4x4_rotate_Z(m, m, (float) glfwGetTime());
 //       mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
 //       mat4x4_mul(mvp, p, m);

        glUseProgram(program);
 //       glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
 //       glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
        end_time = glfwGetTime();
        totalVertices = totalVertices+nVertex*cycleTimes;
    }
    end_time = glfwGetTime();
    eclips_time = end_time-start_time;
    MV = totalVertices/eclips_time;
    printf("addN = %d, MV = %f\n", addN, MV);


    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

//! [code]
