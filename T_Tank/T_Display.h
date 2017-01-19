#pragma once
#include <Windows.h> 
class T_Display
{
public:
	T_Display();
	~T_Display();

	void SaveMode();
	void ResetMode();
	bool ChangeMode(int width, int height);

private:
	DEVMODE	devmode_saved;
	bool mode_changed;
};