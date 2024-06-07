#include "Object.h"
#include "MyH.h"
#include <iostream>

#include "Grobal.h"

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
void Object::DrawObjectImage(HDC mdc)
{
	//�̹����� ����κ��� �����ְ� �Ұ��� ���⼭ ����

	if (type == "Cookie")
	{
		int sprite_size = 319; // ������ �ϳ��� ������
		int line_size = 3; // �����Ӱ� ������ ������ �� ������

		float animation_speed = 7;

		image_raw = 1;
		image_col = 4;

		if (ani_state == ANI_running)
		{
			image_raw = 1;
			image_col = 4;
		}
		if (ani_state == ANI_jumping)
		{
			image_raw = 5;
			image_col = 4;
		}

		int animation_time = floor(MyH::fract(m_ElapseTime * animation_speed) * image_col);

		if (ani_state == ANI_jumping && animation_time == 3)
		{
			ani_state = ANI_running;
		}

	

		ObjectImage.Draw(mdc,
			image.left, image.top, image.right - image.left, image.bottom - image.top, // x, y, ����, ����,
			line_size * animation_time + animation_time * sprite_size + 2, line_size * (image_raw + 1) + sprite_size * image_raw, sprite_size, sprite_size); // �̹��� ���� ��ǥ


	}
	else if (type == "Ground")
	{
		ObjectImage.Draw(mdc,
			image.left, image.top, image.right - image.left, image.bottom - image.top, // x, y, ����, ����,
			0, 0, ObjectImage.GetWidth(), ObjectImage.GetHeight());
	}
	else if (type == "BackGround")
	{


		ObjectImage.Draw(mdc,
			image.left, image.top, image.right - image.left, image.bottom - image.top, // x, y, ����, ����,
			0, 0, ObjectImage.GetWidth(), ObjectImage.GetHeight());
	}
	else
	{
		ObjectImage.Draw(mdc,
			image.left, image.top, image.right - image.left, image.bottom - image.top, // x, y, ����, ����,
			0, 0, ObjectImage.GetWidth(), ObjectImage.GetHeight());
	}

	if (DebugMode)
	{
		//��ġ Ȯ�ο�
		Draw::MakeCircle(mdc, pos_x - 5, pos_y - 5, pos_x + 5, pos_y + 5, RGB(0, 0, 255), 1, RGB(255, 0, 0));
		Draw::MakeDebugRectangle(mdc, image.left, image.top, image.right, image.bottom, RGB(0, 255, 0), 1);
		Draw::MakeDebugRectangle(mdc, CollisionBox.left, CollisionBox.top, CollisionBox.right, CollisionBox.bottom, RGB(255, 0, 0), 1);
	}

}

void Object::BeginEvents()
{
	// ������Ʈ�� ó�� ���忡 ���� �Ǿ��� �� �ѹ��� �߻��ϴ� �̺�Ʈ

	if (type == "Cookie")
	{
		float m_size = 100;
		SetObjectVertexLocation(pos_x - m_size * size, pos_y - m_size * size, pos_x + m_size * size, pos_y + m_size * size);

		SetCollisionBox(m_size / 2 * size, m_size / 2 * size, 0 * size, m_size * size);

		ani_state = ANI_running;
	}
	else if (type == "BackGround")
	{
		float h = ObjectImage.GetHeight();
		float ratio = WINDOW_HEIGHT / h;

		SetObjectVertexLocation(0, 0, WINDOW_WIDTH * ratio, WINDOW_HEIGHT);
	}
	else
	{
		SetCollisionBox(ObjectImage.GetWidth() / 2, ObjectImage.GetWidth() / 2, ObjectImage.GetHeight() / 2, ObjectImage.GetHeight() / 2);
	}
}

void Object::TickEvents()
{
	// �ֱ������� ��� �߻��ϴ� �̺�Ʈ(�ֱ�� 0.008��) 
	UpdateCollisionBox();

	if (type == "BackGround")
	{
		float speed = 2;
		AddObjectMovement(-speed, 0);
	}
}

void Object::CollisionEvent(Object* byWhat)
{
	if (byWhat)
	{



		ani_state = ANI_jumping;
	}
	else
	{
		ani_state = ANI_running;
	}

}



void Object::DrawObject(HDC m_mDC)
{

}

float Object::GetDirectaion()
{
	//�ӵ��� ������ �����̴� ������Ʈ�� ������ ���ϴ� �Լ�
	return atan2(vel_y, vel_x);
}
void Object::SetObjectVertexLocation(float x1, float y1, float x2, float y2)
{
	//������Ʈ�� �̹����� �� ������ ��ǥ�� �������ִ� �Լ�
	image.left = x1;
	image.right = x2;
	image.top = y1;
	image.bottom = y2;

	pos_x = (image.right - image.left) / 2 + image.left;
	pos_y = (image.bottom - image.top) / 2 + image.top;
}
void Object::AddObjectMovement(float del_x, float del_y)
{
	//�ӵ� ��ȭ���� �Է¹޾� ������Ʈ�� �����̰� �ϴ� �Լ�

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
	//������Ʈ�� ��ġ�� �������ִ� �Լ�

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





