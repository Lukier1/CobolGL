#pragma once

class ControllerRec {
public:
	void CleanStates(GLFWwindow * window);

	bool IsPushed(unsigned int key) const;
	bool IsReleased(unsigned int key) const;
	bool IsDown(unsigned int key) const;
	double GetMoveX();
	double GetMoveY();
	static ControllerRec& GetSingleton();
	
	static void MousePosCallback(GLFWwindow* window, double xpos, double ypos);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
private:

	ControllerRec();
	const unsigned int KEY_TABLE_SIZE = 1000;

	double centerX, centerY;
	double moveX, moveY;
	bool * pressKey, *releasedKey, *downKey;
	ControllerRec(ControllerRec &c) = delete;
	~ControllerRec();
};