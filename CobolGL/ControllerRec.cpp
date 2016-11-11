#include "stdafx.h"

#include "Include\ControllerRec.h"


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
		if (key < controlleRec.KEY_TABLE_SIZE)
		{
			controlleRec.pressKey[key] = true;
			controlleRec.downKey[key] = true;
		}
	}
	if (action == GLFW_RELEASE)
	{
		if (key < controlleRec.KEY_TABLE_SIZE)
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

	for (int i = 0; i < KEY_TABLE_SIZE; ++i)
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
