#pragma once
#include <Windows.h>


class Draw
{
public:
	Draw();
	~Draw();

public:
	static void MakeLine(HDC mDC, float x1, float y1, float x2, float y2, COLORREF incolor, int outlinethickness, int style);
	static void Triangle(HDC mDC, float midbottom_x, float midbottom_y, float height);
	static void MakeTriangle(HDC mDC, float x1, float y1, float x2, float y2, COLORREF incolor, int outlinethickness, COLORREF outcolor);
	static void MakeRectangle(HDC mDC, float x1, float y1, float x2, float y2, COLORREF incolor, int outlinethickness, COLORREF outcolor);
	static void MakeCircle(HDC mDC, float x1, float y1, float x2, float y2, COLORREF incolor, int outlinethickness, COLORREF outcolor);

	static void MakeDebugRectangle(HDC mDC, float x1, float y1, float x2, float y2, COLORREF incolor, int outlinethickness);
};

