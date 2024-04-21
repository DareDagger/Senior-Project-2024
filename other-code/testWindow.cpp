//This code is trying to put mainWindow.cpp and cubeDemo.cpp together

#include <GLFW/glfw3.h>
#include <iostream>

// Window dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

GLFWwindow* initializeWindow() {
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
#endif

    // Create a GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Demo Window", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    // Check if the OpenGL context was successfully created
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        return nullptr;
    }

    // Set the callback for resizing the window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;
}
void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Add other key interactions here, e.g., camera movement
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // Check for UI button interactions here
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        // Process button clicks based on xpos and ypos
    }

    // Additional mouse interactions can be added here
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    // Handle cursor position for camera controls or other needs
}


int main() {
    GLFWwindow* window = initializeWindow();
    if (!window) return -1;

    // Set input callbacks
    glfwSetKeyCallback(window, processInput);  // Handle keyboard input
    glfwSetMouseButtonCallback(window, mouse_button_callback);  // Handle mouse button input
    glfwSetCursorPosCallback(window, cursor_position_callback);  // Handle mouse movement
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); // Poll and process input/events

        glClear(GL_COLOR_BUFFER_BIT); // Clear the buffers

        // ... drawing code goes here ...

        glfwSwapBuffers(window); // Swap the screen buffers
    }

    glfwTerminate(); // Clean up and close the app properly
    return 0;
}
