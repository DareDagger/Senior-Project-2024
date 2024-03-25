#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <cstdlib>

struct Button {
    float x, y, width, height;
    bool isClicked(GLFWwindow* window) const {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        bool insideX = mouseX > 0 && mouseX < 160.0;
        bool insideY = mouseY > ((10.0-(y*10.0))*24.0 - 24.0) && mouseY < ((10.0-(y*10.0))*24.0);

        return insideX && insideY && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    }

    void draw() const {
        glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + width, y);
            glVertex2f(x + width, y + height);
            glVertex2f(x, y + height);
        glEnd();
    }
};

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void launchDemo(int demoIndex) {
    const char* demoCommand = ""; // Command to run the demo

    switch (demoIndex) {
        case 0:
            demoCommand = "./cubeDemo";
            break;
        case 1:
            demoCommand = "/home/darren/Mesa/demos/build-dir/src/redbook/alpha3D";
            break;
        case 2:
            demoCommand = "/home/darren/Mesa/demos/build-dir/src/redbook/anti";
            break;
        case 3:
            demoCommand = "/home/darren/Mesa/demos/build-dir/src/redbook/movelight";
            break;
        default:
            std::cerr << "Unknown demo index: " << demoIndex << std::endl;
            return;
    }

    if (std::system(demoCommand) != 0) {
        std::cerr << "Failed to launch demo: " << demoCommand << std::endl;
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Demo Launcher", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    // Define buttons
    std::vector<Button> buttons = {
        {-1, 0.9, 0.5, 0.1}, // Placeholder for demo buttons: Button 1
        {-1, 0.7, 0.5, 0.1}, // Add more as needed; Button 2
        {-1, 0.5, 0.5, 0.1}, // Button 3
        {-1, 0.3, 0.5, 0.1}, // Button 4
        {-1, 0.1, 0.5, 0.1}, // Exit button
    };

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.678f, 0.847f, 0.902f, 1.0f); // #ADD8E6
        glClear(GL_COLOR_BUFFER_BIT);

        // Render buttons
        glColor3f(0.902f, 0.733f, 0.678f); // #E6BBAD
        for (const auto& button : buttons) {
            button.draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Check for button clicks
        for (size_t i = 0; i < buttons.size(); ++i) {
            if (buttons[i].isClicked(window)) {
                if (i == buttons.size() - 1) { // Exit button clicked
                    glfwSetWindowShouldClose(window, true);
                } else {
                    // Launch or terminate Mesa demo based on button
                    launchDemo(i);
                    //std::cout << "Button " << i << " clicked. Implement demo launch/termination here.\n";
                }
            }
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
