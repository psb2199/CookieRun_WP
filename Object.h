#pragma once
#include <iostream>
#include <Windows.h>
#include <atlimage.h>
#include <chrono>

#include "Draw.h"


using namespace Gdiplus;

struct RectRange 
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
	void BeginEvents();
	void TickEvents();
	void CollisionEvent(Object* byWhat);

	void DrawObject(HDC m_mDC);
	void DrawObjectImage(HDC mdc, HDC hDC);

	float GetDirectaion();

	void SetObjectVertexLocation(float x1, float y1, float x2, float y2);
	void AddObjectMovement(float del_x, float del_y);
	void SetObjectLocation(float x, float y);
	void SetCollisionBox(float l, float r, float t, float b);
	void DoJump();
	void SetDebugMode(bool value);
	
private:
	void UpdateCollisionBox();
	bool DebugMode{ false };

public:
	int type;
	CImage ObjectImage;
	float pos_x;
	float pos_y;
	float size;

	int score{ 0 };
	int coin{ 0 };
	float hp{ 100 };

	float m_ElapseTime;
	float vel_x;
	float vel_y;
	float original_y;

	int image_raw;
	int image_col;

	float item_time_fast{ 0 };
	float item_time_big{ 0 };
	float item_time_invicible{ 0 };
	float item_time_magnet{ 0 };

	bool isJumping{ false };
	bool MagnetMode{ false };
	bool FastMode{ false };
	bool BigMode{ false };
	bool InvincibilityMode{ false };
	bool isPassed{ false };

	bool PlayMode{ true };
	bool GameEnd{ false };
	bool StartBackGround{ false };
	bool EndObjectMove{ false };
	bool ClickContinue{ false };
	bool ClickExit{ false };
	bool ClickStart{ false };
	bool ClickAgain{ false };
	bool ClickEnd{ false };

	Object* next;
	Object* prev;

	RectRange image; //이미지의 (x1, y1) , (x2, y2)의 좌표
	
	RectRange CollisionBox;
	RectRange CollisionBox_Value;

	int ani_state;
	int count_jump;

	

private:
	RectRange Del_CollisionBox;
};

