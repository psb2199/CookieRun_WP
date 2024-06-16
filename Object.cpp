#include "Object.h"
#include "ObjectManager.h"
#include "MyH.h"
#include "ImageLoader.h"
#include <iostream>
#include <chrono>
#include <gdiplus.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#pragma comment(lib, "gdiplus.lib")


using namespace Gdiplus;

// GDI+ 초기화
void InitGDIPlus(ULONG_PTR* gdiplusToken)
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(gdiplusToken, &gdiplusStartupInput, NULL);
}

// GDI+ 종료
void ShutdownGDIPlus(ULONG_PTR gdiplusToken)
{
	GdiplusShutdown(gdiplusToken);
}


#include "Grobal.h"
int add_speed = 1;
bool MoveObject{ true };

Object::Object()
{

	{
		pos_x = NULL;
		pos_y = NULL;
		size = NULL;

		vel_x = NULL;
		vel_y = NULL;

		image.left = NULL;
		image.right = NULL;
		image.bottom = NULL;
		image.top = NULL;

		image_raw = NULL;
		image_col = NULL;


		next = nullptr;
		prev = nullptr;
	}


	ani_state = ANI_running;

}
Object::~Object()
{
}
void Object::DrawObjectImage(HDC mdc, HDC hDC)
{
	//이미지의 어느부분을 보여주게 할건지 여기서 정함
	int showRange{ WINDOW_WIDTH };
	if (-showRange < pos_x && pos_x < WINDOW_WIDTH + showRange)
	{
		if (type == Cookie)
		{
			int sprite_size = 319; // 프레임 하나의 사이즈
			int line_size = 3; // 프레임과 프레임 사이의 선 사이즈

			float animation_speed = 5;

			image_raw = 1;
			image_col = 4;

			switch (ani_state) {
			case ANI_running:
				image_raw = 1;
				image_col = 4;
				break;
			case ANI_jumping:
				image_raw = 1;
				image_col = 4;
				break;
			case ANI_double_jumping:
				image_raw = 1;
				image_col = 4;
				break;
			case ANI_sliding:
				image_raw = 0;
				image_col = 10;
				break;
			case ANI_sliding_up:
				image_raw = 0;
				image_col = 13;
				break;
			case ANI_collision:
				image_raw = 5;
				image_col = 6;
				break;
			case ANI_fast:
				image_raw = 1;
				image_col = 9;
				break;
			case ANI_die:
				image_raw = 4;
				image_col = 5;
				break;
			}

			int animation_time = floor(MyH::fract(m_ElapseTime * animation_speed) * image_col);

			switch (ani_state) {
			case ANI_jumping:
				if (count_jump == 2 && animation_time == 3)
				{
					ani_state = ANI_double_jumping;
					count_jump = 0;
				}
				else if (animation_time == 3)
				{
					if (FastMode == true)
						ani_state = ANI_fast;
					else
					{
						ani_state = ANI_running;
						count_jump = 0;
					}
				}
				break;
			case ANI_double_jumping:
				if (animation_time == 3)
				{
					ani_state = ANI_running;
				}
				break;
			case ANI_sliding:
				if (animation_time % 2 == 0)
					animation_time = 8;
				else
					animation_time = 9;
				break;
			case ANI_sliding_up:
				if (animation_time < 10)
				{
					animation_time = 10;
					ani_state = ANI_running;
				}
				if (animation_time == 13)
				{
					ani_state = ANI_running;
				}
				break;
			case ANI_collision:
				if (animation_time == 0)
				{
					animation_time = 3;
				}
				else if (animation_time == 1) {
					animation_time = 2;
				}
				else
				{
					ani_state = ANI_running;
				}
				break;
			case ANI_fast:
				if (animation_time % 2 == 0)
					animation_time = 4;
				else
					animation_time = 5;

				if (FastMode == false)ani_state = ANI_running;

				break;
			case ANI_die:
				if (animation_time == 4)
				{
					//PlayMode = false;
					//GameEnd = true;
					hp = 0;
				}
				break;
			}

			if (BigMode)
			{
				ObjectImage.Draw(mdc,
					image.left - 80, image.top - 130, (image.right - image.left) * 1.5, (image.bottom - image.top) * 1.5, // x, y, 넓이, 높이,
					line_size * animation_time + animation_time * sprite_size + 2, line_size * (image_raw + 1) + sprite_size * image_raw, sprite_size, sprite_size); // 이미지 내의 좌표
			}
			else if (InvincibilityMode)
			{
				if (animation_time % 2 == 0)
				{
					ObjectImage.Draw(mdc,
						image.left, image.top, image.right - image.left, image.bottom - image.top, // x, y, 넓이, 높이,
						line_size * animation_time + animation_time * sprite_size + 2, line_size * (image_raw + 1) + sprite_size * image_raw, sprite_size, sprite_size); // 이미지 내의 좌표
				}
			}


			else
			{
				ObjectImage.Draw(mdc,
					image.left, image.top, image.right - image.left, image.bottom - image.top, // x, y, 넓이, 높이,
					line_size * animation_time + animation_time * sprite_size + 2, line_size * (image_raw + 1) + sprite_size * image_raw, sprite_size, sprite_size); // 이미지 내의 좌표
			}


		}
		else if (type == Background || type == Background2 || type == Background_Last)
		{
			ObjectImage.Draw(mdc,
				image.left, image.top, image.right - image.left, image.bottom - image.top, // x, y, 넓이, 높이,
				0, 0, ObjectImage.GetWidth(), ObjectImage.GetHeight());
		}
		else if (type == Heart)
		{
			Graphics graphics(mdc);
			Image img = (L".\\CookieRun_Resource\\icon_heartLife.png");

			// 반투명 렌더링을 위해 이미지 속성 설정
			ColorMatrix colorMatrix = {
				1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, (float)255 / 255.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f, 1.0f
			};

			ImageAttributes imageAttributes;
			imageAttributes.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

			// 이미지 출력
			graphics.DrawImage(
				&img,
				Rect(image.left, image.top, (image.right - image.left), (image.bottom - image.top)),
				0, 0, ObjectImage.GetWidth(), ObjectImage.GetHeight(), UnitPixel, &imageAttributes);
		}
		else if (type == Pause)
		{
			Graphics graphics(mdc);
			Image img = (L".\\CookieRun_Resource\\puase.png");

			// 반투명 렌더링을 위해 이미지 속성 설정
			ColorMatrix colorMatrix = {
				1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, (float)255 / 255.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f, 1.0f
			};

			ImageAttributes imageAttributes;
			imageAttributes.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

			// 이미지 출력
			graphics.DrawImage(
				&img,
				Rect(image.left, image.top, (image.right - image.left), (image.bottom - image.top)),
				0, 0, ObjectImage.GetWidth(), ObjectImage.GetHeight(), UnitPixel, &imageAttributes);
		}
		else if (type == FastEffect)
		{
			ObjectImage.Draw(mdc,
				image.left, image.top, image.right - image.left, image.bottom - image.top, // x, y, 넓이, 높이,
				0, 0, ObjectImage.GetWidth(), ObjectImage.GetHeight());
		}
		else if (type == LifeBar1)
		{
			ObjectImage.Draw(mdc,
				image.left, image.top, image.right - image.left, image.bottom - image.top, // x, y, 넓이, 높이,
				0, 0, ObjectImage.GetWidth(), ObjectImage.GetHeight());
		}
		else if (type == LifeBar2 || type == Continue_dim || type == Continue_no ||
			type == Exit_dim || type == Exit_no || type == BackGround_Start
			|| type == GameStart_dim || type == GameStart_no || type == ScoreBoard
			|| type == EndGame_dim || type == EndGame_no || type == Playagain_dim || type == Playagain_no)
		{
		}
		else
		{
			ObjectImage.Draw(mdc,
				image.left, image.top, image.right - image.left, image.bottom - image.top, // x, y, 넓이, 높이,
				0, 0, ObjectImage.GetWidth(), ObjectImage.GetHeight());
		}

		if (DebugMode)
		{
			//위치 확인용
			Draw::MakeCircle(mdc, pos_x - 5, pos_y - 5, pos_x + 5, pos_y + 5, RGB(0, 0, 255), 1, RGB(255, 0, 0));
			//Draw::MakeDebugRectangle(mdc, image.left, image.top, image.right, image.bottom, RGB(0, 255, 0), 1);
			Draw::MakeDebugRectangle(mdc, CollisionBox.left, CollisionBox.top, CollisionBox.right, CollisionBox.bottom, RGB(255, 0, 0), 1);
		}
	}
}

void Object::BeginEvents()
{
	// 오브젝트가 처음 월드에 생성 되었을 때 한번만 발생하는 이벤트

	if (type == Cookie)
	{
		float m_size = 140;
		original_y = pos_y;
		hp = 100;
		StartBackGround = true;
		PlayMode = false;
		add_speed = 1;
		//FastMode = true;
		SetObjectVertexLocation(pos_x - m_size * size, pos_y - m_size * size, pos_x + m_size * size, pos_y + m_size * size);

		CollisionBox_Value.left = m_size / 4 * size;
		CollisionBox_Value.right = m_size / 4 * size;
		CollisionBox_Value.top = 0 * size;
		CollisionBox_Value.bottom = m_size / 1.5 * size;
		SetCollisionBox(CollisionBox_Value.left, CollisionBox_Value.right, CollisionBox_Value.top, CollisionBox_Value.bottom);


		ani_state = ANI_running;
		MagnetMode = true;
		count_jump = 0;
	}
	else if (type == BackGround_Start)
	{
		float ratio1 = ObjectImage.GetWidth() / 1.4;
		float ratio2 = ObjectImage.GetHeight() / 1.5;
		//SetObjectVertexLocation(pos_x, pos_y, pos_x + ratio1, pos_y + ratio2);
		SetObjectVertexLocation(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		//SetCollisionBox(ObjectImage.GetWidth() / 3, ObjectImage.GetWidth() / 3, ObjectImage.GetHeight() / 3, ObjectImage.GetHeight() / 3);
	}
	else if (type == Background || type == Background2 || type == Background_Last)
	{
		float h = ObjectImage.GetHeight();
		float ratio = WINDOW_HEIGHT / h;

		SetObjectVertexLocation(pos_x, pos_y, pos_x + WINDOW_WIDTH * ratio, pos_y + WINDOW_HEIGHT);
	}
	else if (type == Bridge)
	{
		float m_size = 60;
		SetObjectVertexLocation(pos_x - m_size * size, pos_y - m_size * size, pos_x + m_size * size, pos_y + m_size * size);
		SetCollisionBox(ObjectImage.GetWidth() / 2, ObjectImage.GetWidth() / 2, ObjectImage.GetHeight() / 2, ObjectImage.GetHeight() / 2);
	}
	else if (type == Obstacle_J)
	{
		float ratio1 = ObjectImage.GetWidth() / 1.6;
		float ratio2 = ObjectImage.GetHeight() / 1.6;
		SetObjectVertexLocation(pos_x, pos_y, pos_x + ratio1, pos_y + ratio2);
		SetCollisionBox(ObjectImage.GetWidth() / 3, ObjectImage.GetWidth() / 3, ObjectImage.GetHeight() / 3, ObjectImage.GetHeight() / 3);
	}
	else if (type == Obstacle_S)
	{
		float ratio1 = ObjectImage.GetWidth() / 1.17;
		float ratio2 = ObjectImage.GetHeight() / 1.17;
		SetObjectVertexLocation(pos_x, 0, pos_x + ratio1, 0 + ratio2);
		SetCollisionBox(ObjectImage.GetWidth() / 3, ObjectImage.GetWidth() / 3, ObjectImage.GetHeight() / 3, ObjectImage.GetHeight() / 3);
	}
	else if (type == PinkJelly || type == YellowJelly || type == BigJelly || type == GeneralJelly || type == Coin_S || type == Coin_B)
	{
		float ratio1 = ObjectImage.GetWidth() / 1.6;
		float ratio2 = ObjectImage.GetHeight() / 1.6;
		SetObjectVertexLocation(pos_x, pos_y, pos_x + ratio1, pos_y + ratio2);
		SetCollisionBox(ObjectImage.GetWidth() / 4, ObjectImage.GetWidth() / 4, ObjectImage.GetHeight() / 4, ObjectImage.GetHeight() / 4);
	}
	else if (type == Coin_G || type == Pause)
	{
		float ratio1 = ObjectImage.GetWidth() / 1.6;
		float ratio2 = ObjectImage.GetHeight() / 1.6;
		SetObjectVertexLocation(pos_x, pos_y, pos_x + ratio1, pos_y + ratio2);
		SetCollisionBox(ObjectImage.GetWidth() / 4, ObjectImage.GetWidth() / 4, ObjectImage.GetHeight() / 4, ObjectImage.GetHeight() / 4);
	}
	else if (type == Big || type == Fast || type == Energy || type == Magnet)
	{
		float ratio1 = ObjectImage.GetWidth() / 2.1;
		float ratio2 = ObjectImage.GetHeight() / 2.1;
		SetObjectVertexLocation(pos_x, pos_y, pos_x + ratio1, pos_y + ratio2);
		SetCollisionBox(ObjectImage.GetWidth() / 5, ObjectImage.GetWidth() / 5, ObjectImage.GetHeight() / 5, ObjectImage.GetHeight() / 5);
	}
	else if (type == EndGame_dim || type == EndGame_no || type == Playagain_dim || type == Playagain_no)
	{
		float ratio1 = ObjectImage.GetWidth() / 2.6;
		float ratio2 = ObjectImage.GetHeight() / 2.6;
		SetObjectVertexLocation(pos_x, pos_y, pos_x + ratio1, pos_y + ratio2);
		SetCollisionBox(ObjectImage.GetWidth() / 5, ObjectImage.GetWidth() / 5, ObjectImage.GetHeight() / 5, ObjectImage.GetHeight() / 5);
	}
	else if (type == FastEffect)
	{
		float ratio1 = ObjectImage.GetWidth() / 1.0;
		float ratio2 = ObjectImage.GetHeight() / 1.0;
		SetObjectVertexLocation(pos_x, pos_y, pos_x + ratio1, pos_y + ratio2);
		SetCollisionBox(ObjectImage.GetWidth() / 5, ObjectImage.GetWidth() / 5, ObjectImage.GetHeight() / 5, ObjectImage.GetHeight() / 5);
	}
	else if (type == Heart)
	{
		float ratio1 = ObjectImage.GetWidth() / 1.0;
		float ratio2 = ObjectImage.GetHeight() / 1.0;
		SetObjectVertexLocation(pos_x, pos_y, pos_x + ratio1, pos_y + ratio2);
		SetCollisionBox(ObjectImage.GetWidth() / 5, ObjectImage.GetWidth() / 5, ObjectImage.GetHeight() / 5, ObjectImage.GetHeight() / 5);
	}
	else if (type == LifeBar1 || type == Coin_Ikon)
	{
		float ratio1 = ObjectImage.GetWidth() / 1.8;
		float ratio2 = ObjectImage.GetHeight() / 1.8;
		SetObjectVertexLocation(pos_x, pos_y, pos_x + ratio1, pos_y + ratio2);
		SetCollisionBox(ObjectImage.GetWidth() / 5, ObjectImage.GetWidth() / 5, ObjectImage.GetHeight() / 5, ObjectImage.GetHeight() / 5);
	}
	else if (type == LifeBar2 || type == Continue_dim || type == Continue_no
		|| type == Exit_dim || type == Exit_no || type == GameStart_dim || type == GameStart_no)
	{
		float ratio1 = ObjectImage.GetWidth() / 1.8;
		float ratio2 = ObjectImage.GetHeight() / 1.8;
		SetObjectVertexLocation(pos_x, pos_y, pos_x + ratio1, pos_y + ratio2);
		SetCollisionBox(ObjectImage.GetWidth() / 5, ObjectImage.GetWidth() / 5, ObjectImage.GetHeight() / 5, ObjectImage.GetHeight() / 5);
	}
	else if (type == ScoreBoard)
	{
		float ratio1 = ObjectImage.GetWidth();
		float ratio2 = ObjectImage.GetHeight();
		SetObjectVertexLocation(pos_x, pos_y, pos_x + ratio1, pos_y + ratio2);
		SetCollisionBox(ObjectImage.GetWidth() / 5, ObjectImage.GetWidth() / 5, ObjectImage.GetHeight() / 5, ObjectImage.GetHeight() / 5);
	}

}

void Object::TickEvents()
{
	// 주기적으로 계속 발생하는 이벤트(주기는 0.008초) 
	UpdateCollisionBox();

	if (type == Cookie)
	{
		if (ani_state == ANI_sliding)
		{
			int Del_TopCollision = 20;
			SetCollisionBox(CollisionBox_Value.left, CollisionBox_Value.right, CollisionBox_Value.top - Del_TopCollision, CollisionBox_Value.bottom);
		}
		else
		{
			SetCollisionBox(CollisionBox_Value.left, CollisionBox_Value.right, CollisionBox_Value.top, CollisionBox_Value.bottom);
		}
	}

	if (InvincibilityMode == true) {
		item_time_invicible += DELTA_TIME;
		if (item_time_invicible > 1) {
			InvincibilityMode = false;
			item_time_invicible = 0;
		}
	}
	if (BigMode == true) {
		item_time_big += DELTA_TIME;
		if (item_time_big > 2.2) {
			BigMode = false;
			InvincibilityMode = true;
			item_time_big = 0;
		}
	}
	if (FastMode == true) {
		item_time_fast += DELTA_TIME;
		InvincibilityMode = false;
		add_speed = 2;
		if (item_time_fast > 2.1) {
			FastMode = false;
			InvincibilityMode = true;
			add_speed = 1;
			item_time_fast = 0;
		}
	}

	if (MagnetMode == true) {
		item_time_magnet += DELTA_TIME;
		if (item_time_magnet > 2.2) {
			//MagnetMode = false;
			item_time_magnet = 0;
		}
	}

	if (MoveObject)
	{
		if (type == Background)
		{
			float speed = 8 * add_speed;
			AddObjectMovement(-speed, 0);
		}
		else if (type == Background_Last)
		{
			float h = ObjectImage.GetHeight();
			float ratio = WINDOW_HEIGHT / h;

			if (pos_x + WINDOW_WIDTH - 70 > WINDOW_WIDTH)
			{
				float speed = 8 * add_speed;
				AddObjectMovement(-speed, 0);
			}
			else
			{
				MoveObject = false;
			}
		}
		else if (type == Background2)
		{
			float speed = 4 * add_speed;
			AddObjectMovement(-speed, 0);
		}
		else if (type == Bridge || type == Obstacle_J || type == Obstacle_S)
		{
			float speed = 8 * add_speed;
			AddObjectMovement(-speed, 0);
		}
		else if (type == PinkJelly || type == YellowJelly || type == BigJelly || type == GeneralJelly || type == Coin_S || type == Coin_B || type == Coin_G)
		{
			float speed = 8 * add_speed;
			AddObjectMovement(-speed, 0);

		}
		else if (type == Big || type == Fast || type == Energy || type == Magnet)
		{
			float speed = 8 * add_speed;
			AddObjectMovement(-speed, 0);

		}
		//else if (type == FastEffect)
		//{
		//	float speed = 16;
		//	AddObjectMovement(-speed, 0);
		//}

	}
}

void Object::CollisionEvent(Object* byWhat)
{
	if (byWhat && byWhat->isPassed == false)
	{
		// Jelly
		if (type == Cookie && byWhat->type == GeneralJelly) {
			byWhat->SetObjectLocation(-1000, 0);
			score += 500;
			byWhat->isPassed = true;
		}
		else if (type == Cookie && byWhat->type == PinkJelly) {
			byWhat->SetObjectLocation(-1000, 0);
			score += 1000;
			byWhat->isPassed = true;
		}
		else if (type == Cookie && byWhat->type == YellowJelly) {
			byWhat->SetObjectLocation(-1000, 0);
			score += 3000;
			byWhat->isPassed = true;
		}
		else if (type == Cookie && byWhat->type == BigJelly) {
			byWhat->SetObjectLocation(-1000, 0);
			score += 5000;
			byWhat->isPassed = true;
		}

		// Coin
		if (type == Cookie && byWhat->type == Coin_S) {
			byWhat->SetObjectLocation(-1000, 0);
			byWhat->isPassed = true;
			coin += 10;
		}
		else if (type == Cookie && byWhat->type == Coin_G) {
			byWhat->SetObjectLocation(-1000, 0);
			byWhat->isPassed = true;
			coin += 50;
		}
		else if (type == Cookie && byWhat->type == Coin_B) {
			byWhat->SetObjectLocation(-1000, 0);
			byWhat->isPassed = true;
			coin += 50;
		}

		// 장애물 충돌
		if (type == Cookie && (BigMode == true || FastMode == true) && (byWhat->type == Obstacle_J || byWhat->type == Obstacle_S)) {
			byWhat->SetObjectLocation(-10000, 0);
			score += 100;
			byWhat->isPassed = true;
		}
		else if (type == Cookie && InvincibilityMode == false && BigMode == false && FastMode == false
			&& (byWhat->type == Obstacle_J || byWhat->type == Obstacle_S))
		{
			byWhat->isPassed = true;
			hp -= 10;
			InvincibilityMode = true;
			m_ElapseTime = 0;
			ani_state = ANI_collision;
		}

		// 아이템
		if (type == Cookie && byWhat->type == Big) {
			byWhat->SetObjectLocation(-10000, 0);
			byWhat->isPassed = true;
			BigMode = true;
		}
		if (type == Cookie && byWhat->type == Fast) {
			byWhat->SetObjectLocation(-10000, 0);
			byWhat->isPassed = true;
			FastMode = true;
			ani_state = ANI_fast;
		}
		if (type == Cookie && byWhat->type == Magnet) {
			byWhat->SetObjectLocation(-10000, 0);
			byWhat->isPassed = true;
			MagnetMode = true;
		}
		if (type == Cookie && byWhat->type == Energy) {
			byWhat->SetObjectLocation(-10000, 0);
			hp += 5;
			byWhat->isPassed = true;
		}



	}
	else
	{

	}

}



void Object::DrawObject(HDC m_mDC)
{
}

float Object::GetDirectaion()
{
	//속도를 가지며 움직이는 오브젝트의 방향을 구하는 함수
	return atan2(vel_y, vel_x);
}
void Object::SetObjectVertexLocation(float x1, float y1, float x2, float y2)
{
	//오브젝트의 이미지의 네 꼭짓점 좌표를 설정해주는 함수
	image.left = x1;
	image.right = x2;
	image.top = y1;
	image.bottom = y2;

	pos_x = (image.right - image.left) / 2 + image.left;
	pos_y = (image.bottom - image.top) / 2 + image.top;
}
void Object::AddObjectMovement(float del_x, float del_y)
{
	//속도 변화량을 입력받아 오브젝트를 움직이게 하는 함수

	float left_d = pos_x - image.left;
	float right_d = pos_x - image.right;
	float top_d = pos_y - image.top;
	float bottom_d = pos_y - image.bottom;

	float c_left_d = pos_x - CollisionBox.left;
	float c_right_d = pos_x - CollisionBox.right;
	float c_top_d = pos_y - CollisionBox.top;
	float c_bottom_d = pos_y - CollisionBox.bottom;

	pos_x += del_x;
	pos_y += del_y;

	image.left = pos_x - left_d;
	image.right = pos_x - right_d;
	image.top = pos_y - top_d;
	image.bottom = pos_y - bottom_d;

	CollisionBox.left = pos_x - c_left_d;
	CollisionBox.right = pos_x - c_right_d;
	CollisionBox.top = pos_y - c_top_d;
	CollisionBox.bottom = pos_y - c_bottom_d;
}
void Object::SetObjectLocation(float x, float y)
{
	//오브젝트의 위치를 설정해주는 함수

	float left_d = pos_x - image.left;
	float right_d = pos_x - image.right;
	float top_d = pos_y - image.top;
	float bottom_d = pos_y - image.bottom;

	pos_x = x;
	pos_y = y;

	image.left = pos_x - left_d;
	image.right = pos_x - right_d;
	image.top = pos_y - top_d;
	image.bottom = pos_y - bottom_d;
}
void Object::SetCollisionBox(float l, float r, float t, float b)
{
	CollisionBox.left = pos_x - l;
	CollisionBox.right = pos_x + r;
	CollisionBox.top = pos_y - t;
	CollisionBox.bottom = pos_y + b;

	Del_CollisionBox.left = l;
	Del_CollisionBox.right = r;
	Del_CollisionBox.top = t;
	Del_CollisionBox.bottom = b;
}

void Object::DoJump()
{
	float left_d = pos_x - image.left;
	float right_d = pos_x - image.right;
	float top_d = pos_y - image.top;
	float bottom_d = pos_y - image.bottom;

	int jump_height = 1000;
	int falling_time = 1;
	int speed = 5;
	float jumping_time = m_ElapseTime * speed;

	pos_y = jump_height * jumping_time * (jumping_time - falling_time) + original_y;


	image.left = pos_x - left_d;
	image.right = pos_x - right_d;
	image.top = pos_y - top_d;
	image.bottom = pos_y - bottom_d;

	if (jumping_time >= falling_time) {
		jumping_time = 0;
		isJumping = false;
	}
}

void Object::SetDebugMode(bool value)
{
	DebugMode = value;
}

void Object::UpdateCollisionBox()
{
	CollisionBox.left = pos_x - Del_CollisionBox.left;
	CollisionBox.right = pos_x + Del_CollisionBox.right;
	CollisionBox.top = pos_y - Del_CollisionBox.top;
	CollisionBox.bottom = pos_y + Del_CollisionBox.bottom;
}





