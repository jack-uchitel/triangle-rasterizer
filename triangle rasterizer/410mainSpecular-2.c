/* On macOS, compile with...
    clang 410mainSpecular.c /usr/local/gl3w/src/gl3w.o -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -Wno-deprecated
...and you might have to change the location of gl3w.o based on your
installation. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "310vector.c"
#include "310matrix.c"
#include "310shading.c"
#include "330mesh.c"
#include "330mesh2D.c"
#include "330mesh3D.c"
#include "330meshGL.c"
#include "360texture.c"
#include "350isometry.c"
#include "350camera.c"
#include "370node.c"
#include "150landscape.c"

#define LANDSIZE 128



/*** Shaders ***/

#define UNIFVIEWING 0
#define UNIFMODELING 1
#define UNIFTEXTURE0 2
#define UNIFCLIGHT 3
#define UNIFDLIGHT 4
#define ATTRXYZ 0
#define ATTRST 1
#define ATTRNOP 2

shaShading sha;

int initializeShaders(void) {
    GLchar vertexCode[] =
        "#version 140\n"
        "uniform mat4 viewing;"
        "uniform mat4 modeling;"
        "in vec3 xyz;"
        "in vec2 st;"
        "in vec3 nop;"
        "out vec2 texCoord;"
        "out vec3 vary;"
        "void main() {"
        "    gl_Position = viewing * modeling * vec4(xyz, 1.0);"
        "    texCoord = st;"
        "    vec4 worldCoords = modeling * vec4(nop,0.0);"
        "    vary = vec3(worldCoords[0], worldCoords[1], worldCoords[2]);"
        "}";
    // does not work when there are 0s in vary. So where is nop being defined? (worried starting as 0)
    GLchar fragmentCode[] = "\
        #version 140\n\
        uniform sampler2D texture0;\
        uniform vec3 cLight;\
        uniform vec3 dLight;\
        in vec2 texCoord;\
        in vec3 vary;\
        out vec4 fragColor;\
        void main() {\
            vec3 pFrag = vec3(0,0,0) ;\
            vec3 pCam = vec3(0,0,0) ;\
            vec3 cSpec = vec3(1,1,1) ;\
            vec3 cAmb = vec3(cLight[0]/4, cLight[1]/4, cLight[2]/4);\
            vec3 dNormal = normalize(vary);\
            vec3 dRefl = 2*dot(dLight,dNormal)*dNormal-dLight;\
            vec3 cDiff = vec3(texture(texture0, texCoord));\
            if((max(0.0, dot(dNormal, dLight))==0){ ;\
                fragColor = vec4((max(0.0, dot(dNormal, dLight)) *cDiff*cLight) + (cAmb *cDiff ) , 1.0);\
            }else{ ;\
                fragColor = vec4(max(0, dot(pCam,dRefl)) + (max(0.0, dot(dNormal, dLight)) *cDiff*cLight) + (cAmb *cDiff ), 1.0);\
            } ;\
        }";
    //iSpec = max(0, dot(pCam,dRefl))
    //iDiff = (max(0.0, dot(dNormal, dLight))
    const GLchar *unifNames[5] = {"viewing", "modeling", "texture0", "cLight", "dLight"};
    const GLchar *attrNames[3] = {"xyz", "st", "nop" };
    return shaInitialize(&sha, vertexCode, fragmentCode, 5, unifNames, 3,
        attrNames);
}

void destroyShaders(void) {
    shaDestroy(&sha);
}



/*** Lights, camera ***/

camCamera cam;
GLdouble cameraTarget[3] = {LANDSIZE / 2.0, LANDSIZE / 2.0, 0.0};
GLdouble cameraRho = 50.0;
GLdouble cameraPhi = M_PI / 4.0;
GLdouble cameraTheta = -M_PI / 2.0;

int initializeLightsCamera(void) {
    camSetProjectionType(&cam, camPERSPECTIVE);
    camSetFrustum(&cam, M_PI / 6.0, cameraRho, 10.0, 1024, 512);
    camLookAt(&cam, cameraTarget, cameraRho, cameraPhi, cameraTheta);
    return 0;
}

void destroyLightsCamera(void) {
    return;
}



/*** Scene ***/

/* The following variable is the root node of the scene graph. It is declared
here but configured in artwork.c. */
nodeNode root;
#include "390artwork.c"

int initializeScene(void) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    if (initializeShaders() != 0)
        return 1;
    if (initializeLightsCamera() != 0) {
        destroyShaders();
        return 2;
    }
    if (initializeArtwork() != 0) {
        destroyLightsCamera();
        destroyShaders();
        return 3;
    }
    return 0;
}

void destroyScene(void) {
    destroyArtwork();
    destroyLightsCamera();
    destroyShaders();
}



/*** Rendering ***/

void render(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLdouble viewing[4][4];
    GLdouble cLight[3] = {.6, .7, .8};
    GLdouble dLight[3] = {0, 0, 1};
    camGetProjectionInverseIsometry(&cam, viewing);
    shaSetUniform44(viewing, sha.unifLocs[UNIFVIEWING]);
    shaSetUniform3(cLight, sha.unifLocs[UNIFCLIGHT]);
    shaSetUniform3(dLight, sha.unifLocs[UNIFDLIGHT]);
    GLdouble identity[4][4] = {
        {1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}};
    nodeRender(&root, identity, sha.unifLocs[UNIFMODELING], NULL,
        &(sha.unifLocs[UNIFTEXTURE0]));
}



/*** User interface ***/

int screenWidth = 1024;
int screenHeight = 512;

double getTime(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec * 0.000001;
}

void handleTimeStep(GLFWwindow *window, double oldTime, double newTime) {
    if (floor(newTime) - floor(oldTime) >= 1.0)
        printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
    GLdouble translation[3] = {0.0, fmod(newTime, 2.0 * M_PI), 0.0};
    isoSetTranslation(&(root.isometry), translation);
    render();
    glfwSwapBuffers(window);
}

void handleKeyDown(
        int key, int shiftIsDown, int controlIsDown, int altOptionIsDown,
        int superCommandIsDown) {
    if (shiftIsDown) {
        if (key == GLFW_KEY_A)
            cameraTarget[0] -= 0.5;
        else if (key == GLFW_KEY_D)
            cameraTarget[0] += 0.5;
        else if (key == GLFW_KEY_W)
            cameraTarget[1] += 0.5;
        else if (key == GLFW_KEY_S)
            cameraTarget[1] -= 0.5;
        else if (key == GLFW_KEY_E)
            cameraTarget[2] += 0.5;
        else if (key == GLFW_KEY_C)
            cameraTarget[2] -= 0.5;
    } else {
        if (key == GLFW_KEY_A)
            cameraTheta -= M_PI / 32.0;
        else if (key == GLFW_KEY_D)
            cameraTheta += M_PI / 32.0;
        else if (key == GLFW_KEY_W)
            cameraPhi -= M_PI / 32.0;
        else if (key == GLFW_KEY_S)
            cameraPhi += M_PI / 32.0;
        else if (key == GLFW_KEY_E)
            cameraRho *= 9.0 / 10.0;
        else if (key == GLFW_KEY_C)
            cameraRho *= 10.0 / 9.0;
        else if (key == GLFW_KEY_L)
            moveRobot(EAST);
        else if (key == GLFW_KEY_J)
            moveRobot(WEST);
        else if (key == GLFW_KEY_K)
            moveRobot(SOUTH);
        else if (key == GLFW_KEY_I)
            moveRobot(NORTH);
    }
    camLookAt(&cam, cameraTarget, cameraRho, cameraPhi, cameraTheta);
}

void handleKeyRepeat(
        int key, int shiftIsDown, int controlIsDown, int altOptionIsDown,
        int superCommandIsDown) {
    handleKeyDown(key, shiftIsDown, controlIsDown, altOptionIsDown,
        superCommandIsDown);
}

void handleKeyUp(
        int key, int shiftIsDown, int controlIsDown, int altOptionIsDown,
        int superCommandIsDown) {
    return;
}

void handleKey(
        GLFWwindow *window, int key, int scancode, int action, int mods) {
    int shiftIsDown = mods & GLFW_MOD_SHIFT;
    int controlIsDown = mods & GLFW_MOD_CONTROL;
    int altOptionIsDown = mods & GLFW_MOD_ALT;
    int superCommandIsDown = mods & GLFW_MOD_SUPER;
    if (action == GLFW_PRESS)
        handleKeyDown(key, shiftIsDown, controlIsDown, altOptionIsDown,
            superCommandIsDown);
    else if (action == GLFW_REPEAT)
        handleKeyRepeat(key, shiftIsDown, controlIsDown, altOptionIsDown,
            superCommandIsDown);
    else
        /* The action is GLFW_RELEASE. */
        handleKeyUp(key, shiftIsDown, controlIsDown, altOptionIsDown,
            superCommandIsDown);
}


void handleError(int error, const char *description) {
    fprintf(stderr, "handleError: %d\n%s\n", error, description);
}

void handleResize(GLFWwindow *window, int width, int height) {
    screenWidth = width;
    screenHeight = height;
    glViewport(0, 0, screenWidth, screenHeight);
    camSetFrustum(&cam, M_PI / 6.0, cameraRho, 10.0, screenWidth,
        screenHeight);
}

GLFWwindow *initializeWindow(int width, int height, const char *name) {
    glfwSetErrorCallback(handleError);
    if (glfwInit() == 0) {
        fprintf(stderr, "initializeWindow: glfwInit failed.\n");
        return NULL;
    }
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow *window;
    window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "initializeWindow: glfwCreateWindow failed.\n");
        glfwTerminate();
        return NULL;
    }
    glfwSetFramebufferSizeCallback(window, handleResize);
    glfwSetKeyCallback(window, handleKey);
    glfwMakeContextCurrent(window);
    if (gl3wInit() != 0) {
        fprintf(stderr, "initializeWindow: gl3wInit failed.\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return NULL;
    }
    fprintf(stderr, "initializeWindow: using OpenGL %s and GLSL %s.\n",
        glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
    return window;
}

void destroyWindow(GLFWwindow *window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}

int main(void) {
    double oldTime;
    double newTime = getTime();
    GLFWwindow *window = initializeWindow(screenWidth, screenHeight,
        "380mainArtwork");
    if (window == NULL)
        return 1;
    if (initializeScene() != 0) {
        destroyWindow(window);
        return 2;
    }
    while (glfwWindowShouldClose(window) == 0) {
        oldTime = newTime;
        newTime = getTime();
        handleTimeStep(window, oldTime, newTime);
        glfwPollEvents();
    }
    destroyScene();
    destroyWindow(window);
    return 0;
}
