#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <string>

#include "MyH.h"
#include "Draw.h"
#include "ObjectManager.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

void Initialize();
void DrawObject(HDC mdc);
void UpdateObjects();
void KeyDownEvents(HWND hWnd, WPARAM wParam);
void KeyUpEvents(HWND hWnd, WPARAM wParam);
struct KeyEventFlag {
	bool up{ false };
	bool down{ false };
	bool right{ false };
	bool left{ false };
	bool KeyPlus{ false };
	bool KeySubtract{ false };
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

#define DELTA_TIME 0.008

int board_size = 20;
void MakeBoard(float gap);

ObjectManager ObjectMgr;


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
		//hDC = BeginPaint(hWnd, &ps);

		//// 오프스크린 버퍼를 위한 메모리 DC 및 비트맵 생성
		//offScreenDC = CreateCompatibleDC(hDC);
		//offScreenBitmap = CreateCompatibleBitmap(hDC, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top);
		//oldBitmap = (HBITMAP)SelectObject(offScreenDC, offScreenBitmap);

		//// 배경 지우기
		//FillRect(offScreenDC, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		//// 오프스크린 버퍼에 그리기 작업 수행
		//mDC = CreateCompatibleDC(offScreenDC);
		//SelectObject(mDC, hBitmap);

		//StretchBlt(offScreenDC, 0, 0, bitmapX, bitmapY, mDC, 0, 0, 274, 184, SRCCOPY);
		//StretchBlt(offScreenDC, bitmapX, 0, WINDOW_WIDTH - bitmapX, bitmapY, mDC, 0, 0, 274, 184, SRCCOPY);
		//StretchBlt(offScreenDC, 0, bitmapY, bitmapX, WINDOW_HEIGHT - bitmapY, mDC, 0, 0, 274, 184, SRCCOPY);
		//StretchBlt(offScreenDC, bitmapX, bitmapY, WINDOW_WIDTH - bitmapX, WINDOW_HEIGHT - bitmapY, mDC, 0, 0, 274, 184, SRCCOPY);
		//DeleteDC(mDC);

		//// 추가적인 그리기 작업
		////DrawObject(offScreenDC);

		//// 오프스크린 버퍼를 화면에 복사
		//BitBlt(hDC, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, offScreenDC, 0, 0, SRCCOPY);

		//// 오프스크린 버퍼 및 메모리 DC 정리
		//SelectObject(offScreenDC, oldBitmap);
		//DeleteObject(offScreenBitmap);
		//DeleteDC(offScreenDC);

		//EndPaint(hWnd, &ps);
		hDC = GetDC(hWnd);
		hBitmap = CreateCompatibleBitmap(hDC, WINDOW_WIDTH, WINDOW_HEIGHT);
		mDC = CreateCompatibleDC(hDC);
		(HBITMAP)SelectObject(mDC, hBitmap);

		DrawObject(mDC);

		BitBlt(hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, mDC, 0, 0, SRCCOPY);

		TransparentBlt(hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, mDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(0, 0, 0));

		InvalidateRect(hWnd, NULL, FALSE);

		DeleteDC(mDC);
		DeleteDC(hDC);
		ReleaseDC(hWnd, hDC);

		break;

	case WM_TIMER:
		GameTime += DELTA_TIME;
		UpdateObjects();
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
	ObjectMgr.AddObject("d", 0, 0, 0, 0, L"Test.bmp")->SetObjectVertexLocation(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	

}

void DrawObject(HDC mdc) {
	ObjectMgr.DrawAll(mdc);
}

void UpdateObjects() {
	Object* ptr = ObjectMgr.GetAllObjects();

	while (ptr != nullptr)
	{

		ptr = ptr->next;
	}

}

void KeyDownEvents(HWND hWnd, WPARAM wParam) {
	Object* ptr = ObjectMgr.GetAllObjects();

	switch (wParam) {

	case VK_UP:
		KEY.up = true;
		break;
	case VK_DOWN:
		KEY.down = true;
		break;
	case VK_LEFT:
		KEY.left = true;
		break;
	case VK_RIGHT:
		KEY.right = true;
		break;

	case VK_ADD:
		KEY.KeyPlus = true;
		break;
	case VK_SUBTRACT:
		KEY.KeySubtract = true;
		break;

	case 'A':
	case 'a':
	break;

	case 'R':
	case 'r':
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
		KEY.down = false;
		break;
	case VK_LEFT:
		KEY.left = false;
		break;
	case VK_RIGHT:
		KEY.right = false;
		break;
	case VK_ADD:
		KEY.KeyPlus = false;
		break;
	case VK_SUBTRACT:
		KEY.KeySubtract = false;
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
