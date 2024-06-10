#include "Object.h"
#include "ObjectManager.h"
#include "MyH.h"
#include "ImageLoader.h"
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
	//이미지의 어느부분을 보여주게 할건지 여기서 정함
	int showRange{ WINDOW_WIDTH };
	if (-showRange < pos_x && pos_x < WINDOW_WIDTH + showRange)
	{
		if (type == "Cookie")
		{
			int sprite_size = 319; // 프레임 하나의 사이즈
			int line_size = 3; // 프레임과 프레임 사이의 선 사이즈

			float animation_speed = 7;

			image_raw = 1;
			image_col = 4;

			switch (ani_state) {
			case ANI_running:
				image_raw = 1;
				image_col = 4;
				break;
			case ANI_jumping:
				image_raw = 5;
				image_col = 4;
				break;
			case ANI_double_jumping:
				image_raw = 0;
				image_col = 9;
				break;
			case ANI_sliding:
				image_raw = 0;
				image_col = 11;
				break;
			case ANI_sliding_up:
				image_raw = 0;
				image_col = 14;
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
					ani_state = ANI_running;
					count_jump = 0;
				}
				break;
			case ANI_double_jumping:
				if (animation_time == 8)
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
			}



			ObjectImage.Draw(mdc,
				image.left, image.top, image.right - image.left, image.bottom - image.top, // x, y, 넓이, 높이,
				line_size * animation_time + animation_time * sprite_size + 2, line_size * (image_raw + 1) + sprite_size * image_raw, sprite_size, sprite_size); // 이미지 내의 좌표


		}
		else if (type == "BackGround" || type == "BackGround2")
		{
			ObjectImage.Draw(mdc,
				image.left, image.top, image.right - image.left, image.bottom - image.top, // x, y, 넓이, 높이,
				0, 0, ObjectImage.GetWidth(), ObjectImage.GetHeight());
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

	if (type == "Cookie")
	{
		float m_size = 140;
		original_y = pos_y;
		SetObjectVertexLocation(pos_x - m_size * size, pos_y - m_size * size, pos_x + m_size * size, pos_y + m_size * size);

		SetCollisionBox(m_size / 2 * size, m_size / 2 * size, 0 * size, m_size * size);

		ani_state = ANI_running;
		count_jump = 0;
	}
	else if (type == "BackGround" || type == "BackGround2")
	{
		float h = ObjectImage.GetHeight();
		float ratio = WINDOW_HEIGHT / h;

		SetObjectVertexLocation(pos_x, pos_y, pos_x + WINDOW_WIDTH * ratio, pos_y + WINDOW_HEIGHT);
	}
	else if (type == "Bridge")
	{
		float m_size = 60;
		SetObjectVertexLocation(pos_x - m_size * size, pos_y - m_size * size, pos_x + m_size * size, pos_y + m_size * size);
		SetCollisionBox(ObjectImage.GetWidth() / 2, ObjectImage.GetWidth() / 2, ObjectImage.GetHeight() / 2, ObjectImage.GetHeight() / 2);
	}
	else if (type == "Jump")
	{
		float ratio1 = ObjectImage.GetWidth() / 1.6;
		float ratio2 = ObjectImage.GetHeight() / 1.6;
		SetObjectVertexLocation(pos_x, pos_y, pos_x + ratio1, pos_y + ratio2);
		SetCollisionBox(ObjectImage.GetWidth() / 2, ObjectImage.GetWidth() / 2, ObjectImage.GetHeight() / 2, ObjectImage.GetHeight() / 2);
	}
	else if (type == "Jelly")
	{
		float ratio1 = ObjectImage.GetWidth() / 1.6;
		float ratio2 = ObjectImage.GetHeight() / 1.6;
		SetObjectVertexLocation(pos_x, pos_y, pos_x + ratio1, pos_y + ratio2);
		SetCollisionBox(ObjectImage.GetWidth() / 2, ObjectImage.GetWidth() / 2, ObjectImage.GetHeight() / 2, ObjectImage.GetHeight() / 2);
	}

}

void Object::TickEvents()
{
	// 주기적으로 계속 발생하는 이벤트(주기는 0.008초) 
	UpdateCollisionBox();

	if (type == "BackGround")
	{
		float speed = 6;
		AddObjectMovement(-speed, 0);
	}
	else if (type == "BackGround2")
	{
		float speed = 2;
		AddObjectMovement(-speed, 0);
	}
	else if (type == "Bridge" || type == "Sliding1" || type == "Jump" || type == "Jelly")
	{
		float speed = 6;
		AddObjectMovement(-speed, 0);
	}
}

void Object::CollisionEvent(Object* byWhat)
{
	if (byWhat)
	{
		if (type == "Cookie" && byWhat->type == "Jelly") byWhat->SetObjectLocation(-10000,0);


		
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
	int jump_height = 1000;
	int falling_time = 1;
	int speed = 5;
	float jumping_time = m_ElapseTime * speed;
	pos_y = jump_height * jumping_time * (jumping_time - falling_time) + original_y;
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





