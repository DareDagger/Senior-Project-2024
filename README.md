# Senior-Project-2024
Development of the User-Interface and Demos for my Senior Project at the University of Florida.

# ALL DEPENDENCIES
GLFW
GLM
GLAD
freetype

# Compiling In Order
Please compile all demo files before compiling mainWindow.cpp.

# Compile cubeDemo.cpp
g++ -o cubeDemo cubeDemo.cpp glad.c -I. -ldl -lglfw -lGL

# Compile triPyramidDemo.cpp
g++ -o triPyramid triPyramidDemo.cpp glad.c -I. -ldl -lglfw -lGL

# Compile sphereDemo.cpp
g++ -o sphereDemo sphereDemo.cpp glad.c -I. -ldl -lglfw -lGL

# Compile diamondDemo.cpp
g++ -o diamondDemo diamondDemo.cpp glad.c -I. -ldl -lglfw -lGL

# Compile advCubeDemo.cpp
g++ -o advCube advCubeDemo.cpp glad.c -I. -ldl -lglfw -lGL

# Compile mainWindow.cpp
g++ -std=c++11 mainWindow.cpp glad.c -o mainWindow -I./ -ldl -lglfw -lGL -lGLU

# Rendering images
Make sure the stb_image.h file is in your root directory.
The file can be found at this GitHub Repository: https://github.com/nothings/stb/blob/master/stb_image.h