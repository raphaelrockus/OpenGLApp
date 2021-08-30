// OpenGLCourseApp.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
// Following the course by Ben Cook on Udemy
//
#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Windows Dimensions
const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader;

//Vertex Shader
static const char* vShader = "                                  \n\
#version 330                                                    \n\
layout (location = 0) in vec3 pos;                              \n\
                                                                \n\
void main()                                                     \n\
{                                                               \n\
  gl_Position = vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);     \n\
                                                                \n\
}";

//Fragment Shader
static const char* fShader = "                                  \n\
#version 330                                                     \n\
out vec4 colour;                                                \n\
                                                                \n\
void main()                                                     \n\
{                                                               \n\
  colour = vec4(1.0f, 0.0, 0.0, 1.0);                           \n\
                                                                \n\
}";



void CreateTriangle()
{
    //3 points in three dimensions (x,y,z)
    GLfloat vertices[] = 
    {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //Static Draw - we are not likely to change where the points are
        //Dynamic Draw - opposite
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, 1024, NULL, eLog);
        fprintf(stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(theProgram, theShader);
}


void CompileShaders()
{
    shader = glCreateProgram();

    if (!shader)
    {
        printf("Error creating shader program!\n");
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    //checking the errors and putting it in an error log
    GLint result = 0;
    GLchar errorLog[1024] = { 0 };

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(errorLog), NULL, errorLog);
        printf("Error linking program: %s \n", errorLog);
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(errorLog), NULL, errorLog);
        printf("Error validating program: %s \n", errorLog);
        return;
    }


}


int main()
{
    //Initialise GLFW
    if (!glfwInit())
    {
        printf("GLFW not initialized!");
        glfwTerminate();
        return 1;
    }

    //Setupd GLFW Window Properties
    //OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //So the version is set to 3.3

    //OpenGL Profile
    //core profile equls no backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH,HEIGHT,"Test Window",NULL, NULL);
    if (!mainWindow)
    {
        printf("GLFW window creation failed!");
        glfwTerminate();
        return 2;
    }

    //Get the buffer size information
    //area in the middle of the window that holds the OpenGL data
    //size of the bit that's drawn to
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    //Set context for GLEW to use
    //you could switch between multiples windows
    glfwMakeContextCurrent(mainWindow);

    //Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW not initialized!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 3;
    }

    //Setup Viewport Size
    glViewport(0,0, bufferWidth, bufferHeight);


    //Creation of the triangle
    CreateTriangle();
    CompileShaders();


    //Loop until window closed
    while (!glfwWindowShouldClose(mainWindow))
    {
        //Get + Handle user input events
        glfwPollEvents();

        //Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(shader);
            glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);
        glUseProgram(0);

        //two scene going on a once
        //one you drawing too and one that is on screen
        //now swap the buffers so they can see what we've drawned
        glfwSwapBuffers(mainWindow);


    }


    return 0;
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
