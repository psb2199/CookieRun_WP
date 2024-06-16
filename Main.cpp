#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <fstream>
#include "Grobal.h"
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include "MyH.h"
#include "Draw.h"
#include "ObjectManager.h"
#include "ImageLoader.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"CookieRun";


LPCSTR soundFile = "C:\\CookieRun_WP\\bgm_login.wav";
LPCSTR soundFile2 = "C:\\CookieRun_WP\\bgm_ingame.wav";


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

void Initialize();
void DrawObject(HDC mdc, HDC hDC);
void TickEvent();
void CheckCollision(Object* obj);
void MagnetMode(Object* obj);
void KeyDownEvents(HWND hWnd, WPARAM wParam);
void KeyUpEvents(HWND hWnd, WPARAM wParam);

int CookieType{ AngelCookie };

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
void DragMenu(LPARAM lParam);
void SelectMenu(LPARAM lParam);
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
Draw draw;
Object obj;

int Score{ 0 };
TCHAR buffer1[80];
TCHAR buffer2[80];

Object* Player;
bool DeBugMode{ false };
void PlayerHandler();
void MakeBridge();
void MakeBackGround1();
void MakeBackGround2();
void MakeObstacles();
void MakeCoin();
void MakeJelly();
void MakeEffect();
void CheckFalling();
void TextOutScoreandCoin(HDC mdc);
void MakeGrid(HDC mdc);
RECT Grid[28][28];
int jellytype{ MagnetItem };
int move_x{};

struct Board {
	int x1;
	int y1;
	int x2;
	int y2;
	int realX;
	int realY;
};

Board board[28][28];

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
		SetTimer(hWnd, 2, 100, NULL);
		SetTimer(hWnd, 3, 1, NULL);
		break;

	case WM_CHAR:
		break;

	case WM_KEYDOWN:
		if (Player->PlayMode && !KEY.down) KeyDownEvents(hWnd, wParam);
		break;

	case WM_KEYUP:
		if (Player->PlayMode) KeyUpEvents(hWnd, wParam);
		break;

	case WM_LBUTTONDOWN:
		//MouseLeftDownEvent(lParam);
		SelectMenu(lParam);
		InvalidateRect(hWnd, NULL, false);
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

		if (Player->PlayMode == false) {
			DragMenu(lParam);
			InvalidateRect(hWnd, NULL, false);
		}
		break;

	case WM_PAINT: {
		hDC = GetDC(hWnd);
		hBitmap = CreateCompatibleBitmap(hDC, WINDOW_WIDTH, WINDOW_HEIGHT);
		mDC = CreateCompatibleDC(hDC);
		(HBITMAP)SelectObject(mDC, hBitmap);

		DrawObject(mDC, hDC);
		//MakeGrid(mDC);

		if (Player->StartBackGround == false)
			TextOutScoreandCoin(mDC);

		BitBlt(hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, mDC, 0, 0, SRCCOPY);
		TransparentBlt(hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, mDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(0, 0, 0));

		DeleteObject(hBitmap);
		DeleteDC(mDC);
		DeleteDC(hDC);

		EndPaint(hWnd, &ps);

	}
				 break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			if (Player->EndObjectMove == true)
			{
				Player->AddObjectMovement(10, 0);
				TickEvent();

				if (Player->CollisionBox.left < WINDOW_WIDTH)
				{
					Player->PlayMode = false;
					Player->GameEnd = true;
				}
			}

			if (Player->PlayMode) {
				GameTime += DELTA_TIME;
				TickEvent();
				MakeEffect();
				CheckFalling();
			}

			if (Player->CollisionBox.bottom > WINDOW_HEIGHT - 20)
			{
				Player->ani_state = ANI_die;
			}
			break;
		case 2:
			if (Player->hp > 0 && Player->InvincibilityMode == false && Player->PlayMode)
				Player->hp -= 0.1;
			break;
		case 3:
			if (Player->FastMode && Player->PlayMode) {
				ObjectMgr.AddObject(FastEffect, ImageL.I_FastEffect, 1, 50, Player->pos_y);
				if (Player->CollisionBox.left < WINDOW_WIDTH && Player->PlayMode == false)
				{
					Player->GameEnd = true;
				}
			}
			break;
		}

		MakeEffect();
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
	PlaySoundA("bgm_login.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
	ObjectMgr.DeleteAll();
	ImageL.~ImageLoader();
	//ObjectMgr.~ObjectManager();
	ImageL.LoadAllImage();
	float width = WINDOW_WIDTH / 28;
	float height = WINDOW_HEIGHT / 20;

	for (int i = 0; i < 28; ++i) {
		for (int j = 0; j < 28; ++j) {
			Grid[i][j] = { i * (int)width ,j * (int)height ,i * (int)width + (int)width,j * (int)height + (int)height };
			board[i][j].realX = Grid[i][j].left;
		}
	}

	MakeBackGround2();
	MakeBackGround1();
	MakeBridge();
	MakeObstacles();
	MakeJelly();
	MakeCoin();
	ObjectMgr.AddObject(BackGround_Start, ImageL.I_BackGround_start, 1, 0, 0);
	ObjectMgr.AddObject(GameStart_dim, ImageL.I_Continue_dim, 1, 380, 500);
	ObjectMgr.AddObject(GameStart_no, ImageL.I_Continue_no, 1, 380, 500);
	Player = ObjectMgr.AddObject(Cookie, ImageL.I_AngelCookie, 1, 200, 350);
	ObjectMgr.AddObject(LifeBar1, ImageL.I_LifeBar1, 1, 40, 32);
	ObjectMgr.AddObject(LifeBar2, ImageL.I_LifeBar2, 1, 40, 32);
	ObjectMgr.AddObject(Heart, ImageL.I_Heart, 1, 28, 18);
	ObjectMgr.AddObject(Pause, ImageL.I_Pause, 1, 920, 30);
	ObjectMgr.AddObject(Coin_Ikon, ImageL.I_SilverCoin, 1, 500, 30);
	ObjectMgr.AddObject(Continue_dim, ImageL.I_Continue_dim, 1, 360, 220);
	ObjectMgr.AddObject(Continue_no, ImageL.I_Continue_no, 1, 360, 220);
	ObjectMgr.AddObject(Exit_dim, ImageL.I_Exit_dim, 1, 367, 317);
	ObjectMgr.AddObject(Exit_no, ImageL.I_Exit_no, 1, 367, 320);
	ObjectMgr.AddObject(ScoreBoard, ImageL.I_ScoreBoard, 1, 300, 170);
	ObjectMgr.AddObject(Playagain_dim, ImageL.I_Playagain_dim, 1, 370, 370);
	ObjectMgr.AddObject(Playagain_no, ImageL.I_Playagain_no, 1, 370, 370);
	ObjectMgr.AddObject(EndGame_dim, ImageL.I_EndGame_dim, 1, 540, 370);
	ObjectMgr.AddObject(EndGame_no, ImageL.I_EndGame_no, 1, 540, 370);
}

void MakeGrid(HDC mdc) {
	float width = WINDOW_WIDTH / 28;
	float height = WINDOW_HEIGHT / 20;

	for (int i = 0; i < 28 + 1; ++i) {
		draw.MakeLine(mdc, 0, i * height, WINDOW_WIDTH, i * height, RGB(0, 0, 0), 1, PS_SOLID);
		draw.MakeLine(mdc, i * width, 0, i * width, WINDOW_HEIGHT, RGB(0, 0, 0), 1, PS_SOLID);
	}

}


void DrawObject(HDC mdc, HDC hDC) {
	ObjectMgr.DrawAll(mdc, hDC);

}

void TickEvent() {

	//if (Player->FastMode == true) {
	//	ObjectMgr.AddObject(FastEffect, ImageL.I_FastEffect, 1, 50, 350);

	//}

	Object* ptr = ObjectMgr.GetAllObjects();

	while (ptr != nullptr)
	{
		if (ptr->pos_x > -1000)
		{
			ptr->TickEvents();
			if (ptr->pos_x > 0 && ptr->pos_x < WINDOW_WIDTH) CheckCollision(ptr);
			if (Player->MagnetMode == true) MagnetMode(ptr);
			ptr->SetDebugMode(DeBugMode);
			ptr->m_ElapseTime += DELTA_TIME;
		}
		ptr = ptr->next;
	}


	PlayerHandler();
}

void PlayerHandler()
{
	float speed{ 10.0 };
	//if (KEY.left) Player->AddObjectMovement(-speed, 0);
	//if (KEY.right) Player->AddObjectMovement(speed, 0);
	if (KEY.keyJ) Player->AddObjectMovement(0, -speed);
	if (KEY.down)
	{
		Player->ani_state = ANI_sliding;
	}
	if (Player->isJumping == true) Player->DoJump();
}

void MakeBridge()
{
	std::ifstream ifs;
	ifs.open("Bridge.txt");
	int pos_x{};

	while (ifs >> pos_x) {
		ObjectMgr.AddObject(Bridge, ImageL.I_Bridge1, 1, pos_x, 550);

	}
}

void MakeObstacles()
{
	std::ifstream ifs;
	ifs.open("Sliding1.txt");
	int pos_x{};
	int pos_y{};

	while (ifs >> pos_x) {
		ObjectMgr.AddObject(Obstacle_S, ImageL.I_Sd1, 1, pos_x - 70, 170);
	}

	ifs.close();
	ifs.clear();

	ifs.open("Jump2_Green.txt");

	while (ifs >> pos_x) {
		ObjectMgr.AddObject(Obstacle_J, ImageL.I_jp2A, 1, pos_x, 340);
	}

	ifs.close();
	ifs.clear();

	ifs.open("Jump2_Pink.txt");

	while (ifs >> pos_x) {
		ObjectMgr.AddObject(Obstacle_J, ImageL.I_jp2B, 1, pos_x, 340);
	}

	ifs.close();
	ifs.clear();

	ifs.open("Jump1.txt");

	while (ifs >> pos_x) {
		ObjectMgr.AddObject(Obstacle_J, ImageL.I_jp1A, 1, pos_x, 410);
	}

	ifs.close();
	ifs.clear();

}

void MakeCoin()
{
	std::ifstream ifs;
	ifs.open("make_big_coin.txt");
	int pos_x{};
	int pos_y{};

	while (ifs >> pos_x >> pos_y) {
		ObjectMgr.AddObject(Coin_B, ImageL.I_BigCoin, 1, pos_x, pos_y);
	}

	ifs.close();
	ifs.clear();

	ifs.open("make_silver_coin.txt");

	while (ifs >> pos_x >> pos_y) {
		ObjectMgr.AddObject(Coin_S, ImageL.I_SilverCoin, 1, pos_x, pos_y);
	}

	ifs.close();
	ifs.clear();

	ifs.open("make_gold_coin.txt");

	while (ifs >> pos_x >> pos_y) {
		ObjectMgr.AddObject(Coin_G, ImageL.I_GoldCoin, 1, pos_x, pos_y);
	}

	ifs.close();
	ifs.clear();
}

void MakeJelly()
{
	std::ifstream ifs;
	int pos_x{};
	int pos_y{};

	ifs.open("make_general_jelly.txt");

	while (ifs >> pos_x >> pos_y) {
		ObjectMgr.AddObject(GeneralJelly, ImageL.I_CommonJelly, 1, pos_x, pos_y);
	}

	ifs.close();
	ifs.clear();

	ifs.open("make_yellow_bear_jelly.txt");

	while (ifs >> pos_x >> pos_y) {
		ObjectMgr.AddObject(YellowJelly, ImageL.I_YellowBear, 1, pos_x, pos_y);
	}

	ifs.close();
	ifs.clear();

	ifs.open("make_big_bear_jelly.txt");

	while (ifs >> pos_x >> pos_y) {
		ObjectMgr.AddObject(BigJelly, ImageL.I_BigBear, 1, pos_x, pos_y);
	}

	ifs.close();
	ifs.clear();

	ifs.open("make_pink_bear_jelly.txt");

	while (ifs >> pos_x >> pos_y) {
		ObjectMgr.AddObject(PinkJelly, ImageL.I_PinkBear, 1, pos_x, pos_y);
	}

	ifs.close();
	ifs.clear();

	ifs.open("energy.txt");

	while (ifs >> pos_x >> pos_y) {
		ObjectMgr.AddObject(Energy, ImageL.I_Energyitem, 1, pos_x, pos_y);
	}

	ifs.close();
	ifs.clear();

	ObjectMgr.AddObject(Big, ImageL.I_Bigitem, 1, 2653, 360);
	ObjectMgr.AddObject(Big, ImageL.I_Bigitem, 1, 34037, 390);

	ObjectMgr.AddObject(Fast, ImageL.I_Fastitem, 1, 8539, 360);
	ObjectMgr.AddObject(Fast, ImageL.I_Fastitem, 1, 13281, 360);
	ObjectMgr.AddObject(Fast, ImageL.I_Fastitem, 1, 15126, 390);
	ObjectMgr.AddObject(Fast, ImageL.I_Fastitem, 1, 17964, 330);

	ObjectMgr.AddObject(Magnet, ImageL.I_Magnetitem, 1, 2669, 390);
	ObjectMgr.AddObject(Magnet, ImageL.I_Magnetitem, 1, 9288, 390);
	ObjectMgr.AddObject(Magnet, ImageL.I_Magnetitem, 1, 13119, 360);
	ObjectMgr.AddObject(Magnet, ImageL.I_Magnetitem, 1, 17322, 360);
}

void MakeEffect()
{
	Object* ptr = ObjectMgr.GetAllObjects();

	while (ptr != nullptr)
	{
		if (ptr->type == FastEffect)
		{
			ptr->AddObjectMovement(-10, 0);
		}

		ptr = ptr->next;
	}
}

void CheckFalling()
{
	Object* ptr = ObjectMgr.GetAllObjects();
	bool CheckFalling{ false };

	while (ptr != nullptr)
	{
		if (ptr->type == Bridge && ((Player->pos_x<ptr->CollisionBox.right + 50 && Player->pos_x > ptr->CollisionBox.left - 50)))
		{
			CheckFalling = true;
		}

		ptr = ptr->next;
	}

	if (CheckFalling == false && Player->pos_y >= Player->original_y && (Player->BigMode == false) && (Player->FastMode == false) && (Player->InvincibilityMode == false))
	{
		//Player->ani_state = ANI_die;
		Player->AddObjectMovement(0, 70);
		//Player->PlayMode = false;
	}
}

void TextOutScoreandCoin(HDC mDC)
{
	AddFontResource(TEXT("CookieRun Bold.ttf"));
	HFONT currentFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("CookieRun Bold"));
	HFONT oldFont = (HFONT)SelectObject(mDC, currentFont);
	SetBkMode(mDC, TRANSPARENT);
	SetTextColor(mDC, RGB(255, 255, 255));
	wsprintf(buffer1, L"score  %d", Player->score);
	TextOut(mDC, 700, 30, buffer1, lstrlen(buffer1));
	wsprintf(buffer2, L"%d", Player->coin);
	TextOut(mDC, 560, 30, buffer2, lstrlen(buffer2));
	SelectObject(mDC, oldFont);
	DeleteObject(currentFont);

	if (Player->PlayMode == false && Player->GameEnd == true)
	{
		AddFontResource(TEXT("CookieRun Bold.ttf"));
		currentFont = CreateFont(70, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("CookieRun Bold"));
		oldFont = (HFONT)SelectObject(mDC, currentFont);
		SetBkMode(mDC, TRANSPARENT);
		SetTextColor(mDC, RGB(0, 0, 0));
		//TextOut(mDC, 448, 190, L"score", lstrlen(L"score"));
		//TextOut(mDC, 452, 190, L"score", lstrlen(L"score"));
		//TextOut(mDC, 450, 192, L"score", lstrlen(L"score"));
		//TextOut(mDC, 450, 188, L"score", lstrlen(L"score"));
		//SetTextColor(mDC, RGB(255, 255, 255));
		TextOut(mDC, 450, 190, L"score", lstrlen(L"score"));
		SelectObject(mDC, oldFont);
		DeleteObject(currentFont);
		currentFont = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("CookieRun Bold"));
		oldFont = (HFONT)SelectObject(mDC, currentFont);
		SetTextColor(mDC, RGB(0, 0, 0));
		wsprintf(buffer1, L"%d", Player->score);
		TextOut(mDC, 458, 265, buffer1, lstrlen(buffer1));
		TextOut(mDC, 462, 265, buffer1, lstrlen(buffer1));
		TextOut(mDC, 460, 267, buffer1, lstrlen(buffer1));
		TextOut(mDC, 460, 263, buffer1, lstrlen(buffer1));
		wsprintf(buffer2, L"%d", Player->coin);
		TextOut(mDC, 482, 325, buffer2, lstrlen(buffer2));
		TextOut(mDC, 478, 325, buffer2, lstrlen(buffer2));
		TextOut(mDC, 480, 323, buffer2, lstrlen(buffer2));
		TextOut(mDC, 480, 327, buffer2, lstrlen(buffer2));
		SetTextColor(mDC, RGB(255, 255, 255));
		wsprintf(buffer1, L"%d", Player->score);
		TextOut(mDC, 460, 265, buffer1, lstrlen(buffer1));
		wsprintf(buffer2, L"%d", Player->coin);
		TextOut(mDC, 480, 325, buffer2, lstrlen(buffer2));
		SelectObject(mDC, oldFont);
		DeleteObject(currentFont);
	}
}

void MakeBackGround1()
{
	float h = ImageL.I_BackGround.GetHeight();
	float ratio = WINDOW_HEIGHT / h;
	for (int i = 0; i < 20; ++i)
	{
		if (i == 19)
			ObjectMgr.AddObject(Background_Last, ImageL.I_BackGround, 1, WINDOW_WIDTH * ratio * i, 0);
		else
			ObjectMgr.AddObject(Background, ImageL.I_BackGround, 1, WINDOW_WIDTH * ratio * i, 0);
	}
}

void MakeBackGround2()
{
	float h = ImageL.I_BackGround2.GetHeight();
	float ratio = WINDOW_HEIGHT / h;
	for (int i = 0; i < 11; ++i)
		ObjectMgr.AddObject(Background2, ImageL.I_BackGround2, 1, WINDOW_WIDTH * ratio * i, 0);
}


void KeyDownEvents(HWND hWnd, WPARAM wParam) {
	Object* ptr = ObjectMgr.GetAllObjects();

	switch (wParam) {
	case '1':
		jellytype = General;
		break;
	case '2':
		jellytype = YellowBear;
		break;
	case '3':
		jellytype = PinkBear;
		break;
	case '4':
		jellytype = BigBear;
		break;
	case '5':
		jellytype = SilverCoin;
		break;
	case '6':
		jellytype = GoldCoin;
		break;
	case '7':
		jellytype = BigCoin;
		break;
	case '8':
		jellytype = BigItem;
		break;
	case '9':
		jellytype = FastItem;
		break;
	case '0':
		jellytype = EnergyItem;
		break;
	case VK_UP:
		KEY.up = true;
		break;

	case VK_DOWN:
		KEY.down = true;
		break;

	case VK_LEFT:
		TickEvent();
		for (int i = 0; i < 28; ++i) {
			for (int j = 0; j < 28; ++j) {
				board[i][j].realX += 6;
			}
		}
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

	case 'J':
	case 'j':
		if (DeBugMode) DeBugMode = false;
		else DeBugMode = true;
		break;

	case 'P':
	case 'p':
		Player->PlayMode = !Player->PlayMode;
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

void DragMenu(LPARAM lParam)
{
	MOUSE.left_click = true;
	MOUSE.x = LOWORD(lParam);
	MOUSE.y = HIWORD(lParam);

	Object* ptr = ObjectMgr.GetAllObjects();

	while (ptr != nullptr)
	{
		if (ptr->CollisionBox.left <= MOUSE.x && ptr->CollisionBox.right >= MOUSE.x &&
			ptr->CollisionBox.top <= MOUSE.y && ptr->CollisionBox.bottom >= MOUSE.y)
		{
			switch (ptr->type)
			{
			case Continue_no:
				Player->ClickContinue = true;
				break;
			case Exit_no:
				Player->ClickExit = true;
				break;
			case GameStart_no:
				Player->ClickStart = true;
				break;
			case EndGame_no:
				Player->ClickEnd = true;
				break;
			case Playagain_no:
				Player->ClickAgain = true;
				break;
			}
		}
		else
		{
			switch (ptr->type)
			{
			case Continue_no:
				Player->ClickContinue = false;
				break;
			case Exit_no:
				Player->ClickExit = false;
				break;
			case GameStart_no:
				Player->ClickStart = false;
				break;
			case EndGame_no:
				Player->ClickEnd = false;
				break;
			case Playagain_no:
				Player->ClickAgain = false;
				break;
			}
		}
		ptr = ptr->next;
	}
}

void SelectMenu(LPARAM lParam)
{

	MOUSE.left_click = true;
	MOUSE.x = LOWORD(lParam);
	MOUSE.y = HIWORD(lParam);

	Object* ptr = ObjectMgr.GetAllObjects();

	while (ptr != nullptr)
	{
		if (ptr->CollisionBox.left <= MOUSE.x && ptr->CollisionBox.right >= MOUSE.x &&
			ptr->CollisionBox.top <= MOUSE.y && ptr->CollisionBox.bottom >= MOUSE.y)
		{
			switch (ptr->type)
			{
			case Pause:
				PlaySoundA(nullptr, nullptr, 0);
				Player->PlayMode = false;
				break;
			case Continue_dim:
				PlaySoundA(nullptr, nullptr, 0);
				PlaySoundA("bgm_ingame.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
				Player->PlayMode = true;
				break;
			case Exit_dim:
				PlaySoundA(nullptr, nullptr, 0);
				PlaySoundA("bgm_login.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
				Initialize();
				return;
				break;
			case GameStart_dim:
				Player->PlayMode = true;
				Player->StartBackGround = false;
				if (Player->PlayMode)
					PlaySoundA("bgm_ingame.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
				break;
			case EndGame_dim:
				PlaySoundA(nullptr, nullptr, 0);
				PlaySoundA("bgm_login.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
				Initialize();
				return;
				break;
			case Playagain_dim:
				PlaySoundA(nullptr, nullptr, 0);
				PlaySoundA("bgm_ingame.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
				Initialize();
				Player->PlayMode = true;
				Player->StartBackGround = false;
				return;
				break;
			}
		}
		ptr = ptr->next;
	}
}

void MouseLeftDownEvent(LPARAM lParam) {

	MOUSE.left_click = true;
	MOUSE.x = LOWORD(lParam);
	MOUSE.y = HIWORD(lParam);

	float width = WINDOW_WIDTH / 28;

	std::ofstream ofs;
	ofs.open("general_jelly.txt", std::ios::app);

	float pos_x1{};
	int pos_y1{};

	float pos_x2{};
	int pos_y2{};

	float pos_x3{};
	int pos_y3{};

	for (int i = 0; i < 28; ++i) {
		for (int j = 0; j < 28; ++j) {
			if (Grid[i][j].left <= MOUSE.x && Grid[i][j].top <= MOUSE.y &&
				Grid[i][j].right >= MOUSE.x && Grid[i][j].bottom >= MOUSE.y)
				switch (jellytype) {
				case General:
					ObjectMgr.AddObject(Jelly, ImageL.I_CommonJelly, 1, Grid[i][j].left, Grid[i][j].top);
					break;
				case YellowBear:
					ObjectMgr.AddObject(Jelly, ImageL.I_YellowBear, 1, Grid[i][j].left, Grid[i][j].top);
					break;
				case PinkBear:
					ObjectMgr.AddObject(Jelly, ImageL.I_PinkBear, 1, Grid[i][j].left, Grid[i][j].top);
					break;
				case BigBear:
					ObjectMgr.AddObject(Jelly, ImageL.I_BigBear, 1, Grid[i][j].left, Grid[i][j].top);
					break;
				case SilverCoin:
					ObjectMgr.AddObject(Coin, ImageL.I_SilverCoin, 1, Grid[i][j].left, Grid[i][j].top);
					break;
				case GoldCoin:
					ObjectMgr.AddObject(Coin, ImageL.I_GoldCoin, 1, Grid[i][j].left, Grid[i][j].top);
					break;
				case BigCoin:
					ObjectMgr.AddObject(Coin, ImageL.I_BigCoin, 1, Grid[i][j].left, Grid[i][j].top);
					break;
				case BigItem:
					ObjectMgr.AddObject(Big, ImageL.I_Bigitem, 1, Grid[i][j].left, Grid[i][j].top);
					break;
				case FastItem:
					ObjectMgr.AddObject(Fast, ImageL.I_Fastitem, 1, Grid[i][j].left, Grid[i][j].top);
					break;
				case EnergyItem:
					ObjectMgr.AddObject(Energy, ImageL.I_Energyitem, 1, Grid[i][j].left, Grid[i][j].top);
					break;
				case MagnetItem:
					ObjectMgr.AddObject(Magnet, ImageL.I_Magnetitem, 1, Grid[i][j].left, Grid[i][j].top);
					pos_x1 = board[i][j].realX;
					pos_y1 = Grid[i][j].top;
					ofs << pos_x1 << " " << pos_y1 << '\n';
					break;
				}
		}
	}

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

void MagnetMode(Object* obj)
{

	if (obj->type == PinkJelly || obj->type == YellowJelly || obj->type == BigJelly || obj->type == GeneralJelly
		|| obj->type == Coin_S || obj->type == Coin_B || obj->type == Coin_G ||
		obj->type == Big || obj->type == Fast || obj->type == Energy && obj != Player)
	{

		if (obj->CollisionBox.right - 500 < Player->CollisionBox.right && obj->CollisionBox.top < Player->CollisionBox.top)
		{
			obj->AddObjectMovement(-5, 15);
		}
		else if (obj->CollisionBox.right - 500 < Player->CollisionBox.right && obj->CollisionBox.bottom > Player->CollisionBox.bottom)
		{
			obj->AddObjectMovement(-7, -8);
		}
		else if (obj->CollisionBox.right - 500 < Player->CollisionBox.right && obj->CollisionBox.top > Player->CollisionBox.top)
		{
			obj->AddObjectMovement(-15, 0);
		}

		if (obj->CollisionBox.right < Player->CollisionBox.left && obj->CollisionBox.top < Player->CollisionBox.top)
		{
			obj->AddObjectMovement(50, 5);
		}

	}

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
