//
//  main.cpp
//  opengl
//  OpenGL rainbow triangle
//
//  Created by Albert Ye on 6/11/23.
//

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float speed = 1;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n" // output a color to the fragment shader
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0);\n"
    "}\0";


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

float vertices[] = {
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};

int main(int argc, const char * argv[]) {
    GLFWwindow* window;
    if (!glfwInit()) {
        return -1;
    }
    
    // version settings
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // create window
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Rainbow Triangle", NULL, NULL);
    if (!window) {
        cout << "failed to create window!" << endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD!" << endl;
        return -1;
    }
    
    // generate and process vertex buffer object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // create vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    
    // vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "ERROR: vertex shader couldn't compile!\n" << infoLog << endl;
        return -1;
    }
    cout << "Vertex shader compiled" << endl;
    
    // fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "ERROR: fragment shader couldn't compile!\n" << infoLog << endl;
        return -1;
    }
    cout << "Fragment shader compiled" << endl;
    
    // shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    // attach shaders to program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "ERROR: couldn't link shaders!\n" << infoLog << endl;
        return -1;
    }
    // do not need shaders anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // set color attribute pointers
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // window actions while open
    while (!glfwWindowShouldClose(window)) {
        // input
        float timeValue = glfwGetTime();
        processInput(window);
        // rendering
        float bright = sin(speed * timeValue) / 4.0f + 0.25f;
        glClearColor(bright, bright, bright, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // activate the program
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // check and call events, swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and height will b significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (speed < 3) speed += 0.01;
        cout << "speed: " << speed << endl;
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (speed > 0) speed -= 0.01;
        cout << "speed: " << speed << endl;
    }
}
