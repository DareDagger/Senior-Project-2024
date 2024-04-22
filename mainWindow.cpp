#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

// Constants
const int windowHeight = 600;
const int numButtons = 6;
const int buttonHeightPixels = 80;
const int buttonWidthPixels = 100;
const int verticalSpacingPixels = 20;

/*
    If I ever want to increase/decrease the number of buttons I would need to
    change the numButtons to the number of buttons I want, either increase or
    decrease buttonHeightPixels, and either increase or decrease the
    verticalSpacingPixels.

    For example, for 10 buttons:
    // Constants
    const int windowHeight = 700;
    const int numButtons = 10;
    const int buttonHeightPixels = 50;
    const int buttonWidthPixels = 100;
    const int verticalSpacingPixels = 15;
*/

// Vertex Shader
const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
out vec3 ourColor;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
    ourColor = aColor;
}
)glsl";

// Fragment Shader
const char* fragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;
in vec3 ourColor;
void main() {
    FragColor = vec4(ourColor, 1.0);
}
)glsl";

// Shader compilation and linking
GLuint compileShader(const char* shaderCode, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}

GLuint createShaderProgram(const char* vertexShaderCode, const char* fragmentShaderCode) {
    GLuint vertexShader = compileShader(vertexShaderCode, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentShaderCode, GL_FRAGMENT_SHADER);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

void launchDemo(int demoIndex) {
    const char* demoCommand = ""; // Command to run the demo

    switch (demoIndex) {
        case 0:
            demoCommand = "./cubeDemo";
            break;
        case 1:
            demoCommand = "./triPyramid";
            break;
        case 2:
            demoCommand = "./sphereDemo";
            break;
        case 3:
            demoCommand = "./diamondDemo";
            break;
        case 4:
            demoCommand = "./advCube";
            break;
        default:
            std::cerr << "Unknown demo index: " << demoIndex << std::endl;
            return;
    }

    if (std::system(demoCommand) != 0) {
        std::cerr << "Failed to launch demo: " << demoCommand << std::endl;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Convert pixel coordinates to normalized device coordinates (NDC)
        float ndcX = (float)xpos / 400.0f - 1.0f; // Assuming window width is 800
        float ndcY = 1.0f - (float)ypos / (windowHeight/2.0); // Assuming window height is 600, use when configuring: (windowHeight/2.0)

        float buttonHeight = 2.0f * buttonHeightPixels / windowHeight;
        float buttonWidth = 2.0f * buttonWidthPixels / 800;  // Use window width for normalization
        float verticalSpacing = 2.0f * verticalSpacingPixels / windowHeight;

        float yOffset = 1.0f;

        for (int i = 0; i < numButtons; i++) {
            float top = yOffset;
            float bottom = yOffset - buttonHeight;
            float left = -1.0f;
            float right = -1.0f + buttonWidth;

            if (ndcX >= left && ndcX <= right && ndcY <= top && ndcY >= bottom) {
                if (i == numButtons - 1) { // Exit button clicked
                    glfwSetWindowShouldClose(window, true);
                } else {
                    // Launch or terminate Mesa demo based on button
                    launchDemo(i);
                    //std::cout << "Button " << (i +1) << " clicked. Implement demo launch/termination here.\n";
                }
            }
            yOffset -= (buttonHeight + verticalSpacing);
        }
    }
}


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, windowHeight, "Buttons Window", NULL, NULL);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    GLuint shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
    glUseProgram(shaderProgram);

    // Convert pixel dimensions to normalized device coordinates (NDC)
    float buttonHeight = 2.0f * buttonHeightPixels / windowHeight;
    float buttonWidth = 2.0f * buttonWidthPixels / 800;  // Use window width for normalization
    float verticalSpacing = 2.0f * verticalSpacingPixels / windowHeight;

    std::vector<float> vertices;
    //float yOffset = 1.0f - buttonHeight; This line will offset the top of the window by one whole button space
    float yOffset = 1.0f;

    for (int i = 0; i < numButtons; ++i) {
        std::vector<float> buttonVertices = {
            // positions       // colors
            -1.0f, yOffset,              1.0f, 0.647f, 0.0f,
            -1.0f, yOffset - buttonHeight, 1.0f, 0.647f, 0.0f,
            -1.0f + buttonWidth, yOffset - buttonHeight, 1.0f, 0.647f, 0.0f,

            -1.0f, yOffset,              1.0f, 0.647f, 0.0f,
            -1.0f + buttonWidth, yOffset - buttonHeight, 1.0f, 0.647f, 0.0f,
            -1.0f + buttonWidth, yOffset, 1.0f, 0.647f, 0.0f
        };
        vertices.insert(vertices.end(), buttonVertices.begin(), buttonVertices.end());
        yOffset -= (buttonHeight + verticalSpacing);
    }

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.678f, 0.847f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, numButtons * 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}
