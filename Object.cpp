#include "Object.h"
#include "MyH.h"
#include <iostream>

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 600

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

		next = nullptr;
		prev = nullptr;
	}


	ani_state = ANI_running;

}

Object::~Object()
{
}

void Object::DrawObject(HDC m_mDC)
{

}

void Object::DrawObjectImage(HDC mdc)
{

	if (type == "Cookie")
	{
		int sprite_size = 269;
		int line_size = 3;

		float animation_speed = 7;

		int image_raw = 1;
		int image_col = 4;

		if (ani_state == ANI_running)
		{
			image_raw = 1;
			image_col = 4;
		}
		if (ani_state == ANI_jumping)
		{
			image_raw = 5;
			image_col = 5;
		}


		int animation_time = floor(MyH::fract(m_ElapseTime * animation_speed) * image_col);
		if (ani_state == ANI_jumping && animation_time > 3) ani_state = ANI_running;

		//Draw::MakeCircle(mdc, pos_x - 5, pos_y - 5, pos_x + 5, pos_y + 5, RGB(255, 0, 0), 1, RGB(255, 0, 0));

		ObjectImage.Draw(mdc,
			image.left, image.top, image.right - image.left, image.bottom - image.top,
			line_size * animation_time + animation_time * sprite_size + 2, line_size * (image_raw + 1) + sprite_size * image_raw, sprite_size, sprite_size);
	}


}


float Object::GetDirectaion()
{
	return atan2(vel_y, vel_x);
}
void Object::SetObjectVertexLocation(float x1, float y1, float x2, float y2)
{
	image.left = x1;
	image.right = x2;
	image.top = y1;
	image.bottom = y2;

	pos_x = (image.right - image.left) / 2 + image.left;
	pos_y = (image.bottom - image.top) / 2 + image.top;
}
void Object::AddObjectMovement(float del_x, float del_y)
{
	float left_d = pos_x - image.left;
	float right_d = pos_x - image.right;
	float top_d = pos_y - image.top;
	float bottom_d = pos_y - image.bottom;

	pos_x += del_x;
	pos_y += del_y;

	image.left = pos_x - left_d;
	image.right = pos_x - right_d;
	image.top = pos_y - top_d;
	image.bottom = pos_y - bottom_d;
}
void Object::SetObjectLocation(float x, float y)
{
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


