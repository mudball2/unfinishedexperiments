#include <windows.h> //Include the Win32 API
//Windows.h - 1000+ Windows functions for building Windows programs.
//            Windows.h creates many new data types.
//				int, long    INT, LONG
//              float, double    FLOAT, DOUBLE
//              unsigned int     UINT
//              unsigned char, unsigned short, unsigned long      BYTE, WORD, DWORD (Respectively).
//              char, char *     CHAR, LPSTR
//              wchar_t, wchar_t *      WCHAR, LPWSTR
//              However, USE the TCHAR or LPTSTR types for char or wchar_t
//              void*     HANDLE, HINSTANCE, HWND, HBRUSH, HDC, HICON, HCURSOR
//            The Win32 API is written in C.
//              No classes, no default arguments, no overloading
//              No class, private, protected, public, virtual, friend, this, bool, true, false
#include <stdlib.h> 
#include <string.h> 
#include <tchar.h> //Include UNICODE support.
//                 L"UNICODE"
//                 _T("TCHAR")
#include <sstream> //Include the (w)ostringstream class
using std::wostringstream;

#include <wincodec.h>//Inlude the WIC header. 

#include <D2D1.h> //Include the Direct2D API.
#pragma comment(lib, "D2D1.lib") //Connect to the Direct2D Import Library. 

#include <DWrite.h> //Include the DirectWrite API.
#pragma comment(lib, "DWrite.lib")//Connect to the DirectWrite Import Library.

#pragma comment(lib, "fmodex_vc.lib")//Connect to the FMOD Import Library.
#include <fmod.h>//Include the FMOD API.
#include <fmod.hpp>//Include the FMOD API.

#include "resource.h" //Include our resources for menu strip. 


class Game
{
public:
	int xwins, owins, ties;
	bool isXTurn;
	bool gameIsOver;
	int board[3][3]; // 1 - X, 2 - O, 0 - Blank

	Game() : gameIsOver(false)
	{
		xwins = 0;
		isXTurn = true;
		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++)
				board[i][j] = 0;
	}
	void SetMark();
	void ClearBoard();
	int WhoWon();
};

int Game::WhoWon()
{
	//TODO
	return 0;
}

Game ttt;

//DirectX Interfaces:
ID2D1Factory* pD2DFactory = NULL;
ID2D1HwndRenderTarget* pRT = NULL;
ID2D1SolidColorBrush* pBrush = NULL;
ID2D1LinearGradientBrush* pGBrush = NULL;
ID2D1Bitmap* background = NULL;

D2D1_COLOR_F custom = D2D1::ColorF(D2D1::ColorF::Black);
D2D1_COLOR_F customText = D2D1::ColorF(D2D1::ColorF::Black);
D2D1_COLOR_F customX = D2D1::ColorF(D2D1::ColorF::Black);
D2D1_COLOR_F customO = D2D1::ColorF(D2D1::ColorF::Black);
D2D1_RECT_F layoutRect = D2D1::RectF(1.f, 1.f, 300.f, 300.f);
D2D1_RECT_F lRect = D2D1::RectF(210.f, 1.f, 400.f, 400.f);
D2D1_RECT_F lastRect = D2D1::RectF(410.f, 1.f, 500.f, 500.f);

IDWriteFactory* pDWFactory = NULL;
IWICImagingFactory* pWICFactory = NULL; 
IDWriteTextFormat* pTF = NULL;

//Create a sound system...
FMOD::System* sys; 
FMOD_RESULT result = FMOD::System_Create(&sys); 

//DirectX Helper Function:
HRESULT CreateGraphics(HWND hWnd);
void DestroyGraphics(void);

#define D2DColor(clr) D2D1::ColorF(D2D1::ColorF::clr)

template<class T> void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
};

static TCHAR szWindowClass[] = _T("win32app");

static TCHAR szTitle[] = _T("Tic Tac Toe");

HINSTANCE hInst;

//Forward Declarations:
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM); 
void OnPaint(HWND hWnd);
void OnLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
void OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
void OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
HRESULT LoadBitmapFromFile(ID2D1RenderTarget *pRenderTarget, IWICImagingFactory *pIWICFactory, PCWSTR uri, 
	UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap **ppBitmap);


int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	//Initialize the COM Library.
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	//Step 1: Register a Window Class.
	WNDCLASSEX wcex = {};

	wcex.cbSize = sizeof(wcex); 
	wcex.style = CS_HREDRAW | CS_VREDRAW; 
	wcex.lpfnWndProc = WndProc; 
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0; 
	wcex.hInstance = hInstance; 
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1); //Add the menu strip! :D
	wcex.lpszClassName = _T("TICTACTOE");
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if( !RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Error"),
			NULL);
		return 1;
	} 

	//Step 2: Create the Main Window.
	HWND hWnd = CreateWindow(
		wcex.lpszClassName,
		szTitle,
		WS_OVERLAPPEDWINDOW /*^ WS_MAXIMIZEBOX*/, //This works! :D
		CW_USEDEFAULT, CW_USEDEFAULT,
		618, 645,
		HWND_DESKTOP,
		NULL,
		hInstance,
		NULL ); 

	if(!hWnd)
	{
		MessageBox(HWND_DESKTOP,
			_T("Call to CreateWindow failed!"),
			_T("Tic Tac Toe"),
			MB_OK | MB_ICONERROR);
		return -1; 
	} 

	//NOTE: The window is about to be displayed.
	//      Make sure all program data is initialized.

	//Step 3: Display the Main Window.
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd); //Send the very first WM_PAINT message.

	//Step 4: Enter the Main Message loop.
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) // The WM_QUIT message makes GetMessage return FALSE.
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);//Invoke the Window Procedure.
	}

	//Uninitialize the COM Library.
	CoUninitialize();

	return (int) msg.wParam;
}

//Step 5: Implement the Window Procedure
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) 
{
	switch (message)
	{
	//Window's Constructor
	case WM_CREATE:
		{
								//Now load the music
			FMOD::Sound* sound;
			sys->setOutput(FMOD_OUTPUTTYPE_DSOUND);
			sys->init(32, FMOD_INIT_NORMAL, 0);
			result = sys->createSound("Sunshine ver 2.mp3", FMOD_LOOP_NORMAL, NULL, &sound);

			//Initializing the channel object
			FMOD::Channel* channel = 0; 

		
			//Now play the music...
			result = sys->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
			sys->update(); 

			//Initialize DirectX
			HRESULT hr = CreateGraphics(hWnd);
			if (FAILED(hr))
			{
				return -1; //Abort Window Creation.
			}
		}
		break;

	//Handle input
	case WM_LBUTTONDOWN:
		OnLButtonDown(hWnd, wParam, lParam);
		break;
	case WM_KEYDOWN:
		OnKeyDown(hWnd, wParam, lParam);
		break;

	//Handle output
	case WM_PAINT:
		OnPaint(hWnd);
		break;
	//Window's Destructor
	case WM_DESTROY:
		//Shutdown DirectX
		DestroyGraphics();

		PostQuitMessage(0); //Push the WM_QUIT message in the message queue.
		break;
		
	case WM_ERASEBKGND:
		return TRUE;

	case WM_COMMAND:
		OnCommand(hWnd, wParam, lParam);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;

}

void OnPaint(HWND hWnd)
{
#if 0
	//GDI - Graphics Device Interfaces.
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps); //HDC - handle to a Device Context
	
	//Draw Lines
	if(ttt.board[0][0] == 1)
	{
		MoveToEx(hdc, 0, 0, NULL);
		LineTo(hdc, 200, 200);
	
		MoveToEx(hdc, 200, 0, NULL);
		LineTo(hdc, 0, 200);
	}
	if(ttt.board[0][1] == 1)
	{
		MoveToEx(hdc, 200, 0, NULL);
		LineTo(hdc, 400, 200);

		MoveToEx(hdc, 400, 0, NULL);
		LineTo(hdc, 200, 200);
	}
	if(ttt.board[2][2] == 1)
	{
		MoveToEx(hdc, 200, 0, NULL);
		LineTo(hdc, 400, 200);

		MoveToEx(hdc, 400, 0, NULL);
		LineTo(hdc, 200, 200);
	}
		

	MoveToEx(hdc, 200, 20, NULL);
	LineTo(hdc, 200, 600);

	MoveToEx(hdc, 400, 20, NULL);
	LineTo(hdc, 400, 600);

	MoveToEx(hdc, 0, 200, NULL);
	LineTo(hdc, 620, 200);

	MoveToEx(hdc, 0, 400, NULL);
	LineTo(hdc, 620, 400);

	//Draw Shapes
	Ellipse(hdc, 300, 500, 400, 500);
	//Ellipse(hdc, 300, 300, 300, 300);

	//Rectangle(hdc, 225, 225, 275, 275);

	//Draw Text
	//TextOut(hdc, 100, 50, _T("Hello World"), 11);

	
	wostringstream oss;
	oss << _T("X Wins = ") << ttt.xwins;
	TextOut(hdc, 100, 0, oss.str().c_str(), oss.str().length());

	oss.str(L""); //Clean out the OSS.
	oss << _T("O Wins = ") << ttt.owins;
	TextOut(hdc, 200, 0, oss.str().c_str(), oss.str().length());

	oss.str(L"");
	oss << _T("Ties = ") << ttt.ties;
	TextOut(hdc, 300, 0, oss.str().c_str(), oss.str().length());

	EndPaint(hWnd, &ps);
#endif

	//DirectX Drawing.
	pRT->BeginDraw();

	D2D1_COLOR_F bg;
	bg.r = 0.5f;
	bg.g = 0.5f;
	bg.b = 0.0f;
	bg.a = 0.8f;
	pRT->Clear(bg);
	
	pRT->DrawBitmap(background, NULL, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);

	//Draw Lines
	if(ttt.board[0][0] == 1)
	{
		pBrush->SetColor(customX);
		pRT->DrawLine(D2D1::Point2F(0, 0), D2D1::Point2F(200, 200), pBrush, 1.0f);
		pBrush->SetColor(customX);
		pRT->DrawLine(D2D1::Point2F(200, 0), D2D1::Point2F(0, 200), pBrush, 1.0f);
	}
	else if(ttt.board[0][0] == 2)
	{
		pBrush->SetColor(customO);
		const D2D1_POINT_2F center = D2D1::Point2(105.0f, 105.0f);

		const D2D1_ELLIPSE ellipse = D2D1::Ellipse(center,
                                           95.0f, // radius X
                                           95.0f); // radius Y

		pRT->DrawEllipse(&ellipse,
                      pBrush,
                      1.0f); // stroke width
	}
	if(ttt.board[0][1] == 1)
	{
		pBrush->SetColor(customX);
		pRT->DrawLine(D2D1::Point2F(200, 0), D2D1::Point2F(400, 200), pBrush, 1.0f);
		pBrush->SetColor(customX);
		pRT->DrawLine(D2D1::Point2F(400, 0), D2D1::Point2F(200, 200), pBrush, 1.0f);
	}
	else if(ttt.board[0][1] == 2)
	{
		pBrush->SetColor(customO);
		const D2D1_POINT_2F center = D2D1::Point2(305.0f, 105.0f);

		const D2D1_ELLIPSE ellipse = D2D1::Ellipse(center,
                                           95.0f, // radius X
                                           95.0f); // radius Y

		pRT->DrawEllipse(&ellipse,
                      pBrush,
                      1.0f); // stroke width
	}
	if(ttt.board[0][2] == 1)
	{
		pBrush->SetColor(customX);
		pRT->DrawLine(D2D1::Point2F(400, 0), D2D1::Point2F(600, 200), pBrush, 1.0f);
		pBrush->SetColor(customX);
		pRT->DrawLine(D2D1::Point2F(600, 0), D2D1::Point2F(400, 200), pBrush, 1.0f);
	}	
	else if(ttt.board[0][2] == 2)
	{
		pBrush->SetColor(customO);
		const D2D1_POINT_2F center = D2D1::Point2(505.0f, 105.0f);

		const D2D1_ELLIPSE ellipse = D2D1::Ellipse(center,
                                           95.0f, // radius X
                                           95.0f); // radius Y

		pRT->DrawEllipse(&ellipse,
                      pBrush,
                      1.0f); // stroke width
	}
	if(ttt.board[1][0] == 1)
	{
		pBrush->SetColor(customX);
		pRT->DrawLine(D2D1::Point2F(0, 200), D2D1::Point2F(200, 400), pBrush, 1.0f);
		pBrush->SetColor(customX);
		pRT->DrawLine(D2D1::Point2F(200, 200), D2D1::Point2F(0, 400), pBrush, 1.0f);
	}
	else if(ttt.board[1][0] == 2)
	{
		pBrush->SetColor(customO);
		const D2D1_POINT_2F center = D2D1::Point2(105.0f, 305.0f);

		const D2D1_ELLIPSE ellipse = D2D1::Ellipse(center,
                                           95.0f, // radius X
                                           95.0f); // radius Y

		pRT->DrawEllipse(&ellipse,
                      pBrush,
                      1.0f); // stroke width
	}
	if(ttt.board[1][1] == 1)
	{
		pBrush->SetColor(customX);
		pRT->DrawLine(D2D1::Point2F(200, 200), D2D1::Point2F(400, 400), pBrush, 1.0f);
		pBrush->SetColor(customX);
		pRT->DrawLine(D2D1::Point2F(400, 200), D2D1::Point2F(200, 400), pBrush, 1.0f);
	}
	else if(ttt.board[1][1] == 2)
	{
		pBrush->SetColor(customO);
		const D2D1_POINT_2F center = D2D1::Point2(305.0f, 305.0f);

		const D2D1_ELLIPSE ellipse = D2D1::Ellipse(center,
                                           95.0f, // radius X
                                           95.0f); // radius Y

		pRT->DrawEllipse(&ellipse,
                      pBrush,
                      1.0f); // stroke width
	}
	if(ttt.board[1][2] == 1)
	{
		pBrush->SetColor(customX);
		pRT->DrawLine(D2D1::Point2F(400, 200), D2D1::Point2F(600, 400), pBrush, 1.0f);
		pBrush->SetColor(customX);
		pRT->DrawLine(D2D1::Point2F(600, 200 ), D2D1::Point2F(400, 400), pBrush, 1.0f);
	}
	else if(ttt.board[1][2] == 2)
	{
		pBrush->SetColor(customO);
		const D2D1_POINT_2F center = D2D1::Point2(505.0f, 305.0f);

		const D2D1_ELLIPSE ellipse = D2D1::Ellipse(center,
                                           95.0f, // radius X
                                           95.0f); // radius Y

		pRT->DrawEllipse(&ellipse,
                      pBrush,
                      1.0f); // stroke width
	}
	if(ttt.board[2][0] == 1)
	{		
		pBrush->SetColor(customX);
		pRT->DrawLine(D2D1::Point2F(0, 400), D2D1::Point2F(200, 600), pBrush, 1.0f);
		pBrush->SetColor(customX);
		pRT->DrawLine(D2D1::Point2F(200, 400), D2D1::Point2F(0, 600), pBrush, 1.0f);

	}
	else if(ttt.board[2][0] == 2)
	{
		pBrush->SetColor(customO);
		const D2D1_POINT_2F center = D2D1::Point2(105.0f, 505.0f);

		const D2D1_ELLIPSE ellipse = D2D1::Ellipse(center,
                                           95.0f, // radius X
                                           95.0f); // radius Y

		pRT->DrawEllipse(&ellipse,
                      pBrush,
                      1.0f); // stroke width
	}
	if(ttt.board[2][1] == 1)
	{		
		pBrush->SetColor(customX);
		pRT->DrawLine(D2D1::Point2F(200, 400), D2D1::Point2F(400, 600), pBrush, 1.0f);
		pBrush->SetColor(customX);
		pRT->DrawLine(D2D1::Point2F(400, 400), D2D1::Point2F(200, 600), pBrush, 1.0f);

	}
	else if(ttt.board[2][1] == 2)
	{
		pBrush->SetColor(customO);
		const D2D1_POINT_2F center = D2D1::Point2(305.0f, 505.0f);

		const D2D1_ELLIPSE ellipse = D2D1::Ellipse(center,
                                           95.0f, // radius X
                                           95.0f); // radius Y

		pRT->DrawEllipse(&ellipse,
                      pBrush,
                      1.0f); // stroke width
	}
	if(ttt.board[2][2] == 1)
	{
		pBrush->SetColor(customX);
		pRT->DrawLine(D2D1::Point2F(400, 400), D2D1::Point2F(600, 600), pBrush, 1.0f);
		pBrush->SetColor(customX);
		pRT->DrawLine(D2D1::Point2F(600, 400), D2D1::Point2F(400, 600), pBrush, 1.0f);
	}
	else if(ttt.board[2][2] == 2)
	{
		pBrush->SetColor(customO);
		const D2D1_POINT_2F center = D2D1::Point2(505.0f, 505.0f);

		const D2D1_ELLIPSE ellipse = D2D1::Ellipse(center,
                                           95.0f, // radius X
                                           95.0f); // radius Y

		pRT->DrawEllipse(&ellipse,
                      pBrush,
                      1.0f); // stroke width
	}
	pBrush->SetColor(custom);
	pRT->DrawLine(D2D1::Point2F(200, 0), D2D1::Point2F(200, 600),
		pBrush, 6.0f);

	pBrush->SetColor(custom);
	pRT->DrawLine(D2D1::Point2F(400, 0), D2D1::Point2F(400, 600),
		pBrush, 6.0f);

	pBrush->SetColor(custom);
	pRT->DrawLine(D2D1::Point2F(0, 200), D2D1::Point2F(600, 200),
		pBrush, 6.0f);

	pBrush->SetColor(custom);
	pRT->DrawLine(D2D1::Point2F(0, 400), D2D1::Point2F(600, 400),
		pBrush, 6.0f);

	pBrush->SetColor(customText);

	wostringstream oss;
	oss << _T("X Wins = ") << ttt.xwins;/*
	TextOut(hdc, 100, 0, oss.str().c_str(), oss.str().length());*/
	pRT->DrawTextW(oss.str().c_str(), oss.str().size(),pTF ,layoutRect ,pBrush, 
		D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
	


	oss.str(L""); //Clean out the OSS.
	oss << _T("O Wins = ") << ttt.owins;
	pRT->DrawTextW(oss.str().c_str(), oss.str().size(),pTF ,lRect ,pBrush, 
		D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);

	oss.str(L"");
	oss << _T("Ties = ") << ttt.ties;
	pRT->DrawTextW(oss.str().c_str(), oss.str().size(),pTF ,lastRect ,pBrush, 
		D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);

	/*pRT->EndDraw();*/
	HRESULT hr = pRT->EndDraw();
    if (hr == D2DERR_RECREATE_TARGET)
    {
        //TODO: Destroy Direct2D.
		DestroyGraphics();
        //TODO: Recreate Direct2D.
		CreateGraphics(hWnd);
    }

	ValidateRect(hWnd, NULL);
}

void OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
		if(ttt.isXTurn == true)
	{
		FMOD::Sound* sound;
		sys->setOutput(FMOD_OUTPUTTYPE_DSOUND);
		sys->init(32, FMOD_INIT_NORMAL, 0);
		result = sys->createSound("boing_1.wav", FMOD_LOOP_OFF, NULL, &sound);//Turn the loop off to play sound once.

		//Initializing the channel object
		FMOD::Channel* channel = 0; 

		result = sys->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
		sys->update(); 
	}
	else if(ttt.isXTurn == false)
	{
		FMOD::Sound* sound;
		sys->setOutput(FMOD_OUTPUTTYPE_DSOUND);
		sys->init(32, FMOD_INIT_NORMAL, 0);
		result = sys->createSound("cork_pop.wav", FMOD_LOOP_OFF, NULL, &sound); //Turn the loop off to play sound
		//once.

		//Initializing the channel object
		FMOD::Channel* channel = 0; 

		result = sys->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
		sys->update(); 

	}
	
	int full = 0;
	//Check if game is over
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			if(ttt.board[i][j] != 0)
			{
				full++;
				if( full == 8)
				{
					
					ttt.ties += 1;
					ttt.gameIsOver = true;
				}
			}
//OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)

	// Buffered Input
	int vkcode = wParam;
	switch (vkcode)
	{
	case VK_NUMPAD1:
	if(ttt.isXTurn == true)
	{
		ttt.board[2][0] = 1;
		ttt.isXTurn = false;
		InvalidateRect(hWnd, NULL, true);
	}
	else if(ttt.isXTurn == false)
	{
		ttt.board[2][0] = 2;
		ttt.isXTurn = true;
		InvalidateRect(hWnd, NULL, true);
	}
		break;
	case VK_NUMPAD2:
	if(ttt.isXTurn == true)
	{
		ttt.board[2][1] = 1;
		ttt.isXTurn = false;
		InvalidateRect(hWnd, NULL, true);
	}
	else if(ttt.isXTurn == false)
	{
		ttt.board[2][1] = 2;
		ttt.isXTurn = true;
		InvalidateRect(hWnd, NULL, true);
	}
		break;
	case VK_NUMPAD3:
	if(ttt.isXTurn == true)
	{
		ttt.board[2][2] = 1;
		ttt.isXTurn = false;
		InvalidateRect(hWnd, NULL, true);
	}
	else if(ttt.isXTurn == false)
	{
		ttt.board[2][2] = 2;
		ttt.isXTurn = true;
		InvalidateRect(hWnd, NULL, true);
	}
		break;
	case VK_NUMPAD4:
	if(ttt.isXTurn == true)
	{
		ttt.board[1][0] = 1;
		ttt.isXTurn = false;
		InvalidateRect(hWnd, NULL, true);
	}
	else if(ttt.isXTurn == false)
	{
		ttt.board[1][0] = 2;
		ttt.isXTurn = true;
		InvalidateRect(hWnd, NULL, true);
	}
		break;
	case VK_NUMPAD5:
	if(ttt.isXTurn == true)
	{
		ttt.board[1][1] = 1;
		ttt.isXTurn = false;
		InvalidateRect(hWnd, NULL, true);
	}
	else if(ttt.isXTurn == false)
	{
		ttt.board[1][1] = 2;
		ttt.isXTurn = true;
		InvalidateRect(hWnd, NULL, true);
	}
		break;
	case VK_NUMPAD6:
	if(ttt.isXTurn == true)
	{
		ttt.board[1][2] = 1;
		ttt.isXTurn = false;
		InvalidateRect(hWnd, NULL, true);
	}
	else if(ttt.isXTurn == false)
	{
		ttt.board[1][2] = 2;
		ttt.isXTurn = true;
		InvalidateRect(hWnd, NULL, true);
	}
		break;
	case VK_NUMPAD7:
	if(ttt.isXTurn == true)
	{
		ttt.board[0][0] = 1;
		ttt.isXTurn = false;
		InvalidateRect(hWnd, NULL, true);
	}
	else if(ttt.isXTurn == false)
	{
		ttt.board[0][0] = 2;
		ttt.isXTurn = true;
		InvalidateRect(hWnd, NULL, true);
	}
		break;
	case VK_NUMPAD8:
	if(ttt.isXTurn == true)
	{
		ttt.board[0][1] = 1;
		ttt.isXTurn = false;
		InvalidateRect(hWnd, NULL, true);
	}
	else if(ttt.isXTurn == false)
	{
		ttt.board[0][1] = 2;
		ttt.isXTurn = true;
		InvalidateRect(hWnd, NULL, true);
	}
		break;
	case VK_NUMPAD9:
	if(ttt.isXTurn == true)
	{
		ttt.board[0][2] = 1;
		ttt.isXTurn = false;
		InvalidateRect(hWnd, NULL, true);
	}
	else if(ttt.isXTurn == false)
	{
		ttt.board[0][2] = 2;
		ttt.isXTurn = true;
		InvalidateRect(hWnd, NULL, true);
	}
		break;
	default:
		return;
	}

		
	if( (ttt.board[0][0] == 1 && ttt.board[1][1] == 1 && ttt.board[2][2] == 1) || 
		(ttt.board[0][0] == 1 && ttt.board[1][0] == 1 && ttt.board[2][0] == 1) ||
		(ttt.board[0][0] == 1 && ttt.board[0][1] == 1 && ttt.board[0][2] == 1) ||
		(ttt.board[0][1] == 1 && ttt.board[1][1] == 1 && ttt.board[2][1] == 1) ||
		(ttt.board[0][2] == 1 && ttt.board[1][2] == 1 && ttt.board[2][2] == 1) ||
		(ttt.board[1][0] == 1 && ttt.board[1][1] == 1 && ttt.board[1][2] == 1) ||
		(ttt.board[2][0] == 1 && ttt.board[2][1] == 1 && ttt.board[2][2] == 1) ||
		(ttt.board[2][0] == 1 && ttt.board[1][1] == 1 && ttt.board[0][2] == 1))
	{
		ttt.xwins += 1;
		MessageBox(hWnd, _T("X Wins!"), _T("Tic Tac Toe"), MB_OK | MB_ICONINFORMATION);
		ttt.gameIsOver = true;
	}
	else if((ttt.board[0][0] == 2 && ttt.board[1][1] == 2 && ttt.board[2][2] == 2) || 
		(ttt.board[0][0] == 2 && ttt.board[1][0] == 2 && ttt.board[2][0] == 2) ||
		(ttt.board[0][0] == 2 && ttt.board[0][1] == 2 && ttt.board[0][2] == 2) ||
		(ttt.board[0][1] == 2 && ttt.board[1][1] == 2 && ttt.board[2][1] == 2) ||
		(ttt.board[0][2] == 2 && ttt.board[1][2] == 2 && ttt.board[2][2] == 2) ||
		(ttt.board[1][0] == 2 && ttt.board[1][1] == 2 && ttt.board[1][2] == 2) ||
		(ttt.board[2][0] == 2 && ttt.board[2][1] == 2 && ttt.board[2][2] == 2) ||
		(ttt.board[2][0] == 2 && ttt.board[1][1] == 2 && ttt.board[0][2] == 2))
	{
		ttt.owins += 1;
		MessageBox(hWnd, _T("O Wins!"), _T("Tic Tac Toe"), MB_OK | MB_ICONINFORMATION);
		ttt.gameIsOver = true;
	}


	if(ttt.gameIsOver == true)
	{
		int response = MessageBox(hWnd, _T("Play Again?"), _T("Tic Tac Toe"), MB_YESNO | MB_ICONQUESTION);
		if(response == IDYES)
		{ 
			for(int i = 0; i < 3; i++)
				for(int j = 0; j < 3; j++)
				{
					ttt.board[i][j] = 0;
					InvalidateRect(hWnd, NULL, true);
				}
			
			full = 0;	
			ttt.gameIsOver = false;
		}
		else if(response == IDNO)
		{
			DestroyWindow(hWnd);
		}
	}
}

void OnLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if(ttt.isXTurn == true)
	{
		FMOD::Sound* sound;
		sys->setOutput(FMOD_OUTPUTTYPE_DSOUND);
		sys->init(32, FMOD_INIT_NORMAL, 0);
		result = sys->createSound("boing_1.wav", FMOD_LOOP_OFF, NULL, &sound);//Turn the loop off to play sound once.

		//Initializing the channel object
		FMOD::Channel* channel = 0; 

		result = sys->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
		sys->update(); 
	}
	else if(ttt.isXTurn == false)
	{
		FMOD::Sound* sound;
		sys->setOutput(FMOD_OUTPUTTYPE_DSOUND);
		sys->init(32, FMOD_INIT_NORMAL, 0);
		result = sys->createSound("cork_pop.wav", FMOD_LOOP_OFF, NULL, &sound); //Turn the loop off to play sound
		//once.

		//Initializing the channel object
		FMOD::Channel* channel = 0; 

		result = sys->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
		sys->update(); 

	}
	
	int full = 0;
	//Check if game is over
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			if(ttt.board[i][j] != 0)
			{
				full++;
				if( full == 8)
				{
					
					ttt.ties += 1;
					ttt.gameIsOver = true;
				}
			}

#if 1
	POINT ptMouse;
	ptMouse.x = LOWORD(lParam);
	ptMouse.y = HIWORD(lParam);

	if(ptMouse.x < 200 && ptMouse.y < 200  && ttt.isXTurn == true)
	{
		ttt.board[0][0] = 1;
		ttt.isXTurn = false;
		InvalidateRect(hWnd, NULL, true);
	}
	else if(ptMouse.x < 200 && ptMouse.y < 200  && ttt.isXTurn == false)
	{
		ttt.board[0][0] = 2;
		ttt.isXTurn = true;
		InvalidateRect(hWnd, NULL, true);
	}
	if(ptMouse.x > 200 && ptMouse.x < 400 && ptMouse.y < 200  && ttt.isXTurn == true)
	{
		ttt.board[0][1] = 1;
		ttt.isXTurn = false;
		InvalidateRect(hWnd, NULL, true);
	}
	else if(ptMouse.x > 200 && ptMouse.x < 400 && ptMouse.y < 200  && ttt.isXTurn == false)
	{
		ttt.board[0][1] = 2;
		ttt.isXTurn = true;
		InvalidateRect(hWnd, NULL, true);
	}
	if(ptMouse.x > 400 && ptMouse.x < 600 && ptMouse.y < 200  && ttt.isXTurn == true)
	{
		ttt.board[0][2] = 1;
		ttt.isXTurn = false;
		InvalidateRect(hWnd, NULL, true);
	}
	else if(ptMouse.x > 400 && ptMouse.x < 600 && ptMouse.y < 200  && ttt.isXTurn == false)
	{
		ttt.board[0][2] = 2;
		ttt.isXTurn = true;
		InvalidateRect(hWnd, NULL, true);
	}
	if(ptMouse.x < 200 && ptMouse.y < 400 && ptMouse.y > 200  && ttt.isXTurn == true)
	{
		ttt.board[1][0] = 1;
		ttt.isXTurn = false;
		InvalidateRect(hWnd, NULL, true);
	}
	else if(ptMouse.x < 200 && ptMouse.y < 400 && ptMouse.y > 200  && ttt.isXTurn == false)
	{
		ttt.board[1][0] = 2;
		ttt.isXTurn = true;
		InvalidateRect(hWnd, NULL, true);
	}
	if(ptMouse.x > 200 && ptMouse.x < 400 && ptMouse.y < 400 && ptMouse.y > 200  && ttt.isXTurn == true)
	{
		ttt.board[1][1] = 1;
		ttt.isXTurn = false;
		InvalidateRect(hWnd, NULL, true);
	}
	else if(ptMouse.x > 200 && ptMouse.x < 400 && ptMouse.y < 400 && ptMouse.y > 200  && ttt.isXTurn == false)
	{
		ttt.board[1][1] = 2;
		ttt.isXTurn = true;
		InvalidateRect(hWnd, NULL, true);
	}
	if(ptMouse.x > 400 && ptMouse.x < 600 && ptMouse.y < 400 && ptMouse.y > 200  && ttt.isXTurn == true)
	{
		ttt.board[1][2] = 1;
		ttt.isXTurn = false;
		InvalidateRect(hWnd, NULL, true);
	}
	else if(ptMouse.x > 400 && ptMouse.x < 600 && ptMouse.y < 400 && ptMouse.y > 200  && ttt.isXTurn == false)
	{
		ttt.board[1][2] = 2;
		ttt.isXTurn = true;
		InvalidateRect(hWnd, NULL, true);
	}
	if(ptMouse.x < 200 && ptMouse.y > 400  && ttt.isXTurn == true)
	{
		ttt.board[2][0] = 1;
		ttt.isXTurn = false;
		InvalidateRect(hWnd, NULL, true);
	}
	else if(ptMouse.x < 200 && ptMouse.y > 400  && ttt.isXTurn == false)
	{
		ttt.board[2][0] = 2;
		ttt.isXTurn = true;
		InvalidateRect(hWnd, NULL, true);
	}
	if(ptMouse.x > 200 && ptMouse.x < 400 && ptMouse.y > 400  && ttt.isXTurn == true)
	{
		ttt.board[2][1] = 1;
		ttt.isXTurn = false;
		InvalidateRect(hWnd, NULL, true);
	}
	else if(ptMouse.x > 200 && ptMouse.x < 400 && ptMouse.y > 400  && ttt.isXTurn == false)
	{
		ttt.board[2][1] = 2;
		ttt.isXTurn = true;
		InvalidateRect(hWnd, NULL, true);
	}
	if(ptMouse.x > 400 && ptMouse.x < 600 && ptMouse.y > 400  && ttt.isXTurn == true)
	{
		ttt.board[2][2] = 1;
		ttt.isXTurn = false;
		InvalidateRect(hWnd, NULL, true);
	}
	else if(ptMouse.x > 400 && ptMouse.x < 600 && ptMouse.y > 400  && ttt.isXTurn == false)
	{
		ttt.board[2][2] = 2;
		ttt.isXTurn = true;
		InvalidateRect(hWnd, NULL, true);
	}
#endif


#if 0
	if(ttt.isXTurn == false)
	{
		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++)
				if(ttt.board[i][j] == 0)
				{
					ttt.board[i][j] = 2;
					ttt.isXTurn = true;
					break;
				}
	}
#endif

		
	if( (ttt.board[0][0] == 1 && ttt.board[1][1] == 1 && ttt.board[2][2] == 1) || 
		(ttt.board[0][0] == 1 && ttt.board[1][0] == 1 && ttt.board[2][0] == 1) ||
		(ttt.board[0][0] == 1 && ttt.board[0][1] == 1 && ttt.board[0][2] == 1) ||
		(ttt.board[0][1] == 1 && ttt.board[1][1] == 1 && ttt.board[2][1] == 1) ||
		(ttt.board[0][2] == 1 && ttt.board[1][2] == 1 && ttt.board[2][2] == 1) ||
		(ttt.board[1][0] == 1 && ttt.board[1][1] == 1 && ttt.board[1][2] == 1) ||
		(ttt.board[2][0] == 1 && ttt.board[2][1] == 1 && ttt.board[2][2] == 1) ||
		(ttt.board[2][0] == 1 && ttt.board[1][1] == 1 && ttt.board[0][2] == 1))
	{
		ttt.xwins += 1;
		MessageBox(hWnd, _T("X Wins!"), _T("Tic Tac Toe"), MB_OK | MB_ICONINFORMATION);
		ttt.gameIsOver = true;
	}
	else if((ttt.board[0][0] == 2 && ttt.board[1][1] == 2 && ttt.board[2][2] == 2) || 
		(ttt.board[0][0] == 2 && ttt.board[1][0] == 2 && ttt.board[2][0] == 2) ||
		(ttt.board[0][0] == 2 && ttt.board[0][1] == 2 && ttt.board[0][2] == 2) ||
		(ttt.board[0][1] == 2 && ttt.board[1][1] == 2 && ttt.board[2][1] == 2) ||
		(ttt.board[0][2] == 2 && ttt.board[1][2] == 2 && ttt.board[2][2] == 2) ||
		(ttt.board[1][0] == 2 && ttt.board[1][1] == 2 && ttt.board[1][2] == 2) ||
		(ttt.board[2][0] == 2 && ttt.board[2][1] == 2 && ttt.board[2][2] == 2) ||
		(ttt.board[2][0] == 2 && ttt.board[1][1] == 2 && ttt.board[0][2] == 2))
	{
		ttt.owins += 1;
		MessageBox(hWnd, _T("O Wins!"), _T("Tic Tac Toe"), MB_OK | MB_ICONINFORMATION);
		ttt.gameIsOver = true;
	}


	if(ttt.gameIsOver == true)
	{
		int response = MessageBox(hWnd, _T("Play Again?"), _T("Tic Tac Toe"), MB_YESNO | MB_ICONQUESTION);
		if(response == IDYES)
		{ 
			for(int i = 0; i < 3; i++)
				for(int j = 0; j < 3; j++)
				{
					ttt.board[i][j] = 0;
					InvalidateRect(hWnd, NULL, true);
				}
			
			full = 0;	
			ttt.gameIsOver = false;
		}
		else if(response == IDNO)
		{
			DestroyWindow(hWnd);
		}
	}
}

	// Array of COLORREFs used to store the custom colors.
COLORREF clrarrCustom[16] =
{
	RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),
	RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),
	RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),
	RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),
};

void OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//Which menu item was selected?
	int menuid = LOWORD(wParam);
	switch(menuid)
	{
	case IDM_FILE_NEW40001:
		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++)
				ttt.board[i][j] = 0;
		ttt.ties = 0;
		ttt.xwins = 0;
		ttt.owins = 0;
		custom = D2D1::ColorF(D2D1::ColorF::Black);
		customText = D2D1::ColorF(D2D1::ColorF::Black);
		customX = D2D1::ColorF(D2D1::ColorF::Black);
		customO = D2D1::ColorF(D2D1::ColorF::Black);
		InvalidateRect(hWnd, NULL, true);
		break;

	case IDM_FILE_EXIT:
		DestroyWindow(hWnd);
		break;

	case IDM_OPTIONS_COLORS:
		{

		case ID_COLORS_XCOLOR:
			CHOOSECOLOR cc;
			ZeroMemory(&cc, sizeof(CHOOSECOLOR));
			cc.lStructSize = sizeof(CHOOSECOLOR);
			cc.hwndOwner = hWnd;
			cc.Flags = CC_RGBINIT;
			cc.lpCustColors = clrarrCustom;

			// Invoke the Color Selection dialog.
			if(ChooseColor(&cc))
			{
				// NOTE: The selected color is stored in the cc.rgbResult variable.
				customX = D2D1::ColorF(GetRValue(cc.rgbResult) << 16 | 
									GetGValue(cc.rgbResult) << 8 |
									GetBValue(cc.rgbResult));

					//custom = D2D1::ColorF(rand()%256 << 16 | 
					//				rand()%256 << 8 |
					//				rand()%256);

				// Repaint the screen.
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case ID_COLORS_OCOLOR:
			{
				// Array of COLORREFs used to store the custom colors.
			COLORREF clrarrCustom2[16] =
			{
				RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),
				RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),
				RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),
				RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),
			};

			CHOOSECOLOR cc2;
			ZeroMemory(&cc2, sizeof(CHOOSECOLOR));
			cc2.lStructSize = sizeof(CHOOSECOLOR);
			cc2.hwndOwner = hWnd;
			cc2.Flags = CC_RGBINIT;
			cc2.lpCustColors = clrarrCustom;

			// Invoke the Color Selection dialog.
			if(ChooseColor(&cc2))
			{
				// NOTE: The selected color is stored in the cc.rgbResult variable.
				customO = D2D1::ColorF(GetRValue(cc2.rgbResult) << 16 | 
									GetGValue(cc2.rgbResult) << 8 |
									GetBValue(cc2.rgbResult));

					//custom = D2D1::ColorF(rand()%256 << 16 | 
					//				rand()%256 << 8 |
					//				rand()%256);

				// Repaint the screen.
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
			}
		case ID_COLORS_BOARDCOLOR:
			{
				// Array of COLORREFs used to store the custom colors.
			COLORREF clrarrCustom3[16] =
			{
				RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),
				RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),
				RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),
				RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),
			};

			CHOOSECOLOR cc3;
			ZeroMemory(&cc3, sizeof(CHOOSECOLOR));
			cc3.lStructSize = sizeof(CHOOSECOLOR);
			cc3.hwndOwner = hWnd;
			cc3.Flags = CC_RGBINIT;
			cc3.lpCustColors = clrarrCustom;

			// Invoke the Color Selection dialog.
			if(ChooseColor(&cc3))
			{
				// NOTE: The selected color is stored in the cc.rgbResult variable.
				custom = D2D1::ColorF(GetRValue(cc3.rgbResult) << 16 | 
									GetGValue(cc3.rgbResult) << 8 |
									GetBValue(cc3.rgbResult));

					//custom = D2D1::ColorF(rand()%256 << 16 | 
					//				rand()%256 << 8 |
					//				rand()%256);

				// Repaint the screen.
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
			}
		case ID_COLORS_SCORES:
			{
				// Array of COLORREFs used to store the custom colors.
			COLORREF clrarrCustom4[16] =
			{
				RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),
				RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),
				RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),
				RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),
			};

			CHOOSECOLOR cc4;
			ZeroMemory(&cc4, sizeof(CHOOSECOLOR));
			cc4.lStructSize = sizeof(CHOOSECOLOR);
			cc4.hwndOwner = hWnd;
			cc4.Flags = CC_RGBINIT;
			cc4.lpCustColors = clrarrCustom;

			// Invoke the Color Selection dialog.
			if(ChooseColor(&cc4))
			{
				// NOTE: The selected color is stored in the cc.rgbResult variable.
				customText = D2D1::ColorF(GetRValue(cc4.rgbResult) << 16 | 
									GetGValue(cc4.rgbResult) << 8 |
									GetBValue(cc4.rgbResult));

					//custom = D2D1::ColorF(rand()%256 << 16 | 
					//				rand()%256 << 8 |
					//				rand()%256);

				// Repaint the screen.
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
			}
		case ID_COLORS_RANDOMIZE:
				custom = D2D1::ColorF(rand()%256 << 16 | 
								rand()%256 << 8 |
								rand()%256);
				customX = D2D1::ColorF(rand()%256 << 16 | 
								rand()%256 << 8 |
								rand()%256);
				customO = D2D1::ColorF(rand()%256 << 16 | 
								rand()%256 << 8 |
								rand()%256);
				customText = D2D1::ColorF(rand()%256 << 16 | 
								rand()%256 << 8 |
								rand()%256);

			// Repaint the screen.
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
	break;
	case ID_OPTIONS_GAMEMODES:
		{
			case ID_GAMEMODES_1PLAYER:
				break;
			case ID_GAMEMODES_2PLAYER:
				break;
		}
	break;
	case ID_OPTIONS_SOUNDS:
		{
			case ID_SOUNDS_MUTE:
				
				break;
			case ID_SOUNDS_MUTE40018:
				break;
		}
	break;
	case ID_HELP_ABOUT:
		MessageBox(hWnd, _T("Programmer: Gabriel Islas\n Contact:\n\tTwitter: @GabrielIslas4\n\tTumblr: http://mudball2.tumblr.com/"),
			_T("Tic Tac Toe"), MB_OK | MB_ICONINFORMATION);
		break;
	}
}

HRESULT LoadBitmapFromFile(ID2D1RenderTarget *pRenderTarget, IWICImagingFactory *pIWICFactory, PCWSTR uri, 
	UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap **ppBitmap)
{
		IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode *pSource = NULL;
	IWICStream *pStream = NULL;
	IWICFormatConverter *pConverter = NULL;
	IWICBitmapScaler *pScaler = NULL;

	HRESULT hr = pIWICFactory->CreateDecoderFromFilename(
		uri,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
		);

	if (SUCCEEDED(hr))
	{
		// Create the initial frame.
		hr = pDecoder->GetFrame(0, &pSource);
	}
	if (SUCCEEDED(hr))
	{

		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = pIWICFactory->CreateFormatConverter(&pConverter);

	}


	if (SUCCEEDED(hr))
	{
		// If a new width or height was specified, create an
		// IWICBitmapScaler and use it to resize the image.
		if (destinationWidth != 0 || destinationHeight != 0)
		{
			UINT originalWidth, originalHeight;
			hr = pSource->GetSize(&originalWidth, &originalHeight);
			if (SUCCEEDED(hr))
			{
				if (destinationWidth == 0)
				{
					FLOAT scalar = static_cast<FLOAT>(destinationHeight) / static_cast<FLOAT>(originalHeight);
					destinationWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
				}
				else if (destinationHeight == 0)
				{
					FLOAT scalar = static_cast<FLOAT>(destinationWidth) / static_cast<FLOAT>(originalWidth);
					destinationHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
				}

				hr = pIWICFactory->CreateBitmapScaler(&pScaler);
				if (SUCCEEDED(hr))
				{
					hr = pScaler->Initialize(
						pSource,
						destinationWidth,
						destinationHeight,
						WICBitmapInterpolationModeCubic
						);
				}
				if (SUCCEEDED(hr))
				{
					hr = pConverter->Initialize(
						pScaler,
						GUID_WICPixelFormat32bppPBGRA,
						WICBitmapDitherTypeNone,
						NULL,
						0.f,
						WICBitmapPaletteTypeMedianCut
						);
				}
			}
		}
		else // Don't scale the image.
		{
			hr = pConverter->Initialize(
				pSource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeMedianCut
				);
		}
	}
	if (SUCCEEDED(hr))
	{

		// Create a Direct2D bitmap from the WIC bitmap.
		hr = pRenderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			ppBitmap
			);
	}

	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pStream);
	SafeRelease(&pConverter);
	SafeRelease(&pScaler);

	return hr;
}

HRESULT CreateGraphics(HWND hWnd)
{
	//Initialize the Direct2D Factory.
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);

	if(FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("Error: Failed to create the Direct2D Facotry."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	// Get the dimensions of the client.
	RECT rc;
	GetClientRect(hWnd, &rc);

	//Initialize a Direct2D Size Structure.
	D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

	//Create the Direct2D Render Target.
	hr = pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, size), &pRT);

	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("Error: Failed to create Direct2D Render Target."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}
	pRT->SetDpi(96.0f, 96.0f);

	//Create the Direct2D Solid Color.
	hr = pRT->CreateSolidColorBrush(D2D1::ColorF(0x0), &pBrush);

	if(FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("Error: Failed to create Solid color brush."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	//Initialize the DirectWrite Factory.
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		(IUnknown**)&pDWFactory);
	if(FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("Error: Failed to create DirectWrite Factory."),
			_T("DirectWrite Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	//Create the DirectWrite Text Format.
	hr = pDWFactory->CreateTextFormat(_T("Veranda"), NULL,
		DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		24, _T(""), &pTF);
	if(FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("Error: Failed to create Text Format."),
			_T("DirectWrite Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	
	 hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pWICFactory));
	 hr = LoadBitmapFromFile(pRT, pWICFactory, L"background.png", 618, 645, &background);


	return S_OK;
}

void DestroyGraphics(void)
{
	SafeRelease(&pTF);
	SafeRelease(&background);
	SafeRelease(&pDWFactory);
	SafeRelease(&pBrush);
	SafeRelease(&pRT);
	SafeRelease(&pD2DFactory);
	SafeRelease(&pWICFactory);

}