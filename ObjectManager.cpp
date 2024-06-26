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
		if (ptr->type == LifeBar2 &&  Player->hp > 100)
		{
			ptr->ObjectImage.Draw(mDC,
				ptr->image.left, ptr->image.top, ptr->image.right - ptr->image.left, ptr->image.bottom - ptr->image.top, // x, y, 넓이, 높이,
				0, 0, ptr->ObjectImage.GetWidth(), ptr->ObjectImage.GetHeight());

		}
		else if (ptr->type == LifeBar2 &&  (100 - Player->hp) > 0 && Player->hp > 0)
		{
			ptr->ObjectImage.Draw(mDC,
				ptr->image.left, ptr->image.top, ptr->image.right - ptr->image.left - (100 - Player->hp) * 4, ptr->image.bottom - ptr->image.top, // x, y, 넓이, 높이,
				0, 0, ptr->ObjectImage.GetWidth() - (100 - Player->hp)*6 , ptr->ObjectImage.GetHeight());

		}
		if (ptr->type == LifeBar2 && ptr->image.right - ptr->image.left - (100 - Player->hp) * 4<10) {
			//Player->PlayMode = false;
			Player->ani_state = ANI_die;
		}
		else
			ptr->DrawObjectImage(mDC, hDC);

		ptr = ptr->next;
	}
	Player->DrawObjectImage(mDC, hDC);

}

Object* ObjectManager::GetAllObjects()
{
	return Head;
}
