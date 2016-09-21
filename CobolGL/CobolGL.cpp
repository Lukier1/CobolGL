// CobolGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Include/CobMath.h"
class Log {
private:
	std::fstream file;
public:
	Log() {
		file.open("gl.log", std::fstream::out | std::fstream::trunc);
		if (!file.is_open())
		{
			std::cout << "Can't open file " << "gl.log";
		}
		else
		{
			file << "Log started" << std::endl;
			file.close();
		}
	}
	bool gl_log(int error, const char* description)
	{
		file.open("gl.log", std::fstream::out | std::fstream::app);
		if (!file.is_open())
		{
			std::cout << "Can't open file " << "gl.log";
			return false;
		}
		file << "GLWF Error code: " << error << " msg " << description << std::endl;
		file.close();
		return true;
	}
	~Log() {

	}
};

//TODO: make singleton
Log  logger;

void glfw_error_callback(int error, const char* description) {
	logger.gl_log(error, description);
}

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

float camAng = 0;
float range = 3.0f;
Vec3 camPos = Vector3f(0,0,3.0f);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_W)
		{
			range -= 1.0f;
		}
		if (key == GLFW_KEY_S)
		{
			range += 1.0f;
		}
		if (key == GLFW_KEY_D)
		{
			camAng += 3.14f/12.0f;
		}
		if (key == GLFW_KEY_A)
		{
			camAng -= 3.14f/12.0f;
		}
		camPos.v[0] = range*sin(camAng);
		camPos.v[2] = range*cos(camAng);
	}
}
Matrix4x4 view_mat, persp_mat;

struct Ray {
	Vec3 ray_wor, ray_clip;
	Ray(double mouse_x, double mouse_y)
	{
		float val[4];
		val[0] = (2.0f* mouse_x) / 800 - 1.0f; // TODO: Add properties width/height
		val[1] = (2.0f* mouse_y) / 600 - 1.0f;
		val[2] = -1.0f;
		val[3] = 1.0f;
		Vec4 ray_clip4(val);
		ray_clip = Vector3f(ray_clip4.v[0], ray_clip4.v[1], ray_clip4.v[2]);
		Vec4 ray_eye = persp_mat.inverse()*ray_clip4;

		ray_eye.v[2] = -1.0f;
		ray_eye.v[3] = 0.0f;

		Vec4 ray_wor4 = view_mat.inverse() * ray_eye;
		ray_wor = Vector3f(ray_wor4.v[0], ray_wor4.v[1], ray_wor4.v[2]);
		ray_wor = ray_wor.normalize();
	}
};

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
	double mouse_x, mouse_y;
	glfwGetCursorPos(window, &mouse_x, &mouse_y);

	if (action == GLFW_PRESS)
	{
		if (button == GLFW_MOUSE_BUTTON_1)
		{
			Ray ray(mouse_x, mouse_y);
			std::cout << "Clicked " << mouse_x << " " << mouse_y << std::endl;
		}
	}
}

void _update_fps_counter(GLFWwindow* window) {
	static double previous_seconds = glfwGetTime();
	static int frame_count;
	double current_seconds = glfwGetTime();
	double elapsed_seconds = current_seconds - previous_seconds;
	if (elapsed_seconds > 0.25) {
		previous_seconds = current_seconds;
		double fps = (double)frame_count / elapsed_seconds;
		char tmp[128];
		sprintf(tmp, "opengl @ fps: %.2f", fps);
		glfwSetWindowTitle(window, tmp);
		frame_count = 0;
	}
	frame_count++;
}

float points[] = {
	0.0f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f
};

float colours[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};


int _tmain(int argc, _TCHAR* argv[])
{
	
	GLFWwindow * window;
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_SAMPLES, 8);
	window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwSetErrorCallback(glfw_error_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
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
	GLuint colours_vbo = 0;
	glGenBuffers(1, &colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colours, GL_STATIC_DRAW);


	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindVertexBuffer(0, vbo, 0, 3 * sizeof(GL_FLOAT));
	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE,0);
	glVertexAttribBinding(0, 0);
	glEnableVertexAttribArray(1);
	glBindVertexBuffer(1, colours_vbo, 0, 3 * sizeof(GL_FLOAT));
	glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(1, 1);

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

	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CW); // GL_CCW for counter clock-wise

	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);
	
	
	Matrix4x4 world_mat = Matrix4x4::rotation(0, 0, 0.0f);
	persp_mat = Matrix4x4::perspective(0.1f, 100.0f, 3.14f/2.0f, 800.0/600.0f);
	float angle = 1.0f;
	
	
	
	while (!glfwWindowShouldClose(window))
	{
		angle += 1.0f;
		Quaterion rot(angle / 180.0f*3.14f, Vector3f(0, 1.0, 1.0).normalize());
		world_mat = Matrix4x4::translate(0, 0, 0)*rot.getMatrix();
		view_mat = Matrix4x4::lookat(camPos, Vector3f(0, 0, 0), Vector3f(0, 1, 0));
		Matrix4x4 send_mat = persp_mat*view_mat*world_mat;

		angle += 0.0001f;
		
		_update_fps_counter(window);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
		int matrix_location = glGetUniformLocation(shader_programme, "matrix");
		glUseProgram(shader_programme);
		glUniformMatrix4fv(matrix_location, 1, GL_TRUE, send_mat.getMatrixData());

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

