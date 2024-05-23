#include "MyH.h"
#include <iostream>
#include <random>
#include <Windows.h>

#define PI 3.14159265359

MyH::MyH()
{
}

MyH::~MyH()
{
}

int MyH::Randint(int start, int end)
{
    std::random_device rd;

    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(start, end);

    return dis(gen);
}

float MyH::Radian(float degree)
{
    return degree * PI / 180;
}

float MyH::Degree(float Radian)
{
    return Radian * 180 / PI;
}

float MyH::GetLookAtDirection(float Obj_x, float Obj_y, float LookObj_x, float LookObj_y)
{
    float del_x = LookObj_x - Obj_x;
    float del_y = LookObj_y - Obj_y;

    return Degree(atan2(del_y, del_x));
}

float MyH::GetDistance(float Obj1_x, float Obj1_y, float Obj2_x, float Obj2_y)
{
    float del_x = Obj2_x - Obj1_x;
    float del_y = Obj2_y - Obj1_y;
    return sqrt(del_x * del_x + del_y * del_y);
}

