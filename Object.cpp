#include "Object.h"
#include "MyH.h"

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 600

Object::Object()
{

	{
		color = NULL;
		pos_x = NULL;
		pos_y = NULL;
		size = NULL;

		vel_x = NULL;
		vel_y = NULL;
		dir = 0;

		next = nullptr;
		prev = nullptr;
	}

	//3-3=====================
	{
		signal = 0;
		signal_time = 0;
	}

	//3-4=====================
	{
		YootFront = true;
		CanGoValue = 0;
		mapnum = 1;
	}

	//3-5=====================
	{
		clock_rotate = true;
		settle_light_dir = 1;
	}

	//4-2=====================
	{
		second_pos_x = NULL;
		second_pos_y = NULL;
		d_x1 = NULL;
		d_y1 = NULL;
		d_x2 = NULL;
		d_y2 = NULL;
		num = 0;
		selected = false;
		bord_switch = false;
	}


	//5-1=====================
	{
		left_x = NULL;
		right_x = NULL;
		top_y = NULL;
		bottom_y = NULL;
	}

	ani_state = ANI_running;

}

Object::~Object()
{
}

void Object::DrawObject(HDC m_mDC)
{
	if (type == "Player")
	{
		Draw::MakeCircle(m_mDC, pos_x - size, pos_y - size, pos_x + size, pos_y + size, color, 0, color);
	}

	//3-3============================================
	if (type == "Car_y" || type == "Car_y1")
	{
		Draw::MakeRectangle(m_mDC,
			pos_x + size * cos(MyH::Radian(245)),
			pos_y + size * sin(MyH::Radian(245)),
			pos_x + size * cos(MyH::Radian(65)),
			pos_y + size * sin(MyH::Radian(65)),
			color, 0, color);
	}
	if (type == "Car_x" || type == "Car_x1")
	{
		Draw::MakeRectangle(m_mDC,
			pos_x + size * sin(MyH::Radian(65)),
			pos_y + size * cos(MyH::Radian(65)),
			pos_x + size * sin(MyH::Radian(245)),
			pos_y + size * cos(MyH::Radian(245)),
			color, 0, color);
	}
	if (type == "Signal")
	{
		float side_size = 70;
		float up_size = 25;
		float radius = 20;
		Draw::MakeRectangle(m_mDC, pos_x - side_size, pos_y - up_size, pos_x + side_size, pos_y + up_size, RGB(0, 0, 0), 0, RGB(0, 0, 0));

		COLORREF signal_color = NULL;
		switch (signal)
		{
		case 0:
			signal_color = RGB(0, 255, 0);
			Draw::MakeCircle(m_mDC, pos_x - radius + 2 * radius, pos_y - radius, pos_x + radius + 2 * radius, pos_y + radius, signal_color, 0, signal_color);
			break;

		case 1:
			signal_color = RGB(255, 255, 0);
			Draw::MakeCircle(m_mDC, pos_x - radius, pos_y - radius, pos_x + radius, pos_y + radius, signal_color, 0, signal_color);
			break;

		case 2:
		case 3:
			signal_color = RGB(255, 0, 0);
			Draw::MakeCircle(m_mDC, pos_x - radius - 2 * radius, pos_y - radius, pos_x + radius - 2 * radius, pos_y + radius, signal_color, 0, signal_color);
			break;
		}
	}
	//===============================================


	//3-4============================================
	if (type == "Yoot")
	{
		float w = 5 * size;
		float h = 20 * size;
		if (YootFront)
		{
			Draw::MakeRectangle(m_mDC, pos_x - w, pos_y - h, pos_x + w, pos_y + h, color, 0, color);

			COLORREF YootLineColor = RGB(100, 50, 0);
			for (int i{ -1 }; i < 2; ++i)
			{
				float LineGap = i * 40;
				Draw::MakeLine(m_mDC, pos_x - w / 1.5, pos_y - w / 1.5 + LineGap, pos_x + w / 1.5, pos_y + w / 1.5 + LineGap, YootLineColor, 5, 0);
				Draw::MakeLine(m_mDC, pos_x + w / 1.5, pos_y - w / 1.5 + LineGap, pos_x - w / 1.5, pos_y + w / 1.5 + LineGap, YootLineColor, 5, 0);
			}
		}
		else
		{
			COLORREF YootBackColor = RGB(155, 155, 50);
			Draw::MakeRectangle(m_mDC, pos_x - w, pos_y - h, pos_x + w, pos_y + h, YootBackColor, 0, YootBackColor);
		}
	}
	if (type == "Pawn")
	{
		Draw::MakeCircle(m_mDC, pos_x - size, pos_y - size, pos_x + size, pos_y + size, color, 2, RGB(255, 255, 255));
	}
	//===============================================


	//3-5============================================
	if (type == "RotationCircle")
	{
		Draw::MakeCircle(m_mDC, pos_x - size, pos_y - size, pos_x + size, pos_y + size, color, 0, color);

		float cos_p;
		float sin_p;

		for (int i{}; i < 4; ++i)
		{
			cos_p = size * cos(MyH::Radian(dir + (i * 45)));
			sin_p = size * sin(MyH::Radian(dir + (i * 45)));
			Draw::MakeLine(m_mDC, pos_x - cos_p, pos_y - sin_p, pos_x + cos_p, pos_y + sin_p, NULL, 1, 0);
		}

		int small_size = size / 10;
		COLORREF small_color = RGB(255, 255, 0);
		cos_p = size * cos(MyH::Radian(dir * settle_light_dir));
		sin_p = size * sin(MyH::Radian(dir * settle_light_dir));
		Draw::MakeCircle(m_mDC, pos_x + cos_p - small_size, pos_y + sin_p - small_size, pos_x + cos_p + small_size, pos_y + sin_p + small_size, small_color, 0, small_color);
	}
	//===============================================


	//4-2============================================
	if (type == "rect")
	{
		if (selected && !bord_switch)Draw::MakeRectangle(m_mDC, pos_x, pos_y, second_pos_x, second_pos_y, color, 3, RGB(255, 0, 0));
		else Draw::MakeRectangle(m_mDC, pos_x, pos_y, second_pos_x, second_pos_y, color, 0, color);
	}
	//===============================================
}

void Object::DrawImageObject(HDC mdc)
{
	if (type == "BG")
	{
		ObjectImage.Draw(mdc,
			left_x, top_y, right_x - left_x, bottom_y - top_y,
			0, 0, ObjectImage.GetWidth(), ObjectImage.GetHeight());
	}
	if (type == "SpaceBG")
	{
		int size_x = 400;
		int size_y = 400;

		ObjectImage.Draw(mdc,
			left_x, top_y, right_x - left_x, bottom_y - top_y,
			0, ObjectImage.GetHeight() - size_y - m_ElapseTime * 20, size_x, size_y);
	}
	else if (type == "Cookie")
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
			left_x, top_y, right_x - left_x, bottom_y - top_y,
			line_size * animation_time + animation_time * sprite_size + 2, line_size * (image_raw + 1) + sprite_size * image_raw, sprite_size, sprite_size);
	}
}


float Object::GetDirectaion()
{
	return atan2(vel_y, vel_x);
}

void Object::SetObjectVertexLocation(float x1, float y1, float x2, float y2)
{
	left_x = x1;
	right_x = x2;
	top_y = y1;
	bottom_y = y2;

	pos_x = (right_x - left_x) / 2 + left_x;
	pos_y = (bottom_y - top_y) / 2 + top_y;
}

void Object::AddMovement(float del_x, float del_y)
{
	float left_d = pos_x - left_x;
	float right_d = pos_x - right_x;
	float top_d = pos_y - top_y;
	float bottom_d = pos_y - bottom_y;

	pos_x += del_x;
	pos_y += del_y;

	left_x = pos_x - left_d;
	right_x = pos_x - right_d;
	top_y = pos_y - top_d;
	bottom_y = pos_y - bottom_d;
}

void Object::SetObjectLocation(float x, float y)
{
	float left_d = pos_x - left_x;
	float right_d = pos_x - right_x;
	float top_d = pos_y - top_y;
	float bottom_d = pos_y - bottom_y;

	pos_x = x;
	pos_y = y;

	left_x = pos_x - left_d;
	right_x = pos_x - right_d;
	top_y = pos_y - top_d;
	bottom_y = pos_y - bottom_d;
}


