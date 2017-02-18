#pragma once

#include <Windows.h> // Include the Win32 API
#include <tchar.h> // Include support for UNICODE
#include <sstream> // Include wostringstream support
using std::wostringstream;

#include <d2d1.h> // Include the Direct2D API.
#pragma comment(lib, "d2d1.lib") // Connect to the Direct2D Import Library.

#include <dwrite.h> // Include the DirectWrite API.
#pragma comment(lib, "dwrite.lib") // Connect to the DirectWrite Import Library.

#include <mmsystem.h> // Include the Multimedia header file.
#pragma comment(lib, "winmm.lib") // Connect to the Multimedia import library.

#include <wincodec.h> // Include the Windows Imaging Component (WIC)
#pragma comment(lib, "windowscodecs.lib")

#pragma comment(lib, "fmodex_vc.lib")//Connect to the FMOD Import Library.
#include <fmod.h>//Include the FMOD API.
#include <fmod.hpp>//Include the FMOD API.

class Game
{
	// Window Handle:
	HWND hWnd;

// DirectX Helper Functions

	// The SafeRelease Pattern
	template <class T> void SafeRelease(T **ppT)
	{
		if (*ppT)
		{
			(*ppT)->Release();
			*ppT = NULL;
		}
	}

// Programmer defined macro to make using the known colors easier.
#define D2DColor(clr) D2D1::ColorF(D2D1::ColorF::clr)



	HRESULT CreateGraphics(HWND hWnd);
	void DestroyGraphics(void);
	HRESULT LoadBitmapFromFile(LPCTSTR strFileName, ID2D1Bitmap** ppBitmap );

	// DirectX Interfaces:
	ID2D1Factory* pD2DFactory;
	ID2D1HwndRenderTarget* pRT;
	ID2D1SolidColorBrush *pBrush;
	ID2D1LinearGradientBrush* pGBrush;
	ID2D1LinearGradientBrush* pGBrush2;
	ID2D1GradientStopCollection* pGradStop;

	ID2D1Bitmap* background;
	ID2D1Bitmap* blonde;
	ID2D1Bitmap* brunette;
	ID2D1Bitmap* particles;
	ID2D1Bitmap* volley;
	ID2D1Bitmap* blondeMug;
	ID2D1Bitmap* brunetteMug;
	ID2D1Bitmap* menuPic;

	D2D1_COLOR_F custom;
	D2D1_POINT_2F ptmouse;

	// DirectWrite Interfaces:
	IDWriteFactory* pDWFactory;
	IDWriteTextFormat* pTF;

	LPARAM lParam;


	IWICImagingFactory* pWICFactory;


	// Game Components:
	D2D1_RECT_F ballpos;
	D2D1_SIZE_F ballvec;

	D2D1_RECT_F paddlepos;
	D2D1_SIZE_F paddlevec;

	D2D1_RECT_F paddle2pos;
	D2D1_SIZE_F paddle2vec;

	D2D1_RECT_F netpos;

	D2D1_RECT_F mugRct;
	D2D1_RECT_F mug2Rct;

	D2D1_RECT_F spikePower;
	D2D1_RECT_F spike2Power;

	D2D1_RECT_F layoutRect;
	D2D1_RECT_F lRect;
	D2D1_RECT_F lastRect;

	D2D1_RECT_F layoutRectStart;
	D2D1_RECT_F layoutRectOptions;
	D2D1_RECT_F layoutRectCredits;
	D2D1_RECT_F layoutRectExit;

	// Variables used to calculate frames per second:
	DWORD dwFrames;
	DWORD dwCurrentTime;
	DWORD dwLastUpdateTime;
	DWORD dwElapsedTime;
	std::wstring szFPS;

	//Misc. Variables:
	bool twoPlayMode;
	bool showCollision;
	bool menu;
	bool isPaused;
	int p1Point, p2Point;
	int ballx;

public:
	BOOL bRunning;		// Is the game running?
	HANDLE hGameThread; // Handle to the GameMain thread.
	BYTE keys[256];		// Current state of the keyboard.

	
	//Create a sound system...
	FMOD::System* sys; 

	Game(void);
	virtual ~Game(void);

	void Startup(void);
	void Shutdown(void);
	void Input(void);
	void Simulate(void);
	void Render(void);
	void OnLButtonDown(LPARAM lParam);
	// Function that toggles a window between fullscreen and windowed mode:
	void ToggleFullscreenMode(HWND hWnd, BOOL bFullScreen,
			int iWidth, int iHeight, int iBpp, int iRefreshRate);

	void SetHwnd(HWND _hWnd) {hWnd = _hWnd;}
};
//Can create your or Global Unique Identifier. :)
//static const GUID myguid =
//{0xc055ce1, 0x6b90, 0x4962, {0xbb, 0x6f, 0x9e, 0x12, 0xdb, 0x9e, 0xb2, 0x28} };