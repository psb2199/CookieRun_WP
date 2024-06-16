#include "ObjectManager.h"
#include "Grobal.h"

ObjectManager::ObjectManager()
{
	Head = nullptr;
	Tail = nullptr;
	Player = nullptr;
}

ObjectManager::~ObjectManager()

{
}

Object* ObjectManager::AddObject(int type, CImage& image, float size, float x, float y)
{
	Object* newObject = new Object;
	newObject->type = type;
	newObject->pos_x = x;
	newObject->pos_y = y;

	newObject->size = size;

	newObject->ObjectImage = image;

	float imageW = newObject->ObjectImage.GetWidth();
	float imageH = newObject->ObjectImage.GetHeight();

	newObject->image.left = newObject->pos_x - imageW / 2;
	newObject->image.right = newObject->pos_x + imageW / 2;
	newObject->image.top = newObject->pos_y - imageH / 2;
	newObject->image.bottom = newObject->pos_y + imageH / 2;


	newObject->BeginEvents();
	newObject->next = nullptr; // 새로운 노드의 다음 노드는 없음

	if (Head == nullptr) {
		Head = newObject;
		Tail = newObject;
		newObject->prev = nullptr; // 첫 번째 노드의 이전 노드는 없음
	}
	else {
		Tail->next = newObject;
		newObject->prev = Tail;
		Tail = newObject;
	}

	if (newObject->type == Cookie)
		Player = newObject;

	return newObject;
}

void ObjectManager::DeleteObject(Object* object)
{
	if (object == nullptr)
		return;

	if (object == Head) {
		Head = object->next;
		if (Head != nullptr)
			Head->prev = nullptr; // 다음 노드의 이전 노드는 없음
	}
	else {
		object->prev->next = object->next;
		if (object->next != nullptr)
			object->next->prev = object->prev;
	}

	if (object == Tail) {
		Tail = object->prev;
		if (Tail != nullptr)
			Tail->next = nullptr; // 이전 노드의 다음 노드는 없음
	}

	delete object;
}

void ObjectManager::DeleteAll()
{
	Object* ptr = Head;
	while (ptr != nullptr) {
		Object* temp = ptr;
		ptr = ptr->next;
		delete temp;
	}

	Head = nullptr;
	Tail = nullptr;
}

void ObjectManager::DrawAll(HDC mDC, HDC hDC)
{
	Object* ptr = Head;
	while (ptr != nullptr)
	{
		//ptr->DrawObject(mDC);
		//if (ptr->type == LifeBar2 && ptr->ObjectImage.GetWidth() - (100 - Player->hp) * 50 < 1) Player->PlayMode = false;
		if (Player->StartBackGround == true && Player->PlayMode == false && ptr->type == BackGround_Start)
		{
			ptr->ObjectImage.Draw(mDC,
				ptr->image.left, ptr->image.top, ptr->image.right - ptr->image.left, ptr->image.bottom - ptr->image.top, // x, y, 넓이, 높이,
				0, 0, ptr->ObjectImage.GetWidth(), ptr->ObjectImage.GetHeight());
		}
		else if (Player->StartBackGround == true && Player->PlayMode == false && Player->ClickStart == false && (ptr->type == GameStart_no))
		{
			ptr->ObjectImage.Draw(mDC,
				ptr->image.left, ptr->image.top, ptr->image.right - ptr->image.left, ptr->image.bottom - ptr->image.top, // x, y, 넓이, 높이,
				0, 0, ptr->ObjectImage.GetWidth(), ptr->ObjectImage.GetHeight());
		}
		else if (Player->StartBackGround == true && Player->PlayMode == false && Player->ClickStart == true && (ptr->type == GameStart_dim))
		{
			ptr->ObjectImage.Draw(mDC,
				ptr->image.left, ptr->image.top, ptr->image.right - ptr->image.left, ptr->image.bottom - ptr->image.top, // x, y, 넓이, 높이,
				0, 0, ptr->ObjectImage.GetWidth(), ptr->ObjectImage.GetHeight());
		}
		/*	else if (Player->StartBackGround == false&& Player->GameEnd == true )
			{
			}*/
		else if (Player->StartBackGround == false)
		{
			if (ptr->type == ScoreBoard && Player->GameEnd == true)
			{
				ptr->ObjectImage.Draw(mDC,
					ptr->image.left, ptr->image.top, ptr->image.right - ptr->image.left, ptr->image.bottom - ptr->image.top, // x, y, 넓이, 높이,
					0, 0, ptr->ObjectImage.GetWidth(), ptr->ObjectImage.GetHeight());
			}

			if (ptr->type == Playagain_dim&&Player->ClickAgain==true && Player->GameEnd == true)
			{
				ptr->ObjectImage.Draw(mDC,
					ptr->image.left, ptr->image.top, ptr->image.right - ptr->image.left, ptr->image.bottom - ptr->image.top, // x, y, 넓이, 높이,
					0, 0, ptr->ObjectImage.GetWidth(), ptr->ObjectImage.GetHeight());
			}
			else if (ptr->type == Playagain_no && Player->ClickAgain == false && Player->GameEnd == true)
			{
				ptr->ObjectImage.Draw(mDC,
					ptr->image.left, ptr->image.top, ptr->image.right - ptr->image.left, ptr->image.bottom - ptr->image.top, // x, y, 넓이, 높이,
					0, 0, ptr->ObjectImage.GetWidth(), ptr->ObjectImage.GetHeight());
			}

			if (ptr->type == EndGame_dim && Player->ClickEnd == true && Player->GameEnd == true)
			{
				ptr->ObjectImage.Draw(mDC,
					ptr->image.left, ptr->image.top, ptr->image.right - ptr->image.left, ptr->image.bottom - ptr->image.top, // x, y, 넓이, 높이,
					0, 0, ptr->ObjectImage.GetWidth(), ptr->ObjectImage.GetHeight());
			}
			else if (ptr->type == EndGame_no && Player->ClickEnd == false && Player->GameEnd == true)
			{
				ptr->ObjectImage.Draw(mDC,
					ptr->image.left, ptr->image.top, ptr->image.right - ptr->image.left, ptr->image.bottom - ptr->image.top, // x, y, 넓이, 높이,
					0, 0, ptr->ObjectImage.GetWidth(), ptr->ObjectImage.GetHeight());
			}

			if (ptr->type == Background_Last)
			{
				float h = ptr->ObjectImage.GetHeight();
				float ratio = WINDOW_HEIGHT / h;

				if (ptr->pos_x + WINDOW_WIDTH - 70 > WINDOW_WIDTH)
				{
				}
				else
				{
					Player->EndObjectMove = true;
				}
			}
			if (Player->PlayMode == false && Player->ClickExit == true && (ptr->type == Exit_dim))
			{
				ptr->ObjectImage.Draw(mDC,
					ptr->image.left, ptr->image.top, ptr->image.right - ptr->image.left, ptr->image.bottom - ptr->image.top, // x, y, 넓이, 높이,
					0, 0, ptr->ObjectImage.GetWidth(), ptr->ObjectImage.GetHeight());
			}
			else if (Player->PlayMode == false && Player->ClickExit == false && (ptr->type == Exit_no))
			{
				ptr->ObjectImage.Draw(mDC,
					ptr->image.left, ptr->image.top, ptr->image.right - ptr->image.left, ptr->image.bottom - ptr->image.top, // x, y, 넓이, 높이,
					0, 0, ptr->ObjectImage.GetWidth(), ptr->ObjectImage.GetHeight());
			}

			if (Player->PlayMode == false && Player->ClickContinue == true && (ptr->type == Continue_dim))
			{
				ptr->ObjectImage.Draw(mDC,
					ptr->image.left, ptr->image.top, ptr->image.right - ptr->image.left, ptr->image.bottom - ptr->image.top, // x, y, 넓이, 높이,
					0, 0, ptr->ObjectImage.GetWidth(), ptr->ObjectImage.GetHeight());
			}
			else if (Player->PlayMode == false && Player->ClickContinue == false && (ptr->type == Continue_no))
			{
				ptr->ObjectImage.Draw(mDC,
					ptr->image.left, ptr->image.top, ptr->image.right - ptr->image.left, ptr->image.bottom - ptr->image.top, // x, y, 넓이, 높이,
					0, 0, ptr->ObjectImage.GetWidth(), ptr->ObjectImage.GetHeight());
			}


			if (ptr->type == LifeBar2 && Player->hp > 100)
			{
				ptr->ObjectImage.Draw(mDC,
					ptr->image.left, ptr->image.top, ptr->image.right - ptr->image.left, ptr->image.bottom - ptr->image.top, // x, y, 넓이, 높이,
					0, 0, ptr->ObjectImage.GetWidth(), ptr->ObjectImage.GetHeight());

			}
			else if (ptr->type == LifeBar2 && (100 - Player->hp) > 0 && Player->hp > 0)
			{
				ptr->ObjectImage.Draw(mDC,
					ptr->image.left, ptr->image.top, ptr->image.right - ptr->image.left - (100 - Player->hp) * 4, ptr->image.bottom - ptr->image.top, // x, y, 넓이, 높이,
					0, 0, ptr->ObjectImage.GetWidth() - (100 - Player->hp) * 6, ptr->ObjectImage.GetHeight());

			}
			if (ptr->type == LifeBar2 && ptr->image.right - ptr->image.left - (100 - Player->hp) * 4 < 30) {
				//Player->PlayMode = false;
				Player->ani_state = ANI_die;
			}
			else
				ptr->DrawObjectImage(mDC, hDC);
		}
		ptr = ptr->next;

	}
	if (Player->StartBackGround == false)Player->DrawObjectImage(mDC, hDC);
}

Object* ObjectManager::GetAllObjects()
{
	return Head;
}
