#pragma once
#include "Object.h"
#include "ImageLoader.h"

class ObjectManager
{
private:
	Object* Head;
	Object* Tail;
	Object* Player;



public:
	ObjectManager();
	~ObjectManager();

	Object* AddObject(int type, CImage &image, float size, float x, float y);
	void DeleteObject(Object* object);
	void DeleteAll();

	void DrawAll(HDC mDC, HDC hDC);
	Object* GetAllObjects();
};

