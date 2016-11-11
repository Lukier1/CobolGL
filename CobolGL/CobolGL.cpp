// CobolGL.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include "Include/CobMath.h"
#include "Include\ShaderProgram.h"
#include "Include\Exceptions\IOException.h"
#include "Include\Mesh.h"
#include "Include\ControllerRec.h"
void glfw_error_callback(int error, const char* description) {
	Logger::getLogger()->gl_log(error, description);
}

double camAng = 0;
float range = 2.0f;

struct Camera {
	Vec3 position = Vector3f(0, 0, -2.0f);
	Vec3 rotation = Vector3f(0, 0, 0);
};

Camera cam;
void Logic()
{
	ControllerRec& rec = ControllerRec::GetSingleton();
//	if (rec.IsDown(GLFW_KEY_ESCAPE))
		//glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (rec.IsDown(GLFW_KEY_S))
	{
		float v[] = { 0, 0, -0.001, 0 };
		Vec4 move4 = Matrix4x4::rotation(cam.rotation.v[0], cam.rotation.v[1], cam.rotation.v[2])*Vec4(v);
		Vec3 move3 = Vector3f(move4.v[0], move4.v[1], move4.v[2]);
		cam.position = cam.position +  move3;

	}
	if (rec.IsDown(GLFW_KEY_W))
	{
		float v[] = { 0, 0, 0.001, 0 };
		Vec4 move4 = Matrix4x4::rotation(cam.rotation.v[0], cam.rotation.v[1], cam.rotation.v[2])*Vec4(v);
		Vec3 move3 = Vector3f(move4.v[0], move4.v[1], move4.v[2]);
		cam.position = cam.position + move3;
	}
	if (rec.IsDown(GLFW_KEY_A))
	{
		float v[] = { -0.001, 0, 0, 0 };
		Vec4 move4 = Matrix4x4::rotation(cam.rotation.v[0], cam.rotation.v[1], cam.rotation.v[2])*Vec4(v);
		Vec3 move3 = Vector3f(move4.v[0], move4.v[1], move4.v[2]);
		cam.position = cam.position + move3;

	}
	if (rec.IsDown(GLFW_KEY_D))
	{
		float v[] = { 0.001,0, 0, 0 };
		Vec4 move4 = Matrix4x4::rotation(cam.rotation.v[0], cam.rotation.v[1], cam.rotation.v[2])*Vec4(v);
		Vec3 move3 = Vector3f(move4.v[0], move4.v[1], move4.v[2]);
		cam.position = cam.position + move3;
	}
	cam.rotation.v[1] += rec.GetMoveX()/3.14/400.0;
	cam.rotation.v[0] += rec.GetMoveY()/3.14/300.0;
	
	/*if (rec.IsDown(GLFW_KEY_D))
	{
		
	}
	if (rec.IsDown(GLFW_KEY_A))
	{
		camAng -= 3.14f/12000.0f;
	}*/
	//camPos.v[0] = range*sin(camAng);
	//camPos.v[2] = range*cos(camAng);
	
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
void APIENTRY debug_gl_callback(GLenum source, GLenum type, GLuint id, GLenum severity, 
						GLsizei length, const GLchar* message, GLvoid* userParam
	) {
	char src_str[2048]; /* source */
	char type_str[2048]; /* type */
	char sev_str[2048]; /* severity */

	switch (source) {
	case 0x8246:
		strcpy(src_str, "API");
		break;
	case 0x8247:
		strcpy(src_str, "WINDOW_SYSTEM");
		break;
	case 0x8248:
		strcpy(src_str, "SHADER_COMPILER");
		break;
	case 0x8249:
		strcpy(src_str, "THIRD_PARTY");
		break;
	case 0x824A:
		strcpy(src_str, "APPLICATION");
		break;
	case 0x824B:
		strcpy(src_str, "OTHER");
		break;
	default:
		strcpy(src_str, "undefined");
		break;
	}

	switch (type) { //type
	case 0x824C:
		strcpy(type_str, "ERROR");
		break;
	case 0x824D:
		strcpy(type_str, "DEPRECATED_BEHAVIOR");
		break;
	case 0x824E:
		strcpy(type_str, "UNDEFINED_BEHAVIOR");
		break;
	case 0x824F:
		strcpy(type_str, "PORTABILITY");
		break;
	case 0x8250:
		strcpy(type_str, "PERFORMANCE");
		break;
	case 0x8251:
		strcpy(type_str, "OTHER");
		break;
	case 0x8268:
		strcpy(type_str, "MARKER");
		break;
	case 0x8269:
		strcpy(type_str, "PUSH_GROUP");
		break;
	case 0x826A:
		strcpy(type_str, "POP_GROUP");
		break;
	default:
		strcpy(type_str, "undefined");
		break;
	}

	switch (severity) {
	case 0x9146:
		strcpy(sev_str, "HIGH");
		break;
	case 0x9147:
		strcpy(sev_str, "MEDIUM");
		break;
	case 0x9148:
		strcpy(sev_str, "LOW");
		break;
	case 0x826B:
		strcpy(sev_str, "NOTIFICATION");
		break;
	default:
		strcpy(sev_str, "undefined");
		break;
	}

	fprintf(
		stderr,
		"source: %s type: %s id: %u severity: %s length: %i message: %s userParam: %i\n",
		src_str,
		type_str,
		id,
		sev_str,
		length,
		message,
		*(int*)userParam
		);
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
	glfwSetKeyCallback(window, ControllerRec::KeyCallback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	//glfwSetCursorPosCallback(window, ControllerRec::MousePosCallback);
	glfwMakeContextCurrent(window);
	
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glewInit();



	if (GLEW_KHR_debug) {
		int param = -1;
		printf("KHR_debug extension found\n");
		glDebugMessageCallback(debug_gl_callback, &param);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		printf("debug callback engaged\n");
	}
	else {
		printf("KHR_debug extension NOT found\n");
	}

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);


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

	Mesh mesh;
	mesh.LoadFile("Data/Meshes/testpool.obj");
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CW); // GL_CCW for counter clock-wise
	
	//glDepthFunc(GL_NEAREST);
	
		
	Matrix4x4 world_mat = Matrix4x4::rotation(0, 0, 0.0f);
	persp_mat = Matrix4x4::perspective(0.1f, 100.0f, 3.14f/3.0f, 800.0f/600.0f);
	float angle = 1.0f;

	//Light const
	const Vec3 LPositon = Vector3f(10.0f, 0.0f, 0.0f);
	const Vec3 Ls = Vector3f(1, 1, 1);
	const Vec3 Ld = Vector3f(1.0f, 1.0f, 1.0f);
	const Vec3 La = Vector3f(0.4f, 0.4f, 0.4f);
	const Vec3 Ks = Vector3f(1.0f, 1.0f, 1.0f);
	const Vec3 Kd = Vector3f(1.0f, 1.0f, 1.0f);
	const Vec3 Ka = Vector3f(1.0f, 1.0f, 1.0f);
	float specular_exponent = 50.0f;
	
	//Loading image
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP * dib(0);

	const char * filename = "data/images/cubTex.png";
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
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	while (!glfwWindowShouldClose(window))
	{
		
		angle += 0.001f;
		Quaterion rot(angle / 180.0f*3.14f, Vector3f(0.0, 1.0, 0.0).normalize());
		world_mat = Matrix4x4::translate(0, 0, 0)*rot.getMatrix();
		//view_mat = Matrix4x4::lookat(camPos, Vector3f(0, 0, 0), Vector3f(0, 1, 0));
		//view_mat = Matrix4x4::viewMatrix(Vector3f(angle , 0, 0), cam.position);
		view_mat = Matrix4x4::viewMatrix(cam.rotation, cam.position);
		Matrix4x4 send_mat = persp_mat*view_mat*world_mat;
		Matrix4x4 mv_mat = view_mat*world_mat;
		send_mat = persp_mat*mv_mat;
		//####################
		//Test
		float dv[] = { 1, 1, 1, 1 };
		Vec4 ret = send_mat*Vec4(dv);
		
		Vec4 ret2 = send_mat*Vec4(0, 0, 0, 1);
		//#####################
		//angle += 0.005f;
		
		_update_fps_counter(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
		
		shaderProgram.UseProgram();

		shaderProgram.applyTexture("basic_texutre", 0);
	
		shaderProgram.apply("mvp_mat", send_mat);
		shaderProgram.apply("mv_mat", mv_mat);
		shaderProgram.apply("view_mat", view_mat);
		shaderProgram.apply("LPos_world", LPositon);
		shaderProgram.apply("Ls", Ls);
		shaderProgram.apply("Ld", Ld);
		shaderProgram.apply("La", La);
		shaderProgram.apply("Ks", Ks);
		shaderProgram.apply("Kd", Kd);
		shaderProgram.apply("Ka", Ka);
		shaderProgram.apply("spec_power", specular_exponent);
		
		//glBindVertexArray(vao);
		// draw points 0-3 from the currently bound VAO with current in-use shader
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		mesh.Draw();
		// update other events like input handling 

		glfwSwapBuffers(window);
		
		
		
		ControllerRec::GetSingleton().CleanStates(window);
		glfwPollEvents();
		Logic();
		glfwSetCursorPos(window, 400.0, 300.0);
	}
	shaderProgram.Release();
	FreeImage_Unload(dib);
	glfwTerminate();
	return 0;
}

