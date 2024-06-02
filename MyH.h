#pragma once
class MyH
{

private:
	MyH();
	~MyH();

public:
	static int Randint(int start, int end);
	static float Radian(float degree);
	static float Degree(float Radian);
	static float fract(float value);

	static float GetLookAtDirection(float Obj_x, float Obj_y, float LookObj_x, float LookObj_y);
	static float GetDistance(float Obj1_x, float Obj1_y, float Obj2_x, float Obj2_y);
};


