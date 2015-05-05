
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"

class Application
{
public:
	enum KEYPRESS_TYPE
	{
		K_W,
		K_A,
		K_S,
		K_D,
		NUM_KEYPRESS,
	};
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Exit();
	void Controller();
	static bool IsKeyPressed(unsigned short key);
	static bool GetMouseUpdate();

	static double mouse_last_x, mouse_last_y, mouse_current_x, mouse_current_y, mouse_diff_x, mouse_diff_y;
	static double camera_yaw, camera_pitch;

private:
	Application();
	~Application();

	const static int m_window_deadzone = 100;
	const static int m_window_width = 800;
	const static int m_window_height = 600;

	bool keypressed[NUM_KEYPRESS];

	//Declare a window object
	StopWatch m_timer;
	double m_dElapsedTime;
	double m_dAccumulatedTime_ThreadOne;
	double m_dAccumulatedTime_ThreadTwo;
};

#endif