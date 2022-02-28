:: This file will atomatically be run at build time in CMakeList.txt
:: add to the file to compile more shadrs in this format
:: %VK_SDK_PATH%/Bin/glslangValidator.exe -V Shaders/shader.[type]
%VK_SDK_PATH%/Bin/glslangValidator.exe -V Shaders/shader.vert
%VK_SDK_PATH%/Bin/glslangValidator.exe -V Shaders/shader.frag
pause