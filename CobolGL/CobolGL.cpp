// CobolGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"



std::string LoadTextFromFile(std::string filename)
{
	std::fstream file;
	file.open(filename, std::fstream::in); // catch error
	if (file.fail())
		return "";
	else {
		std::string res((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
		return res;
	}
}


void error_callback(int error, const char * description)
{
	printf("Error: %s\n", description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

float points[] = {
	0.0f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f
};

int _tmain(int argc, _TCHAR* argv[])
{
	GLFWwindow * window;
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindVertexBuffer(0, vbo, 0, 3 * sizeof(GL_FLOAT));
	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE,0);
	glVertexAttribBinding(0, 0);

	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	std::string sVert = LoadTextFromFile("Shaders/simp_vert.glsl");
	std::string sFrag = LoadTextFromFile("Shaders/simp_frag.glsl");
	std::string sFragPink = LoadTextFromFile("Shaders/simpPink_frag.glsl");

	const char* vertex_shader = sVert.c_str();
	const char* fragment_shader = sFrag.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.6f, 0.6f, 0.8f, 1.0f);

		glUseProgram(shader_programme);
		glBindVertexArray(vao);
		// draw points 0-3 from the currently bound VAO with current in-use shader
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// update other events like input handling 

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

