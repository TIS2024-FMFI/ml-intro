#include <GLFW/glfw3.h>
#include "openWindow.h"
#include "Perceptron.h"

int main(void)
{
    GLFWwindow* window;

    std::string filePath = "resources/example.html"; // Path to your HTML file
    openWindow(filePath);

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    //Perceptron perceptron(2, 0.1);

    // /*Training data for an AND gate*/
    //std::vector<std::vector<double>> trainingData = {
    //    {0, 0},
    //    {0, 1},
    //    {1, 0},
    //    {1, 1}
    //};
    //std::vector<double> labels = { -1, -1, -1, 1 };

    //perceptron.fit(trainingData, labels, 100);
    //perceptron.printModel();

    // /*Test the perceptron*/
    //std::cout << "Predictions:\n";
    //for (const auto& data : trainingData) {
    //    std::cout << perceptron.guess(data) << std::endl;
    //}

    return 0;
}