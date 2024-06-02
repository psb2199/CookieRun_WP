#pragma once
#include <iostream>
#include <Windows.h>
#include <atlimage.h>

#include "Draw.h"


#define ANI_idle 0
#define ANI_running 1
#define ANI_jumping 2

struct ImageVertex 
{
	float left;
	float top;
	float right;
	float bottom;
};

class Object
{
public:
	Object();
	~Object();

	void DrawObject(HDC m_mDC);
	void DrawObjectImage(HDC mdc);

	float GetDirectaion();

	void SetObjectVertexLocation(float x1, float y1, float x2, float y2);
	void AddObjectMovement(float del_x, float del_y);
	void SetObjectLocation(float x, float y);



public:
	std::string type;
	CImage ObjectImage;
	float pos_x;
	float pos_y;
	float size;

	float m_ElapseTime;
	float vel_x;
	float vel_y;

	Object* next;
	Object* prev;

	ImageVertex image; //이미지의 (x1, y1) , (x2, y2)의 좌표
	
	int ani_state;
};

