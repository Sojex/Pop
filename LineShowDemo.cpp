#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <iostream>

#include <fstream>
#include <string>
#include <sstream>
#include <cassert>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include <math.h>



static unsigned int CompileShader(unsigned int type, const std::string& source)
{

    //creates a shader and you define what type it is id because thats the id for the shader
    unsigned int id = glCreateShader(type);


    const char* src = source.c_str();

    //specifies source of the shader            id, how many source codes, memory address of source, length if u dont want full string
    glShaderSource(id, 1, &src, nullptr);

    glCompileShader(id);
    int result;

    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
   
    if (result == GL_FALSE)
    {
        int length;

        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*)alloca(length * sizeof(char));

       glGetShaderInfoLog(id, length, &length, message);

        std::cout << "FAILED TO COMPILE " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "SHADER" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }


    return id;

}



static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    //defines a shader program
    unsigned int program = glCreateProgram();
    
    //makes the shader with the definition you gave
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);

    
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    //links both shaders and compiles them into one program
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    //links the program and validates them
    glLinkProgram(program);
    glValidateProgram(program);

    //deletes shaders because they have already beein linked to a program
    glDeleteShader(vs);
    glDeleteShader(fs);





    return program;

};

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "x^2 Line Example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "ERROR";

    /*
    float positions[12] =
    {
        -0.5f,-0.5f, 0.0f,  //0
         0.5f,-0.5f, 0.0f,  //1
         0.5f, 0.5f, 0.0f,    //2
        -0.5f, 0.5f, 0.0f    //3
        
    };
    */

    float y1 = 0.5f;
    float delta = 0.1;
    float initialX = -0.95f;
    float yAbsoluteMax;
    float yCoord[20]
    {
        0.0f,0.0f,0.0f,0.0f,0.0f,
        0.0f,0.0f,0.0f,0.0f,0.0f,
        0.0f,0.0f,0.0f,0.0f,0.0f,
        0.0f,0.0f,0.0f,0.0f,0.0f,

    };

    float xCoord[20]
    {
        0.0f,0.0f,0.0f,0.0f,0.0f,
        0.0f,0.0f,0.0f,0.0f,0.0f,
        0.0f,0.0f,0.0f,0.0f,0.0f,
        0.0f,0.0f,0.0f,0.0f,0.0f,

    };
    //sets xCords based on desired distance between points
    for (int i = 0; i < 20; i++)
    {
        xCoord[i] = xCoord[i] + initialX + (i*delta);
       // std::cout << xCoord[i] << std::endl;
    }
   
  
    //sets xCords based on desired distance between points      x^2 function
    for (int i = 0; i < 20; i++)
    {
        yCoord[i] = pow(xCoord[i], 2);
        std::cout << "x: " << xCoord[i] << " " << "y: " << yCoord[i] << std::endl;
    }

    float example = pow(0.5f, 2.0f);
    std::cout << example;

   /* float positions[12] =
    {
        -0.5f,-0.5f, 0.0f,  //0
         0.5f,-y1, 0.0f,  //1
         0.5f, 0.5f, 0.0f,    //2
        -0.5f, 0.5f, 0.0f    //3

    };
    */
    float positions[]
    {
          xCoord[0], yCoord[0], 0,          //1
          xCoord[1], yCoord[1], 0,          //2
          xCoord[2], yCoord[2], 0,          //3
          xCoord[3], yCoord[3], 0,          //4
          xCoord[4], yCoord[4], 0,          //5
          xCoord[5], yCoord[5], 0,          //6
          xCoord[6], yCoord[6], 0,          //7
          xCoord[7], yCoord[7], 0,          //8
          xCoord[8], yCoord[8], 0,          //9
          xCoord[9], yCoord[9], 0,          //10
        xCoord[10], yCoord[10], 0,          //11
        xCoord[11], yCoord[11], 0,          //12
        xCoord[12], yCoord[12], 0,          //13
        xCoord[13], yCoord[13], 0,          //14
        xCoord[14], yCoord[14], 0,          //15
        xCoord[15], yCoord[15], 0,          //16
        xCoord[16], yCoord[16], 0,          //17
        xCoord[17], yCoord[17], 0,          //18
        xCoord[18], yCoord[18], 0,          //19
        xCoord[19], yCoord[19], 0           //20


    };


    unsigned int indices[]
    {
        0,1,
        1,2,
        2,3,
        3,4,
        4,5,
        5,6,
        6,7,
        7,8,
        8,9,
        9,10,
        10,11,
        11,12,
        12,13,
        13,14,
        14,15,
        15,16,
        16,17,
        17,18,
        18,19,
        19,20
    };

    unsigned int buffer;
    /*Defines the  Vertex Buffer   tells how many buffers you want at once*/
    glGenBuffers(1, &buffer);

    /*Selects the Buffer*/
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    /*Defines How Big the Buffer Is*/
                    //if buffer  , size of data in bytes,  the array, //expolain 6*3*sizeof float           
    glBufferData(GL_ARRAY_BUFFER, 20 * 3*sizeof(float), positions, GL_DYNAMIC_DRAW);


    //enables the attribute
    glEnableVertexAttribArray(0);

    //vertex attrib array pointer tells how the data is layed out
    //musst bind each attribute here we are binding postitions for example
                        //which attrib pointer, component per attribute count, type of data, whether or not you want them normalized, stride amount of bytes between each vertex, how far to get to next attribute 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);



    unsigned int ibo;
    /*Defines the  Index Buffer   tells how many buffers you want at once*/
    glGenBuffers(1, &ibo);

    /*Selects the Buffer*/
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    /*Defines How Big the Buffer Is*/
                    //if buffer  , size of data in bytes,  the array,            
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 38 * sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);


    //enables the attribute
    glEnableVertexAttribArray(0);


   unsigned int a;

    glGenBuffers(1,&a);


    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "	gl_Position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "	color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";



    //creats the shader program
    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    //makes this shader program the active one
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

 

                                                                            //mode, 
      //when using vertex array instead of index arrays to draw  glDrawArrays(GL_LINE_STRIP, 0, 3);
                            //mode, number of indices drawing, type , poiter to the index buffer
        glDrawElements(GL_LINE_STRIP, 38, GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}