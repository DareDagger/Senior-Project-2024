## What is the include Folder
These  are folders that contain header files (.h) which can be included in your source code to use functions or classes defined there.

Instructions for where to find these files and what to do with them.

# Adding GLAD to system
- download glad from https://glad.dav1d.de/
- language C/C++
- Specificaiton OpenGL
- API -> gl -> 3.3
- leave everything else as is
- click "ADD ALL"
- generate loader
- click generate
- download the zip file
- put glad.c in source file directory
- put the two folders (glad, KHR) in /usr/include/
- you will have to use command "sudo cp -r glad /usr/include/" and "sudo cp -r KHR /usr/include/"
- make sure to add the glad folder to your include dependencies for the project
