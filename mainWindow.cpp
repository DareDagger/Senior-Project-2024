#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>


// While creating the main window I used this website as a basis.
// https://learnopengl.com/code_viewer_gh.php?code=src/7.in_practice/2.text_rendering/text_rendering.cpp

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

// Picture Vertex Shader
const char* picVertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(position, 1.0);
    TexCoords = texCoords;
}
)glsl";

// Picture Fragment Shader
const char* picFragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoords);
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

void setupTexture(const char* imagePath, GLuint& texture) {
    // Flips image vertically to address corrdinate difference
    stbi_set_flip_vertically_on_load(true);
    // Load Image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture" << std::endl;
        return;
    }

    // Determine how many channels the picture has
    GLenum format;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;

    // Create texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}

GLuint setupShaders() {
    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &picVertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &picFragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Link shaders to a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void setupQuadLogo(GLuint& VAO, GLuint& VBO) {
    float vertices[] = {
        // positions         // texture coords
         1.0f,  -0.7f, 0.0f,  1.0f, 1.0f, // top right
         1.0f,  -1.0f, 0.0f,  1.0f, 0.0f, // bottom right
         0.7f,  -1.0f, 0.0f,  0.0f, 0.0f, // bottom left
         0.7f,  -0.7f, 0.0f,  0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    GLuint EBO;
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
}

void setupQuadWelcome(GLuint& VAO, GLuint& VBO) {
    float vertices[] = {
        // positions         // texture coords
          0.5f,  1.0f, 0.0f,  1.0f, 1.0f, // top right
          0.5f,  0.5f, 0.0f,  1.0f, 0.0f, // bottom right
         -0.6f,  0.5f, 0.0f,  0.0f, 0.0f, // bottom left
         -0.6f,  1.0f, 0.0f,  0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    GLuint EBO;
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
}

void setupQuadInstructions(GLuint& VAO, GLuint& VBO) {
    float vertices[] = {
        // positions         // texture coords
         0.7f,  0.3f, 0.0f,  1.0f, 1.0f, // top right
         0.7f,  -0.3f, 0.0f,  1.0f, 0.0f, // bottom right
         -0.5f,  -0.3f, 0.0f,  0.0f, 0.0f, // bottom left
         -0.5f,  0.3f, 0.0f,  0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    GLuint EBO;
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
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

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

    // Picture Setup
    GLuint pictureShaderProgram = setupShaders();
    GLuint texture;
    setupTexture("images/RenderWare_Colored_Logo_Full.png", texture);
    GLuint picVAO, picVBO;
    setupQuadLogo(picVAO, picVBO);

    // Welcome Text Setup
    GLuint welcomeShaderProgram = setupShaders();
    GLuint welcomeTexture;
    setupTexture("images/Ruda_Welcome.png", welcomeTexture);
    GLuint welVAO, welVBO;
    setupQuadWelcome(welVAO, welVBO);

    // Instructions Text Setup
    GLuint instrShaderProgram = setupShaders();
    GLuint instrTexture;
    setupTexture("images/Ruda_Instructions.png", instrTexture);
    GLuint instrVAO, instrVBO;
    setupQuadInstructions(instrVAO, instrVBO);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.678f, 0.847f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw Picture
        glUseProgram(pictureShaderProgram);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(picVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Draw Welcome Mesaage
        glUseProgram(welcomeShaderProgram);
        glBindTexture(GL_TEXTURE_2D, welcomeTexture);
        glBindVertexArray(welVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Draw Instructions message
        glUseProgram(instrShaderProgram);
        glBindTexture(GL_TEXTURE_2D, instrTexture);
        glBindVertexArray(instrVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Draw Rectangles and Buttons
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
