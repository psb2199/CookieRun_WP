#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <fstream>
#include "Grobal.h"

#include "MyH.h"
#include "Draw.h"
#include "ObjectManager.h"
#include "ImageLoader.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"CookieRun";


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

void Initialize();
void DrawObject(HDC mdc);
void TickEvent();
void CheckCollision(Object* obj);
void KeyDownEvents(HWND hWnd, WPARAM wParam);
void KeyUpEvents(HWND hWnd, WPARAM wParam);
struct KeyEventFlag {
	bool up{ false };
	bool down{ false };
	bool right{ false };
	bool left{ false };
	bool enter{ false };
	bool keyJ{ false };
	bool KeyPlus{ false };
	bool KeySubtract{ false };
	bool KeySpace{ false };
};
KeyEventFlag KEY;
void MouseLeftDownEvent(LPARAM lParam);
void MouseLeftUpEvent(LPARAM lParam);
void MouseRightDownEvent(LPARAM lParam);
void MouseRightUpEvent(LPARAM lParam);
struct MouseEventFlag {
	bool left_click{ false };
	bool right_click{ false };
	float x{ 0 };
	float y{ 0 };
	float selection_range{ 5.f };
};
MouseEventFlag MOUSE;

ImageLoader ImageL;
ObjectManager ObjectMgr;

Object* Player;
bool DeBugMode{ false };
void PlayerHandler();
void MakeBridge();
void MakeBackGround1();
void MakeBackGround2();
void MakeObstacles();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WindowClass;

	g_hInst = hInstance;

	WindowClass.cbSize = sizeof(WindowClass);
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = (WNDPROC)WndProc;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = hInstance;
	WindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WindowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = lpszClass;
	WindowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WindowClass);

	RECT windowRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	static float GameTime{ 0.f };
	HDC offScreenDC;
	HBITMAP offScreenBitmap;
	HBITMAP oldBitmap;

	HDC hDC, mDC;
	static HBITMAP hBitmap;

	switch (uMsg) {
	case WM_CREATE:
		Initialize();
		SetTimer(hWnd, 1, DELTA_TIME * 1000, NULL);
		break;

	case WM_KEYDOWN:
		KeyDownEvents(hWnd, wParam);
		break;

	case WM_KEYUP:
		KeyUpEvents(hWnd, wParam);
		break;

	case WM_LBUTTONDOWN:
		MouseLeftDownEvent(lParam);
		break;
	case WM_LBUTTONUP:
		MouseLeftUpEvent(lParam);
		break;
	case WM_RBUTTONDOWN:
		MouseRightDownEvent(lParam);
		break;
	case WM_RBUTTONUP:
		MouseRightUpEvent(lParam);
		break;
	case WM_MOUSEMOVE:
		if (MOUSE.left_click) {
			MOUSE.x = LOWORD(lParam);
			MOUSE.y = HIWORD(lParam);
		}
		break;

	case WM_PAINT:
		hDC = GetDC(hWnd);
		hBitmap = CreateCompatibleBitmap(hDC, WINDOW_WIDTH, WINDOW_HEIGHT);
		mDC = CreateCompatibleDC(hDC);
		(HBITMAP)SelectObject(mDC, hBitmap);

		DrawObject(mDC);

		BitBlt(hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, mDC, 0, 0, SRCCOPY);
		TransparentBlt(hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, mDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(0, 0, 0));

		DeleteObject(hBitmap);
		DeleteDC(mDC);
		DeleteDC(hDC);

		EndPaint(hWnd, &ps);

		break;

	case WM_TIMER:
		GameTime += DELTA_TIME;
		TickEvent();
		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_DESTROY:
		DeleteObject(hBitmap);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Initialize() {
	ObjectMgr.DeleteAll();
	ImageL.LoadAllImage();

	MakeBackGround2();
	MakeBackGround1();
	MakeObstacles();
	MakeBridge();
	Player = ObjectMgr.AddObject("Cookie", ImageL.I_AngelCookie, 1, 200, 350);

}


void DrawObject(HDC mdc) {
	ObjectMgr.DrawAll(mdc);
}

void TickEvent() {
	Object* ptr = ObjectMgr.GetAllObjects();

	while (ptr != nullptr)
	{
		ptr->TickEvents();
		CheckCollision(ptr);
		ptr->SetDebugMode(DeBugMode);
		ptr->m_ElapseTime += DELTA_TIME;
		ptr = ptr->next;
	}

	PlayerHandler();
}

void PlayerHandler()
{
	float speed{ 10.0 };
	if (KEY.left) Player->AddObjectMovement(-speed, 0);
	if (KEY.right) Player->AddObjectMovement(speed, 0);
	if (KEY.keyJ) Player->AddObjectMovement(0, -speed);
	// if (KEY.down) Player->AddObjectMovement(0, speed);
	if (Player->isJumping == true) Player->DoJump();
}

void MakeBridge()
{
	std::ifstream ifs;
	ifs.open("Bridge.txt");
	int pos_x{};
	while (ifs >> pos_x) {
		ObjectMgr.AddObject("Bridge", ImageL.I_Bridge1, 1, pos_x, 550);
		//ObjectMgr.AddObject("Jelly", ImageL.I_CommonJelly, 1, pos_x-30, 430);
		//ObjectMgr.AddObject("Jump", ImageL.I_jp1A, 1, pos_x - 30, 410);
		//ObjectMgr.AddObject("Jump", ImageL.I_jp2A, 1, pos_x - 40, 340);
		ObjectMgr.AddObject("Jump", ImageL.I_jp2B, 1, pos_x - 40, 340);
	}
}

void MakeObstacles()
{
	std::ifstream ifs;
	ifs.open("Sliding1.txt");
	int pos_x{};

	while (ifs >> pos_x) {
		ObjectMgr.AddObject("Sliding1", ImageL.I_Sd1, 1, pos_x, 0);
	}


}

void MakeBackGround1()
{
	float h = ImageL.I_BackGround.GetHeight();
	float ratio = WINDOW_HEIGHT / h;
	for (int i = 0; i < 20; ++i)
		ObjectMgr.AddObject("BackGround", ImageL.I_BackGround, 1, WINDOW_WIDTH * ratio * i, 0);
}

void MakeBackGround2()
{
	float h = ImageL.I_BackGround2.GetHeight();
	float ratio = WINDOW_HEIGHT / h;
	for (int i = 0; i < 7; ++i)
		ObjectMgr.AddObject("BackGround2", ImageL.I_BackGround2, 1, WINDOW_WIDTH * ratio * i, 0);
}


void KeyDownEvents(HWND hWnd, WPARAM wParam) {
	Object* ptr = ObjectMgr.GetAllObjects();

	switch (wParam) {

	case VK_UP:
		KEY.up = true;
		break;
	case 'J':
	case 'j':
		if (DeBugMode) DeBugMode = false;
		else DeBugMode = true;
		break;
	case VK_DOWN:
		//Player->m_ElapseTime = 0;
		Player->ani_state = ANI_sliding;
		break;
	case VK_LEFT:
		KEY.left = true;
		break;
	case VK_RIGHT:
		KEY.right = true;
		break;
	case VK_RETURN:
		KEY.enter = true;
		break;

	case VK_ADD:
		KEY.KeyPlus = true;
		break;
	case VK_SUBTRACT:
		KEY.KeySubtract = true;
		break;
	case VK_SPACE:
		KEY.KeySpace = true;
		Player->m_ElapseTime = 0;
		Player->isJumping = true;
		Player->ani_state = ANI_jumping;
		Player->count_jump += 1;
		break;


	case 'Q':
	case 'q':
		PostQuitMessage(0);
		break;
	}
	InvalidateRect(hWnd, NULL, false);
}

void KeyUpEvents(HWND hWnd, WPARAM wParam) {
	switch (wParam) {
	case VK_UP:
		KEY.up = false;
		break;
	case VK_DOWN:
		Player->m_ElapseTime = 0;
		Player->ani_state = ANI_sliding_up;
		break;
	case VK_LEFT:
		KEY.left = false;
		break;
	case VK_RIGHT:
		KEY.right = false;
		break;
	case VK_RETURN:
		KEY.enter = false;
		break;
	case 'J':
	case 'j':
		KEY.keyJ = false;
		break;


	case VK_ADD:
		KEY.KeyPlus = false;
		break;
	case VK_SUBTRACT:
		KEY.KeySubtract = false;
		break;
	case VK_SPACE:
		KEY.KeySpace = false;
		break;
	}
	InvalidateRect(hWnd, NULL, false);
}

void MouseLeftDownEvent(LPARAM lParam) {
	MOUSE.left_click = true;
	MOUSE.x = LOWORD(lParam);
	MOUSE.y = HIWORD(lParam);


}

void MouseLeftUpEvent(LPARAM lParam) {
	MOUSE.left_click = false;
}

void MouseRightDownEvent(LPARAM lParam) {
	MOUSE.right_click = true;
	MOUSE.x = LOWORD(lParam);
	MOUSE.y = HIWORD(lParam);
}

void MouseRightUpEvent(LPARAM lParam) {
	MOUSE.right_click = false;
}


void CheckCollision(Object* obj)
{
	Object* ptr = ObjectMgr.GetAllObjects();

	bool FlagCollision{ false };

	while (ptr != nullptr)
	{
		Object* nextptr = ptr->next;

		if (ptr != obj)
		{
			if (obj->CollisionBox.right > ptr->CollisionBox.left &&
				obj->CollisionBox.left < ptr->CollisionBox.right &&
				obj->CollisionBox.bottom > ptr->CollisionBox.top &&
				obj->CollisionBox.top < ptr->CollisionBox.bottom)
			{
				FlagCollision = true;
				obj->CollisionEvent(ptr);


			}
		}

		ptr = nextptr;
	}

	if (!FlagCollision) obj->CollisionEvent(nullptr);
}
