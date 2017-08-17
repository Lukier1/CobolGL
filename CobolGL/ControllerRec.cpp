#include "stdafx.h"

#include "Include\ControllerRec.h"
#include "Include/CobMath.h"

/****
Temp
***/
struct Ray {
	Matrix4x4 view_mat, persp_mat; 
	Vec3 ray_wor, ray_clip;
	//Doesn't work -- problem with view_mat and persp_mat
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
/***
##########################
***/


void ControllerRec::CleanStates(GLFWwindow * window) {
	for (unsigned int i = 0; i < KEY_TABLE_SIZE; ++i)
	{
		pressKey[i] = false;
		releasedKey[i] = false;
	}
	double mX, mY;
	glfwGetCursorPos(window, &mX, &mY);
	moveX = mX - 400.0;
	moveY = mY - 300.0;
}

bool ControllerRec::IsPushed(unsigned int key) const
{
	return (key < KEY_TABLE_SIZE) ? pressKey[key] : false;
}
bool ControllerRec::IsReleased(unsigned int key) const
{
	return (key < KEY_TABLE_SIZE) ? releasedKey[key] : false;
}
bool ControllerRec::IsDown(unsigned int key) const {
	return (key < KEY_TABLE_SIZE) ? downKey[key] : false;
}

double ControllerRec::GetMoveX()
{
	return moveX;
}

double ControllerRec::GetMoveY() {
	return moveY;
}
void ControllerRec::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	ControllerRec& controlleRec = ControllerRec::GetSingleton();
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (action == GLFW_PRESS)
	{
		if ((unsigned int)key < controlleRec.KEY_TABLE_SIZE)
		{
			controlleRec.pressKey[key] = true;
			controlleRec.downKey[key] = true;
		}
	}
	if (action == GLFW_RELEASE)
	{
		if ((unsigned int)key < controlleRec.KEY_TABLE_SIZE)
		{
			controlleRec.releasedKey[key] = true;
			controlleRec.downKey[key] = false;
		}
	}
}



void ControllerRec::MousePosCallback(GLFWwindow* window, double xpos, double ypos) {
	ControllerRec& controlleRec = ControllerRec::GetSingleton();
	controlleRec.moveX = xpos - controlleRec.centerX;
	controlleRec.moveY = ypos - controlleRec.centerY;
}

void ControllerRec::MouseButtonCallback(GLFWwindow * window, int button, int action, int mode)
{
	double mouse_x, mouse_y;
	glfwGetCursorPos(window, &mouse_x, &mouse_y);

	if (action == GLFW_PRESS)
	{
		if (button == GLFW_MOUSE_BUTTON_1)
		{
	//		Ray ray(mouse_x, mouse_y);
			std::cout << "Clicked " << mouse_x << " " << mouse_y << std::endl;
		}
	}
}

ControllerRec& ControllerRec::GetSingleton() {
	static ControllerRec controllerRec;
	return controllerRec;
}
ControllerRec::ControllerRec()
{
	pressKey = new bool[KEY_TABLE_SIZE];
	releasedKey = new bool[KEY_TABLE_SIZE];
	downKey = new bool[KEY_TABLE_SIZE];

	centerX = 400;
	centerY = 300;
	moveX = 0;
	moveY = 0;

	for (unsigned int i = 0; i < KEY_TABLE_SIZE; ++i)
	{
		pressKey[i] = false;
		releasedKey[i] = false;
		downKey[i] = false;
	}
}

ControllerRec::~ControllerRec() {
	delete[] pressKey;
	delete[] releasedKey;
	delete[] downKey;
}
