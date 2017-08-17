#include "stdafx.h"

#include "Include/WindowGL.h"
#include "Include\ControllerRec.h"
WindowGL::WindowGL()
{
	mPreviousSeconds = glfwGetTime();
	mFrameCount = 0;
}

int  WindowGL::initWindow() {
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_SAMPLES, 8);
	mWindow = glfwCreateWindow(SIZE_W, SIZE_H, "Hello World", NULL, NULL);
	if (!mWindow)
	{
		glfwTerminate();
		return -1;
	}
	glfwSetErrorCallback(glfw_error_callback);
	glfwSetKeyCallback(mWindow, ControllerRec::KeyCallback);
	glfwSetMouseButtonCallback(mWindow, ControllerRec::MouseButtonCallback);
	//glfwSetCursorPosCallback(window, ControllerRec::MousePosCallback);
	glfwMakeContextCurrent(mWindow);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glewInit();

	return 0;
}
GLFWwindow *  WindowGL::getWindow() {
	return mWindow;
}
bool  WindowGL::isClose() {
	return glfwWindowShouldClose(mWindow) > 0;
}
void  WindowGL::startFrame() {
	update_fps_counter();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
}
void  WindowGL::finishFrame() {
	glfwSwapBuffers(mWindow);

	ControllerRec::GetSingleton().CleanStates(mWindow);
	glfwPollEvents();
}

void WindowGL::screenshot(std::string filename)
{
	unsigned char* buffer = new unsigned char[800 * 600 * 3]; //TODO: dodac sciaganie wymiarow okna z WindowGL
	glReadPixels(0, 0, SIZE_W, SIZE_H, GL_BGR, GL_UNSIGNED_BYTE, buffer);
	std::cout << "INFO: Saving screen to: " << filename << std::endl;
	FIBITMAP * bitmap = FreeImage_ConvertFromRawBits(buffer, SIZE_W, SIZE_H, 800 * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
	FreeImage_Save(FREE_IMAGE_FORMAT::FIF_PNG, bitmap, filename.c_str());

	FreeImage_Unload(bitmap);
	delete[] buffer;
}

unsigned short WindowGL::getScreenWidth() const
{
	return SIZE_W;
}

unsigned short WindowGL::getScreenHeigth() const
{
	return SIZE_H;
}

void WindowGL::update_fps_counter() {
	
	double current_seconds = glfwGetTime();
	double elapsed_seconds = current_seconds - mPreviousSeconds;
	if (elapsed_seconds > 0.25) {
		mPreviousSeconds = current_seconds;
		double fps = (double)mFrameCount / elapsed_seconds;
		char tmp[128];
		sprintf_s(tmp, "opengl @ fps: %.2f", fps);
		glfwSetWindowTitle(mWindow, tmp);
		mFrameCount = 0;
	}
	mFrameCount++;
}

void WindowGL::glfw_error_callback(int error, const char * description)
{
	Logger::getLogger()->gl_log(error, description);
}
