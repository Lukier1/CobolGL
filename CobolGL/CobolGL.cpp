// CobolGL.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include "Include/CobMath.h"
#include "Include\ShaderProgram.h"
#include "Include\Exceptions\IOException.h"

void glfw_error_callback(int error, const char* description) {
	Logger::getLogger()->gl_log(error, description);
}

float camAng = 0;
float range = 2.0f;
Vec3 camPos = Vector3f(0,0,2.0f);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
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
		val[0] = (2.0f* (float)mouse_x) / 800 - 1.0f; // TODO: Add properties width/height
		val[1] = (2.0f* (float)mouse_y) / 600 - 1.0f;
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
		sprintf_s(tmp, "opengl @ fps: %.2f", fps);
		glfwSetWindowTitle(window, tmp);
		frame_count = 0;
	}
	frame_count++;
}

float points[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.5f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f
};

float normals[] = {
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f
};

float texcoords[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f
};

void applyMatrix(std::string location, GLuint shaderProgram, const float * matrixData)
{
	int matrix_location = glGetUniformLocation(shaderProgram, location.c_str());
	glUniformMatrix4fv(matrix_location, 1, GL_TRUE, matrixData);
}

void applyVec3(std::string location, GLuint shaderProgram, const float * vecData)
{
	int vec_location = glGetUniformLocation(shaderProgram, location.c_str());
	glUniform3fv(vec_location, 1, vecData);
}

void applyFloat(std::string location, GLuint shaderProgram,  float val)
{
	int vec_location = glGetUniformLocation(shaderProgram, location.c_str());
	glUniform1f(vec_location, val);
}

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
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), points, GL_STATIC_DRAW);
	GLuint colours_vbo = 0;
	glGenBuffers(1, &colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), normals, GL_STATIC_DRAW);
	GLuint tc_vbo = 0;
	glGenBuffers(1, &tc_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, tc_vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), texcoords, GL_STATIC_DRAW);


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
	glEnableVertexAttribArray(2);
	glBindVertexBuffer(2, tc_vbo, 0, 2 * sizeof(GL_FLOAT));
	glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(2, 2);

	ShaderProgram shaderProgram;
	
	try {
		shaderProgram.AddShader("Shaders/light_vert.glsl", ShaderProgram::VERTEX);
		shaderProgram.AddShader("Shaders/light_frag.glsl", ShaderProgram::FRAGMENT);
	}
	catch (IOException e)
	{
		return -1;
	}
	shaderProgram.GenerateProgram();
	GLuint shader_programme = shaderProgram.getProgramId();


	///glEnable(GL_CULL_FACE); // cull face
	//glCullFace(GL_BACK); // cull back face
	//glFrontFace(GL_CW); // GL_CCW for counter clock-wise

		
	Matrix4x4 world_mat = Matrix4x4::rotation(0, 0, 0.0f);
	persp_mat = Matrix4x4::perspective(0.1f, 100.0f, 3.14f/4.0f, 800.0f/600.0f);
	float angle = 1.0f;

	//Light const
	const Vec3 LPositon = Vector3f(10.0f, 10.0f, 10.0f);
	const Vec3 Ls = Vector3f(1, 1, 1);
	const Vec3 Ld = Vector3f(1.0f, 1.0f, 1.0f);
	const Vec3 La = Vector3f(0.1f, 0.1f, 0.1f);
	const Vec3 Ks = Vector3f(1.0f, 1.0f, 1.0f);
	const Vec3 Kd = Vector3f(1.0f, 1.0f, 1.0f);
	const Vec3 Ka = Vector3f(1.0f, 1.0f, 1.0f);
	float specular_exponent = 100.0f;
	
	//Loading image
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP * dib(0);

	const char * filename = "data/images/Test.png";
	fif = FreeImage_GetFileType(filename);
	dib = FreeImage_Load(fif, filename);
	if (!dib) {
		Logger::getLogger()->Error("Can't load " + std::string(filename));
		return -1;
	}
	int x, y;
	unsigned char * image_data = FreeImage_GetBits(dib);
	x = FreeImage_GetWidth(dib);
	y = FreeImage_GetHeight(dib);

	GLuint tex = 0;
	glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//#################################

	while (!glfwWindowShouldClose(window))
	{
		//angle += 0.005f;
		Quaterion rot(angle / 180.0f*3.14f, Vector3f(0.0, -1.0, -1.0).normalize());
		world_mat = Matrix4x4::translate(0, 0, 0)*rot.getMatrix();
		view_mat = Matrix4x4::lookat(camPos, Vector3f(0, 0, 0), Vector3f(0, 1, 0));
		Matrix4x4 send_mat = persp_mat*view_mat*world_mat;
		Matrix4x4 mv_mat = view_mat*world_mat;
		angle += 0.005f;
		
		_update_fps_counter(window);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
	
		glUseProgram(shader_programme);

		glBindTexture(GL_TEXTURE_2D, tex);
		int tex_loc = glGetUniformLocation(shader_programme, "basic_texture");
		glUniform1i(tex_loc, 0); // use active texture 0
		//int matrix_location = glGetUniformLocation(shader_programme, "matrix");
		//glUniformMatrix4fv(matrix_location, 1, GL_TRUE, send_mat.getMatrixData());

		applyMatrix("mvp_mat", shader_programme, send_mat.getMatrixData());
		applyMatrix("mv_mat", shader_programme, mv_mat.getMatrixData());
		applyMatrix("view_mat", shader_programme, view_mat.getMatrixData());	
		applyVec3("LPos_world", shader_programme, LPositon.getData());
		applyVec3("Ls", shader_programme, Ls.getData());
		applyVec3("Ld", shader_programme, Ld.getData());
		applyVec3("La", shader_programme, La.getData());
		applyVec3("Ks", shader_programme, Ks.getData());
		applyVec3("Kd", shader_programme, Kd.getData());
		applyVec3("Ka", shader_programme, Ka.getData());
		
		applyFloat("spec_power", shader_programme, specular_exponent);

		glBindVertexArray(vao);
		// draw points 0-3 from the currently bound VAO with current in-use shader
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		// update other events like input handling 

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	shaderProgram.Release();
	FreeImage_Unload(dib);
	glfwTerminate();
	return 0;
}

