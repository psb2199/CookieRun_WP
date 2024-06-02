#pragma once
#include <iostream>
#include <Windows.h>
#include "Draw.h"
#include <atlimage.h>

#define ANI_idle 0
#define ANI_running 1
#define ANI_jumping 2

class Object
{
public:
	Object();
	~Object();

	void DrawObject(HDC m_mDC);
	void DrawImageObject(HDC mdc);

	float GetDirectaion();

	void SetObjectVertexLocation(float x1, float y1, float x2, float y2);
	void AddMovement(float del_x, float del_y);
	void SetObjectLocation(float x, float y);

	CImage ObjectImage;


public:


	std::string type;
	COLORREF color;
	float pos_x;
	float pos_y;
	float size;

	float vel_x;
	float vel_y;
	float dir;

	float m_ElapseTime;

	Object* next;
	Object* prev;

	//3-3=====================
	int signal;
	float signal_time;
	//========================

	//3-4=====================
	bool YootFront;
	int CanGoValue;
	int mapnum;
	//========================

	//3-5=====================
	bool clock_rotate;
	int settle_light_dir;
	//========================

	//4-2=====================
	float center_x;
	float center_y;
	float second_pos_x;
	float second_pos_y;
	float d_x1;
	float d_y1;
	float d_x2;
	float d_y2;
	int num;
	bool selected;
	bool bord_switch;
	//========================

	float origin_x;
	float origin_y;

	float left_x;
	float top_y;
	float right_x;
	float bottom_y;

	int ani_state;
};

