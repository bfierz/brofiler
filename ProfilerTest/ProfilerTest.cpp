#include <iostream>
#include "TestEngine.h"
#include "GLFW/glfw3.h"

using namespace std;

int main(int, char **)
{
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);

	if (!window)
		return -1;
	glfwMakeContextCurrent(window);

	cout << "Starting profiler test." << endl;

	Test::Engine engine;
	cout << "Engine successfully created." << endl;

	cout << "Starting main loop update." << endl;
	while(!glfwWindowShouldClose(window) && engine.Update() )
	{
		glfwSwapBuffers(window);
		glfwPollEvents();

		cout<<'.'; cout.flush();
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}