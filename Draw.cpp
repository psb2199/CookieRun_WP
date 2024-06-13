#include "Draw.h"
#include <iostream>
Draw::Draw()
{
}

Draw::~Draw()
{
}

void Draw::MakeCircle(HDC mDC, float x1, float y1, float x2, float y2, COLORREF incolor, int outlinethickness, COLORREF outcolor)
{
	HBRUSH hBrush = CreateSolidBrush(incolor);
	HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush); // 새로운 브러시 선택하기
	HPEN hPen = CreatePen(PS_SOLID, outlinethickness, outcolor);
	HPEN oldpen = (HPEN)SelectObject(mDC, hPen);

	Ellipse(mDC, x1, y1, x2, y2);

	hBrush = (HBRUSH)SelectObject(mDC, oldBrush); // 새로운 펜 선택하기
	DeleteObject(hBrush);
	hPen = (HPEN)SelectObject(mDC, oldpen); // 새로운 펜 선택하기
	DeleteObject(hPen);
}
void Draw::MakeDebugRectangle(HDC mDC, float x1, float y1, float x2, float y2, COLORREF incolor, int outlinethickness)
{
	MakeLine(mDC, x1, y1, x1, y2, incolor, outlinethickness, PS_SOLID);
	MakeLine(mDC, x1, y2, x2, y2, incolor, outlinethickness, PS_SOLID);
	MakeLine(mDC, x2, y2, x2, y1, incolor, outlinethickness, PS_SOLID);
	MakeLine(mDC, x2, y1, x1, y1, incolor, outlinethickness, PS_SOLID);
}
void Draw::MakeLine(HDC mDC, float x1, float y1, float x2, float y2, COLORREF incolor, int outlinethickness, int style)
{
	HPEN hPen = CreatePen(style, outlinethickness, incolor);
	HPEN oldpen = (HPEN)SelectObject(mDC, hPen);

	MoveToEx(mDC, x1, y1, NULL);
	LineTo(mDC, x2, y2);



	hPen = (HPEN)SelectObject(mDC, oldpen); // 새로운 펜 선택하기
	DeleteObject(hPen);
}
void Draw::Triangle(HDC mDC, float midbottom_x, float midbottom_y, float height)
{
	POINT point[] = {
		{midbottom_x - height / sqrt(3), midbottom_y},
		{midbottom_x + height / sqrt(3), midbottom_y},
		{midbottom_x, midbottom_y - height}
	};
	Polygon(mDC, point, 3);
}
void Draw::MakeTriangle(HDC mDC, float x1, float y1, float x2, float y2, COLORREF incolor, int outlinethickness, COLORREF outcolor)
{
	HBRUSH hBrush = CreateSolidBrush(incolor);
	HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush); // 새로운 브러시 선택하기
	HPEN hPen = CreatePen(PS_SOLID, outlinethickness, outcolor);
	HPEN oldpen = (HPEN)SelectObject(mDC, hPen);

	float height = y2 - y1;

	Triangle(mDC,x1 + (x2 - x1) / 2, y2, height);

	hBrush = (HBRUSH)SelectObject(mDC, oldBrush); // 새로운 펜 선택하기
	DeleteObject(hBrush);
	hPen = (HPEN)SelectObject(mDC, oldpen); // 새로운 펜 선택하기
	DeleteObject(hPen);
}
void Draw::MakeRectangle(HDC mDC, float x1, float y1, float x2, float y2, COLORREF incolor, int outlinethickness, COLORREF outcolor)
{
	HBRUSH hBrush = CreateSolidBrush(incolor);
	HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush); // 새로운 브러시 선택하기
	HPEN hPen = CreatePen(PS_SOLID, outlinethickness, outcolor);
	HPEN oldpen = (HPEN)SelectObject(mDC, hPen);

	POINT point[] = {
		{x1, y1},
		{x1, y2},
		{x2, y2},
		{x2, y1}
	};
	Polygon(mDC, point, 4);

	hBrush = (HBRUSH)SelectObject(mDC, oldBrush); // 새로운 펜 선택하기
	DeleteObject(hBrush);
	hPen = (HPEN)SelectObject(mDC, oldpen); // 새로운 펜 선택하기
	DeleteObject(hPen);
}