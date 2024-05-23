#pragma once
#include "Object.h"

class ObjectManager
{
private:
	Object* Head;
	Object* Tail;
	Object* Player;



public:
	ObjectManager();
	~ObjectManager();

	Object* AddObject(std::string type, COLORREF color, float size, float x, float y, LPCTSTR strFileName);
	void DeleteObject(Object* object);
	void DeleteAll();

	void DrawAll(HDC mDC);
	Object* GetAllObjects();
};

