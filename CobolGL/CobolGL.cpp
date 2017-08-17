// CobolGL.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include "Include/CobMath.h"
#include "Include\ShaderProgram.h"
#include "Include\Exceptions\IOException.h"
#include "Include\Mesh.h"
#include "Include\ControllerRec.h"
#include "Include\ModelLoader.h"
#include "Include\WindowGL.h"



double camAng = 0;
float range = 2.0f;

struct Camera {
	Vec3 position = Vector3f(0, 5.0f, -5.0f);
	Vec3 rotation = Vector3f(0.6f, 0, 0);
};

Camera cam;
void Logic()
{
	ControllerRec& rec = ControllerRec::GetSingleton();
//	if (rec.IsDown(GLFW_KEY_ESCAPE))
		//glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (rec.IsDown(GLFW_KEY_S))
	{
		float v[] = { 0, 0, -0.001f, 0 };
		Vec4 move4 = Matrix4x4::rotation(cam.rotation.v[0], cam.rotation.v[1], cam.rotation.v[2])*Vec4(v);
		Vec3 move3 = Vector3f(move4.v[0], move4.v[1], move4.v[2]);
		cam.position = cam.position +  move3;

	}
	if (rec.IsDown(GLFW_KEY_W))
	{
		float v[] = { 0, 0, 0.001f, 0 };
		Vec4 move4 = Matrix4x4::rotation(cam.rotation.v[0], cam.rotation.v[1], cam.rotation.v[2])*Vec4(v);
		Vec3 move3 = Vector3f(move4.v[0], move4.v[1], move4.v[2]);
		cam.position = cam.position + move3;
	}
	if (rec.IsDown(GLFW_KEY_A))
	{
		float v[] = { -0.001f, 0, 0, 0 };
		Vec4 move4 = Matrix4x4::rotation(cam.rotation.v[0], cam.rotation.v[1], cam.rotation.v[2])*Vec4(v);
		Vec3 move3 = Vector3f(move4.v[0], move4.v[1], move4.v[2]);
		cam.position = cam.position + move3;

	}
	if (rec.IsDown(GLFW_KEY_D))
	{
		float v[] = { 0.001f,0, 0, 0 };
		Vec4 move4 = Matrix4x4::rotation(cam.rotation.v[0], cam.rotation.v[1], cam.rotation.v[2])*Vec4(v);
		Vec3 move3 = Vector3f(move4.v[0], move4.v[1], move4.v[2]);
		cam.position = cam.position + move3;
	}
	cam.rotation.v[1] += (float)rec.GetMoveX()/3.14f/400.0f;
	cam.rotation.v[0] += (float)rec.GetMoveY()/3.14f/300.0f;

}


void APIENTRY debug_gl_callback(GLenum source, GLenum type, GLuint id, GLenum severity, 
						GLsizei length, const GLchar* message, GLvoid* userParam
	) {
	char src_str[2048]; /* source */
	char type_str[2048]; /* type */
	char sev_str[2048]; /* severity */

	switch (source) {
	case 0x8246:
		strcpy_s(src_str, sizeof(src_str), "API");
		break;
	case 0x8247:
		strcpy_s(src_str, sizeof(src_str), "WINDOW_SYSTEM");
		break;
	case 0x8248:
		strcpy_s(src_str, sizeof(src_str), "SHADER_COMPILER");
		break;
	case 0x8249:
		strcpy_s(src_str, sizeof(src_str), "THIRD_PARTY");
		break;
	case 0x824A:
		strcpy_s(src_str, sizeof(src_str), "APPLICATION");
		break;
	case 0x824B:
		strcpy_s(src_str, sizeof(src_str), "OTHER");
		break;
	default:
		strcpy_s(src_str, sizeof(src_str), "undefined");
		break;
	}

	switch (type) { //type
	case 0x824C:
		strcpy_s(type_str, sizeof(src_str), "ERROR");
		break;
	case 0x824D:
		strcpy_s(type_str, sizeof(src_str), "DEPRECATED_BEHAVIOR");
		break;
	case 0x824E:
		strcpy_s(type_str, sizeof(src_str), "UNDEFINED_BEHAVIOR");
		break;
	case 0x824F:
		strcpy_s(type_str, sizeof(src_str), "PORTABILITY");
		break;
	case 0x8250:
		strcpy_s(type_str, sizeof(src_str), "PERFORMANCE");
		break;
	case 0x8251:
		strcpy_s(type_str, sizeof(src_str), "OTHER");
		break;
	case 0x8268:
		strcpy_s(type_str, sizeof(src_str), "MARKER");
		break;
	case 0x8269:
		strcpy_s(type_str, sizeof(src_str), "PUSH_GROUP");
		break;
	case 0x826A:
		strcpy_s(type_str, sizeof(src_str), "POP_GROUP");
		break;
	default:
		strcpy_s(type_str, sizeof(src_str), "undefined");
		break;
	}

	switch (severity) {
	case 0x9146:
		strcpy_s(sev_str, sizeof(src_str), "HIGH");
		break;
	case 0x9147:
		strcpy_s(sev_str, sizeof(src_str), "MEDIUM");
		break;
	case 0x9148:
		strcpy_s(sev_str, sizeof(src_str), "LOW");
		break;
	case 0x826B:
		strcpy_s(sev_str, sizeof(src_str), "NOTIFICATION");
		break;
	default:
		strcpy_s(sev_str, sizeof(src_str), "undefined");
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
	
	WindowGL window;

	if (window.initWindow() != 0)
		return -1;

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

	ModelLoader ml("Data/Meshes/testpool.obj");

	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CW); // GL_CCW for counter clock-wise
	
	Matrix4x4 view_mat, persp_mat;
	Matrix4x4 world_mat = Matrix4x4::rotation(0, 0, 0.0f);
	persp_mat = Matrix4x4::perspective(0.1f, 100.0f, 3.14f/3.0f, 800.0f/600.0f);
	float angle = 1.0f;

	//Light default const
	const Vec3 LPositon = Vector3f(10.0f, 40.0f, 0.0f);
	const Vec3 Ls = Vector3f(1, 1, 1);
	const Vec3 Ld = Vector3f(1.0f, 1.0f, 1.0f);
	const Vec3 La = Vector3f(0.2f, 0.2f, 0.2f);
	const Vec3 Ks = Vector3f(1.0f, 1.0f, 1.0f);
	const Vec3 Kd = Vector3f(1.0f, 1.0f, 1.0f);
	const Vec3 Ka = Vector3f(1.0f, 1.0f, 1.0f);
	float specular_exponent = 100.0f;
	
	//CONFIG DEPTH 
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


	while (!window.isClose())
	{
		
		angle += 0.01f;
		Quaterion rot(angle / 180.0f*3.14f, Vector3f(0.0, 1.0, 0.0).normalize());
		world_mat = rot.getMatrix();
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
		window.startFrame();
		
		shaderProgram.UseProgram();
	 
		shaderProgram.apply("mvp_mat", send_mat);
		shaderProgram.apply("mv_mat", mv_mat);
		shaderProgram.apply("view_mat", view_mat);
		shaderProgram.apply("world_mat", world_mat);
		shaderProgram.apply("LPos_world", LPositon);
		shaderProgram.apply("Ls", Ls);
		shaderProgram.apply("Ld", Ld);
		shaderProgram.apply("La", La);
		shaderProgram.apply("Ks", Ks);
		shaderProgram.apply("Kd", Kd);
		shaderProgram.apply("Ka", Ka);
		shaderProgram.apply("spec_power", specular_exponent);
		
	
		auto meshList = ml.getMeshList();
		for (Mesh * mesh : meshList)
		{
			if (mesh->getMaterialIndex() != 0)
			{
				ml.getMaterialList()[mesh->getMaterialIndex()]->ApplyParams(shaderProgram);
			}
			//mesh->getMaterialIndex
			mesh->Draw();
		}
		// update other events like input handling 

		window.finishFrame();

		Logic();
		glfwSetCursorPos(window.getWindow(), window.getScreenWidth()/2.0, window.getScreenHeigth()/2.0);
	
		if (ControllerRec::GetSingleton().IsDown(GLFW_KEY_P))
		{
			window.screenshot("screen.png");
		}
	}
	shaderProgram.Release();

	glfwTerminate();
	return 0;
}

