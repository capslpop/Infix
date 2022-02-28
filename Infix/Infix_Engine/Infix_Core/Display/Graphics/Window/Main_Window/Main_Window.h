#pragma once

//************************************************
// Main Window - main window creation
//************************************************

class MainWindow
{
public:
	MainWindow();
	~MainWindow();

    static void frameBufferResizedCallback(GLFWwindow* window, int width, int height);

	GLFWwindow* window;
    
    static bool frameBufferResized;
private:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
};

bool MainWindow::frameBufferResized = false;

MainWindow::MainWindow()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Infix Engine", nullptr, nullptr);

    // set the window icon: 
    GLFWimage images[1];
    images[0].pixels = stbi_load("Resources/infix_logo_400.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
    glfwSetWindowIcon(window, 1, images);
    stbi_image_free(images[0].pixels);

    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, frameBufferResizedCallback);
}

MainWindow::~MainWindow()
{
    glfwDestroyWindow(window);

    glfwTerminate();
}