#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
	Head = nullptr;
	Tail = nullptr;
	Player = nullptr;
}

ObjectManager::~ObjectManager()

{
}

Object* ObjectManager::AddObject(std::string type, COLORREF color, float size, float x, float y, LPCTSTR strFileName)
{
	Object* newObject = new Object;
	newObject->type = type;
	newObject->pos_x = x;
	newObject->pos_y = y;
	newObject->color = color;
	newObject->size = size;

	newObject->ObjectImage.Load(strFileName);

	float imageW = newObject->ObjectImage.GetWidth();
	float imageH = newObject->ObjectImage.GetHeight();

	newObject->left_x = newObject->pos_x - imageW / 2;
	newObject->right_x = newObject->pos_x + imageW / 2;
	newObject->top_y = newObject->pos_y - imageH / 2;
	newObject->bottom_y = newObject->pos_y + imageH / 2;



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

	if (newObject->type == "Player")
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

void ObjectManager::DrawAll(HDC mDC)
{
	Object* ptr = Head;
	while (ptr != nullptr)
	{
		//ptr->DrawObject(mDC);
		ptr->DrawImageObject(mDC);

		ptr = ptr->next;
	}
}

Object* ObjectManager::GetAllObjects()
{
	return Head;
}
