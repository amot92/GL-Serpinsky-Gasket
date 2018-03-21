// DrawSquare.cpp
// Generate a square on a solid background at user’s mouse position


#include "GL/glew.h"
#include <stdio.h>
#include <stdlib.h>
#include "GLFW/glfw3.h"

GLuint shader_programme;
GLuint vao;
GLfloat points[500];
int w;
int i = 0;
GLfloat size = 20;
int cnt = 0;
GLuint buffer;
//----------------------------------------------------------------------------
void init( void )
{
    /* GL shader programme object [combined, to link] */
    GLuint vert_shader, frag_shader;
    
    // 1. Create and initialize a buffer object (variable is global)
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );
    
    // 2. Create a vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    
    // "attribute #0 is created from every 2 variables in the above buffer, of type float (i.e. make me vec2s)"
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    
    // 3. define shaders
    const char *vertex_shader = "#version 410\n"
    "in vec4 vPosition;"
    "void main () {"
    "    gl_Position = vPosition;"
    "}";
    
    // the fragment shader colours each fragment (pixel-sized area of the triangle)
    const char *fragment_shader = "#version 410\n"
    "out vec4 FragColor;"
    "void main () {"
    "    FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );"
    "}";
    
    //4. create shader objects
    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vertex_shader, NULL);
    glCompileShader(vert_shader);
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &fragment_shader, NULL);
    glCompileShader(frag_shader);
    
    //5. use the resulting shader program
    shader_programme = glCreateProgram();
    glAttachShader(shader_programme, frag_shader);
    glAttachShader(shader_programme, vert_shader);
    glLinkProgram(shader_programme);
    

}

void drawSquare(int x, int y)
{
    //y = w - y; /* invert y position */
    points[i] = (2.0f * (x+size) / w )- 1.0f;
    points[i + 1] = 1.0f - (2.0f * (y+size)) / w;;
    points[i + 2] = (2.0f * (x - size) / w) - 1.0f;
    points[i + 3] = 1.0f - (2.0f * (y + size)) / w;;
    points[i + 4] = (2.0f * (x + size) / w) - 1.0f;
    points[i + 5] = 1.0f - (2.0f * (y - size)) / w;;
    points[i + 6] = (2.0f * (x - size) / w) - 1.0f;
    points[i + 7] = 1.0f - (2.0f * (y + size)) / w;;
    points[i + 8] = (2.0f * (x + size) / w) - 1.0f;
    points[i + 9] = 1.0f - (2.0f * (y - size)) / w;;
    points[i + 10] = (2.0f * (x - size) / w) - 1.0f;
    points[i + 11] = 1.0f - (2.0f * (y - size)) / w;
    i += 12;
    cnt+=6;
}


void mymouse(GLFWwindow* window, int button, int action, int mods){
    if (GLFW_PRESS == action && button == GLFW_MOUSE_BUTTON_LEFT) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);

        drawSquare(x, y);//add new points to points[] array
        //glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
        //glGenVertexArrays(1, &vao);
        
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        // "attribute #0 is created from every 2 variables in the above buffer, of type
        // float (i.e. make me vec2s)"
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    } else if (GLFW_PRESS == action && button == GLFW_MOUSE_BUTTON_RIGHT){
        glfwWindowShouldClose(window);
        glfwTerminate();
    }
}

void mykey(GLFWwindow* window, int key, int scancode, int action, int mods) //define keyboard callback
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
}



//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
    GLFWwindow *window = NULL;
    const GLubyte *renderer;
    const GLubyte *version;
    /* start GL context and O/S window using the GLFW helper library */
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
    /* We must specify 3.2 core if on Apple OS X -- other O/S can specify
     anything here. I defined 'APPLE' in the makefile for OS X
     
     Remove the #ifdef #endif and play around with this - you should be starting
     an explicit version anyway, and some non-Apple drivers will require this too.
     */

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 640, "simple", NULL, NULL);
    w = 640;
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    /* start GLEW extension handler */
    glewExperimental = GL_TRUE;
    glewInit();
    renderer = glGetString(GL_RENDERER); /* get renderer string */
    version = glGetString(GL_VERSION);     /* version as a string */
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    init();
    
    glfwSetMouseButtonCallback(window, mymouse);
    glfwSetKeyCallback(window, mykey);
    
    
    do{
        glClear(GL_COLOR_BUFFER_BIT);     // clear the window
        glUseProgram(shader_programme);
        glBindVertexArray(vao);
        glDrawArrays(GL_LINE_STRIP, 0, cnt );    // draw the points
        /* update other events like input handling */
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    while (!glfwWindowShouldClose(window));
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
    
}
