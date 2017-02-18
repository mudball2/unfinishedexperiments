#include "Game.h"

Game::Game(void) : twoPlayMode(true), showCollision(false), p1Point(0), p2Point(0), isPaused(false), menu(true), 
	ballx(0)
{
	bRunning = TRUE;
	hWnd = NULL;
	ZeroMemory(keys, sizeof(keys));

	pD2DFactory = NULL;
	pRT = NULL;
	pBrush = NULL;
	pGBrush = NULL;
	pGBrush2 = NULL;
	pGradStop = NULL;
	pDWFactory = NULL;
	pTF = NULL;
	menuPic = NULL;
	pWICFactory = NULL;
	blonde = NULL;
	brunette = NULL;
	blondeMug = NULL;
	brunetteMug = NULL;
	background = NULL;
	particles = NULL;
	volley = NULL;

	// Zero out the frames per second variables:
	dwFrames = 0;
	dwCurrentTime = 0;
	dwLastUpdateTime = 0;
	dwElapsedTime = 0;
	szFPS = _T("");
}

Game::~Game(void)
{
}

void Game::Startup(void)
{
//TODO: Create Back Buffer

	// Initialize DirectX.
	HRESULT hr = CreateGraphics(hWnd);
	if (FAILED(hr))
	{
		return; // Return -1 to abort Window Creation.
	}

//TODO: Load Bitmaps/Brushes/Pens/Fonts

//TODO: Initialize Game Components

	ballpos = D2D1::RectF(20, 20, 45, 45);
	ballvec = D2D1::SizeF(3, 3);

	paddlepos = D2D1::RectF(130, 445, 190, 550);
	paddlevec = D2D1::SizeF(0,3);

	paddle2pos = D2D1::RectF(530, 445, 590, 550);
	paddle2vec = D2D1::SizeF(0, 3);

	netpos = D2D1::RectF(400, 370, 420, 410);

	mugRct = D2D1::RectF(50, 50, 150, 100);
	mug2Rct = D2D1::RectF(650, 50, 750, 100);

	spikePower = D2D1::RectF(90, 50, 150, 100);
	spike2Power = D2D1::RectF(690, 50, 750, 100);

	layoutRect = D2D1::RectF(350, 50, 380, 100);
	lRect = D2D1::RectF(380, 20, 410, 40);
	lastRect = D2D1::RectF(410, 50, 440, 100);

	layoutRectStart = D2D1::RectF(600.f, 250.f, 750.f, 300.f);
	layoutRectOptions = D2D1::RectF(600.f, 350.f, 750.f, 400.f);
	layoutRectCredits = D2D1::RectF(600.f, 450.f, 750.f, 500.f);
	layoutRectExit = D2D1::RectF(600.f, 550.f, 750.f, 600.f);

}

void Game::Shutdown(void)
{
//TODO: Unload Bitmaps/Brushes/Pens/Fonts

//TODO: Destroy Back Buffer

	// Shutdown DirectX.
	DestroyGraphics();
}

void Game::Input(void)
{
//TODO: Read User Input
	if (keys[VK_ESCAPE] & 0x80)
	{
		PostMessage(hWnd, WM_CLOSE, 0, 0);
	}

	// Check if the W Key is pressed:
	if (keys['W'] & 0x80)
	{
		// TODO: W Key down action.
		showCollision = true;
	}

	if (keys['P'] & 0x80)
	{
		if(isPaused == false)
			isPaused = true;
		else
			isPaused = false;
	}

	if (keys['U'] & 0x80)
	{
		if(twoPlayMode == false)
			twoPlayMode = true;
		else
			twoPlayMode = false;
	}

	if (keys['F'] & 0x80)
	{
		ToggleFullscreenMode(hWnd, FALSE, 816, 642, 0, 0);
	}

	if(isPaused == false)
	{

		if (keys['A'] & 0x80 && paddlepos.left > 0)
		{
			// TODO: Up Arrow Key down action.
			paddlepos.right -= paddlevec.height;
			paddlepos.left -= paddlevec.height;
		}
		else if (paddlepos.left <= 0)
		{
			paddlepos.right++;
			paddlepos.left++;
		}

		if (keys['D'] & 0x80 && paddlepos.right < pRT->GetSize().width/2)
		{
			paddlepos.right += paddlevec.height;
			paddlepos.left += paddlevec.height;
		}
		else if (paddlepos.right >= pRT->GetSize().width/2)
		{
			paddlepos.right--;
			paddlepos.left--;
		}

		if (keys[VK_SPACE] & 0x80 && spikePower.right > spikePower.left)
		{
			spikePower.right -= 10;
		}
		else
		{
			if(spikePower.right != 150)
				spikePower.right++;
		}
	}
}

void Game::Simulate(void)
{
	if( isPaused == false && !menu)
	{
//TODO: Simulate AI
		if( twoPlayMode == true)
		{
			if (keys[VK_LEFT] & 0x80 && paddle2pos.left > (pRT->GetSize().width/2)+60)
			{
				// TODO: Up Arrow Key down action.
				paddle2pos.right -= paddle2vec.height;
				paddle2pos.left -= paddle2vec.height;
			}
			else if (paddle2pos.left <= (pRT->GetSize().width/2)+60)
			{
				paddle2pos.right++;
				paddle2pos.left++;
			}

			if (keys[VK_RIGHT] & 0x80 && paddle2pos.right < pRT->GetSize().width)
			{
				paddle2pos.right += paddle2vec.height;
				paddle2pos.left += paddle2vec.height;
			}
			else if (paddle2pos.right >= pRT->GetSize().width)
			{
				paddle2pos.right--;
				paddle2pos.left--;
			}
		
			if (keys[VK_SHIFT] & 0x80 && spike2Power.right > spike2Power.left)
			{
				spike2Power.right -= 10;
			}
			else
			{
				if(spike2Power.right != 750)
					spike2Power.right++;
			}
		}
		else
		{
			if(paddle2pos.left > (pRT->GetSize().width/2)+60 && ballx < 300)
			{
				//if(ballx < 800)
				//{
				//	paddle2pos.right -= paddle2vec.height;
				//	paddle2pos.left -= paddle2vec.height;
				//}
				paddle2pos.right -= paddle2vec.height;
				paddle2pos.left -= paddle2vec.height;
			}
			else if (paddle2pos.left <= (pRT->GetSize().width/2)+60)
			{
				paddle2pos.right++;
				paddle2pos.left++;
			}
			if(paddle2pos.right < pRT->GetSize().width && ballx > 300 )
			{
				paddle2pos.right += paddle2vec.height;
				paddle2pos.left += paddle2vec.height;
			}
			else if (paddle2pos.right >= pRT->GetSize().width)
			{
				paddle2pos.right--;
				paddle2pos.left--;
			}
		}

	//TODO: Simulate Physics

		ballpos.left += ballvec.width;
		ballpos.top += ballvec.height;
		ballpos.right += ballvec.width;
		ballpos.bottom += ballvec.height;
		ballx += ballvec.width;

	//TODO: Simulate Collision Detection & Response

		if (ballpos.bottom > pRT->GetSize().height)
		{
			if( ballpos.right > pRT->GetSize().width/2)
				p1Point++;
			else if ( ballpos.right < pRT->GetSize().width/2)
				p2Point++;
			//ballvec.height = -ballvec.height;
			ballpos = D2D1::RectF(20, 20, 45, 45);
			ballvec = D2D1::SizeF(3, 3);
			ballx = 0;
		}

		if (ballpos.top < 0)
		{
			ballvec.height = -ballvec.height;
		}

		if (ballpos.left < 0)
		{
			ballvec.width = -ballvec.width;
			ballx++;
		}

		if (ballpos.right > pRT->GetSize().width)
		{
			ballvec.width = -ballvec.width;
			ballx--;
		}

		// Rectangle to Rectangle Intersection:
		RECT rIntersect;
		RECT rBall = {(LONG)ballpos.left, (LONG)ballpos.top, (LONG)ballpos.right, (LONG)ballpos.bottom};
		RECT rPaddle = {(LONG)paddlepos.left, (LONG)paddlepos.top, (LONG)paddlepos.right, (LONG)paddlepos.bottom};
		RECT rPaddle2 = {(LONG)paddle2pos.left, (LONG)paddle2pos.top, (LONG)paddle2pos.right, (LONG)paddle2pos.bottom};
		RECT rNet = {(LONG)netpos.left, (LONG)netpos.top, (LONG)netpos.right, (LONG)netpos.bottom};
		if(IntersectRect(&rIntersect, &rBall, &rPaddle) || IntersectRect(&rIntersect,&rBall, &rPaddle2))
		{
			// TODO: Intersection Response.
			ballvec.width = ballvec.width;
			ballvec.height = -ballvec.height;
		}
		if(IntersectRect(&rIntersect, &rBall, &rNet))
		{
			ballvec.width = -ballvec.width;
			ballvec.height = -ballvec.height+(rand()%3);
		}
	}
}

void Game::Render(void)
{
	pRT->BeginDraw();

//TODO: Clear Back Buffer

	pRT->Clear(D2DColor(CornflowerBlue));
	if(menu == true)
	{
			pRT->Clear(D2D1::ColorF(255, 255, 255, 1.0f));
			pRT->DrawBitmap(menuPic);
			pRT->DrawTextW(L"Start", wcslen(L"Start"), pTF, layoutRectStart, pBrush);
			pRT->DrawRectangle(&layoutRectStart, pBrush, 2.0f);
			pRT->DrawTextW(L"Options", wcslen(L"Options"), pTF, layoutRectOptions, pBrush);
			pRT->DrawRectangle(&layoutRectOptions, pBrush, 2.0f);
			pRT->DrawTextW(L"Credits", wcslen(L"Credits"), pTF, layoutRectCredits, pBrush);
			pRT->DrawRectangle(&layoutRectCredits, pBrush, 2.0f);
			pRT->DrawTextW(L"Exit Game", wcslen(L"Exit Game"), pTF, layoutRectExit, pBrush);
			pRT->DrawRectangle(&layoutRectExit, pBrush, 2.0f);

	}
	else
	{

//TODO: Render Game Components

		pRT->DrawBitmap(background); //Draw bitmaps here!!!
		pRT->DrawBitmap(brunetteMug, mugRct);
		pRT->FillRectangle(spikePower, pGBrush);
		pRT->DrawBitmap(blondeMug, mug2Rct);
		pRT->FillRectangle(spike2Power, pGBrush2);

	
		pBrush->SetColor(D2DColor(Black));
		wostringstream oss;
		oss << p1Point;
		pRT->DrawTextW(oss.str().c_str(), oss.str().size(),pTF ,layoutRect ,pBrush, 
			D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);

		oss.str(L""); //Clean out the OSS.
		oss << _T("     |     ");
		pRT->DrawTextW(oss.str().c_str(), oss.str().size(),pTF ,lRect ,pBrush, 
			D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);

		oss.str(L"");
		oss << p2Point;
		pRT->DrawTextW(oss.str().c_str(), oss.str().size(),pTF ,lastRect ,pBrush, 
			D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
	
		if(showCollision == true)
		{
			pBrush->SetColor(D2DColor(Black));
			pRT->DrawRectangle(netpos, pBrush, 1.0f, NULL);
			pRT->DrawRectangle(ballpos, pBrush, 1.0f, NULL);
			pRT->DrawRectangle(paddlepos, pBrush, 1.0f, NULL);
			pRT->DrawRectangle(paddle2pos, pBrush, 1.0f, NULL);
		}

		//float bitmapWidth = background->GetSize().width;
		//float bitmapHeight = background->GetSize().height;
		//pRT->DrawBitmap(background, D2D1::RectF(0, 50, 240, 98));

		//D2D1_ELLIPSE circle;
		//circle.point.x = (ballpos.left+ballpos.right)/2;
		//circle.point.y = (ballpos.top+ballpos.bottom)/2;
		//circle.radiusX = (ballpos.right-ballpos.left)/2;
		//circle.radiusY = (ballpos.bottom-ballpos.top)/2;
		//pBrush->SetColor(D2DColor(Chartreuse));
		//pRT->FillEllipse(circle, pBrush);
		pRT->DrawBitmap(volley, ballpos);

		// Render the Left Paddle.
		//pBrush->SetColor(D2DColor(Maroon));
		//pRT->FillRectangle(paddlepos, pBrush);
		float bitmapWidth = blonde->GetSize().width-1; //Get the image width.
		float bitmapHeight = blonde->GetSize().height; //Get the image height.
		//pRT->DrawBitmap(blonde, D2D1::RectF(0, 50, bitmapWidth, 50+bitmapHeight));
		float bitmapWidth2 = brunette->GetSize().width-1;
		float bitmapHeight2 = brunette->GetSize().height;

		float spriteWidth = bitmapWidth / 2;
		float spriteHeight = bitmapHeight / 1;
		float spriteWidthB = bitmapWidth2 / 2; //Number of columns.
		float spriteHeightB = bitmapHeight2 / 1; //Number of rows.

		static float spriteX = 0.0f; //static keyword will allow variable to be changed in scope 
	//									without resetting values.
		static int spriteFrame = 0;
		static int spriteFrameB = 0;

		// Render a sprite sheet (cropped)
		pRT->DrawBitmap(brunette, //Image.
			paddlepos, //D2D1_RECT_F
			1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, //Some stuff.
			/*D2D1::RectF(0, 0, spriteWidthB, spriteHeightB)*/D2D1::RectF(spriteWidthB*spriteFrameB, 0,
						spriteWidthB*spriteFrameB+spriteWidthB, spriteHeightB) ); // 
		//pRT->DrawBitmap(blonde, paddlepos);

		pRT->DrawBitmap(blonde, 
			paddle2pos,
			1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			/*D2D1::RectF(0, 0, spriteWidth, spriteHeight)*/D2D1::RectF(spriteWidth*spriteFrame, 0,
						spriteWidth*spriteFrame+spriteWidth, spriteHeight) );

		static DWORD spriteTime = GetTickCount();
		if (GetTickCount() - spriteTime > 200)
		{
			// Update frame counter (based on time)
			spriteFrame++;
			if (spriteFrame == 2)
				spriteFrame = 0;

			spriteFrameB += 1;
			if (spriteFrameB == 2)
				spriteFrameB = 0;

			spriteTime = GetTickCount();
		}
	}

#if 0
	static float spriteX = 0.0f;
	static int spriteFrame = 0;

	// Render a sprite sheet (moving, cropped, animated)
	pRT->DrawBitmap(sprite,
		D2D1::RectF(spriteX, 200, spriteX+spriteWidth, 200+spriteHeight),
		1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF(spriteWidth*spriteFrame, 0,
					spriteWidth*spriteFrame+spriteWidth, spriteHeight) );
#endif

//TODO: Present Back Buffer

    HRESULT hr = pRT->EndDraw();
    if (hr == D2DERR_RECREATE_TARGET)
    {
        //TODO: Destroy Direct2D.
		DestroyGraphics();
        //TODO: Recreate Direct2D.
		CreateGraphics(hWnd);
    }
}

void Game::OnLButtonDown(LPARAM lParam)
{
	POINT ptMouse;
	ptMouse.x = LOWORD(lParam); // Bits 0-15 contain the X Axis position
	ptMouse.y = HIWORD(lParam); // Bits 16-31 contain the Y Axis position

		if((ptMouse.x > layoutRectStart.left && ptMouse.x < layoutRectStart.right) && (ptMouse.y > layoutRectStart.top 
			&& ptMouse.y < layoutRectStart.bottom) /*&& start == false*/) 
		{
			//Does stuff here. 
			/*start = true;*/
			menu = false;

			FMOD_RESULT result = FMOD::System_Create(&sys); 

			//Now load the music
			FMOD::Sound* sound;
			sys->setOutput(FMOD_OUTPUTTYPE_DSOUND);
			sys->init(32, FMOD_INIT_NORMAL, 0);
			result = sys->createSound("song.mp3", FMOD_LOOP_NORMAL, NULL, &sound);

			//Initializing the channel object
			FMOD::Channel* channel = 0; 

			//Now play the music...
			result = sys->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
			sys->update(); 

			InvalidateRect(hWnd, NULL, TRUE);
		}
		
		if((ptMouse.x > layoutRectOptions.left && ptMouse.x < layoutRectOptions.right) && (ptMouse.y > layoutRectOptions.top 
			&& ptmouse.y < layoutRectOptions.bottom) /*&& opt == false*/)
		{
			//Does stuff here. 
			/*opt = true;*/
			ToggleFullscreenMode(hWnd, TRUE, 816, 642, 1, 1);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		
		if((ptMouse.x > layoutRectCredits.left && ptMouse.x < layoutRectCredits.right) && (ptMouse.y > layoutRectCredits.top 
			&& ptMouse.y < layoutRectCredits.bottom) /*&& dispCred == false*/) 
		{
			//Does stuff here. 
			/*dispCred = true;*/
			pRT->DrawBitmap(menuPic);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		
		if((ptMouse.x > layoutRectExit.left && ptMouse.x < layoutRectExit.right) && (ptMouse.y > layoutRectExit.top 
			&& ptMouse.y < layoutRectExit.bottom))
		{
			//Does stuff here. 
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
}

void Game::ToggleFullscreenMode(HWND hWnd, BOOL bFullScreen,
			int iWidth, int iHeight, int iBpp, int iRefreshRate)
{
	static WINDOWPLACEMENT wp = {0};
	static HMENU hMenu = NULL;

	if(bFullScreen)
	{
		// Remember the window position.
		wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(hWnd, &wp);
		// Remove the window's title bar.
		SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP);
		// Put the changes to the window into effect.
		SetWindowPos(hWnd, 0, 0, 0, 0, 0,
			SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
		// Remember the menu, then remove it.
		hMenu = GetMenu(hWnd);
		SetMenu(hWnd, NULL);
		// Switch to the requested display mode.
		//SetDisplayMode(iWidth, iHeight, iBpp, iRefreshRate);
		// Position the window to cover the entire screen.
		SetWindowPos(hWnd, HWND_TOPMOST,
			0, 0,
			GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			SWP_SHOWWINDOW);
		// Remove the cursor.
		//ShowCursor(FALSE);
	}
	else
	{
		// Restore the window's title bar.
		SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		// Put the changes to the window into effect.
		SetWindowPos(hWnd, 0, 0, 0, 0, 0,
			SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
		// Restore the window's menu.
		SetMenu(hWnd, hMenu);
		// Restore the display mode.
		//SetDisplayMode(0, 0, 0, 0);
		// Restore the window's original position.
		SetWindowPlacement(hWnd, &wp);
		// Restore the cursor.
		ShowCursor(TRUE);
	}
}

HRESULT Game::CreateGraphics(HWND hWnd)
{
	// Initialize the Direct2D Factory.
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the Direct2D Factory."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}


	// Get the dimensions of the client.
	RECT rc;
	GetClientRect(hWnd, &rc);

	// Initialize a Direct2D Size Structure.
	D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);


	// Create the Direct2D Render Target.
	hr = pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, size), &pRT);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the Direct2D Render Target."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}
	pRT->SetDpi(96.0f, 96.0f);


	// Create the Direct2D Solid Color Brush.
	hr = pRT->CreateSolidColorBrush(D2D1::ColorF(0x0), &pBrush);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the Direct2D Solid Color Brush."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	D2D1_GRADIENT_STOP gradientStops[2];
	gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Yellow, 1);
	gradientStops[0].position = 0.0f;
	gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Green, 1);
	gradientStops[1].position = 1.0f;

	hr = pRT->CreateGradientStopCollection(gradientStops, 2, &pGradStop);
	if(FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the Direct2D Gradient Stop Collection."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}


	hr = pRT->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(
                        D2D1::Point2F(0, 0),
                        D2D1::Point2F(150, 150)),
		pGradStop, &pGBrush);
	if(FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to create the Direct2D Linear Gradient Brush."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	hr = pRT->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(
						D2D1::Point2F(500, 0),
						D2D1::Point2F(750, 750)),
		pGradStop, &pGBrush2);
	if(FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to create the Direct2D Linear Gradient Brush(2)."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}


	// Initialize the DirectWrite Factory.
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
			(IUnknown**)&pDWFactory);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the DirectWrite Factory."),
			_T("DirectWrite Error"), MB_OK | MB_ICONERROR);
		return hr;
	}


	// Create the DirectWrite Text Format.
	hr = pDWFactory->CreateTextFormat(_T("Veranda"), NULL,
		DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		24, _T(""), &pTF);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the DirectWrite Text Format."),
			_T("DirectWrite Error"), MB_OK | MB_ICONERROR);
		return hr;
	}
	//pTF->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
	//pTF->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	//pTF->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	//Create the COM Imaging Fafcotry
	 hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pWICFactory)
		);
	 if(FAILED(hr))
	 {
		 MessageBox(HWND_DESKTOP, _T("ERROR: Failed to create WIC Factory."),
			 _T("WIC Error"), MB_OK | MB_ICONERROR);
		 return hr;
	 }

	 hr = LoadBitmapFromFile(_T("background.png"), &background);
	 if(FAILED(hr))
	 {
			MessageBox(HWND_DESKTOP, _T("ERROR: Failed to load background.png."),
			 _T("WIC Error"), MB_OK | MB_ICONERROR);
		 return hr;
	 }

	 hr = LoadBitmapFromFile(_T("blondeMove1.png"), &blonde);
	 if(FAILED(hr))
	 {
		 MessageBox(HWND_DESKTOP, _T("ERROR: Failed to load the blonde.png."),
			 _T("WIC Error"), MB_OK | MB_ICONERROR);
		 return hr;
	 }

	 hr = LoadBitmapFromFile(_T("brunetteMove1.png"), &brunette);
	 if(FAILED(hr))
	 {
		 MessageBox(HWND_DESKTOP, _T("ERROR: Failed to load the brunette.png."),
			 _T("WIC Error"), MB_OK | MB_ICONERROR);
		 return hr; 
	 }

	 hr = LoadBitmapFromFile(_T("volley.png"), &volley);
	 if(FAILED(hr))
	 {
		 MessageBox(HWND_DESKTOP, _T("ERROR: Failed to load the volley.png."),
			 _T("WIC Error"), MB_OK | MB_ICONERROR);
		 return hr;
	 }

	 hr = LoadBitmapFromFile(_T("brunetteMug.png"), &brunetteMug);
	 if(FAILED(hr))
	 {
		 MessageBox(HWND_DESKTOP, _T("ERROR: Failed to load the brunetteMug.png."),
			 _T("WIC Error"), MB_OK | MB_ICONERROR);
		 return hr;
	 }

	 hr = LoadBitmapFromFile(_T("blondeMug.png"), &blondeMug);
	 if(FAILED(hr))
	 {
		 MessageBox(HWND_DESKTOP, _T("ERROR: Failed to load the blondeMug.png."),
			 _T("WIC Error"), MB_OK | MB_ICONERROR);
		 return hr;
	 }

	 hr = LoadBitmapFromFile(_T("beach.png"), &menuPic);
	 if(FAILED(hr))
	 {
		 MessageBox(HWND_DESKTOP, _T("ERROR: Failed to load the beach.png."),
			 _T("WIC Error"), MB_OK | MB_ICONERROR);
		 return hr;
	 }

	return S_OK; // Success!
}

void Game::DestroyGraphics(void)
{
	SafeRelease(&background);
	SafeRelease(&blonde);
	SafeRelease(&brunette);
	SafeRelease(&menuPic);
	SafeRelease(&volley);
	SafeRelease(&brunetteMug);
	SafeRelease(&blondeMug);
	SafeRelease(&pWICFactory);

	SafeRelease(&pTF);
	SafeRelease(&pDWFactory);
	SafeRelease(&pBrush);
	SafeRelease(&pGBrush);
	SafeRelease(&pGBrush2);
	SafeRelease(&pGradStop);
	SafeRelease(&pRT);
	// Release the Direct2D Factory.
	SafeRelease(&pD2DFactory);
}

HRESULT Game::LoadBitmapFromFile(LPCTSTR strFileName, ID2D1Bitmap** ppBitmap )
{
	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode * pSource = NULL;
	IWICFormatConverter* pConverter = NULL;

	HRESULT hr = pWICFactory->CreateDecoderFromFilename(
    strFileName,
    NULL,
    GENERIC_READ,
    WICDecodeMetadataCacheOnLoad,
    &pDecoder);

	if (SUCCEEDED(hr))
    {
        // Create the initial frame.
        hr = pDecoder->GetFrame(0, &pSource);
    }
	
    if (SUCCEEDED(hr))
    {
        // Convert the image format to 32bppPBGRA
        // (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
        hr = pWICFactory->CreateFormatConverter(&pConverter);
    }

	if(SUCCEEDED(hr))
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

	if (SUCCEEDED(hr))
    {
        // Create a Direct2D bitmap from the WIC bitmap.
        hr = pRT->CreateBitmapFromWicBitmap(
            pConverter,
            NULL,
            ppBitmap
            );
    }

	SafeRelease(&pConverter);
	SafeRelease(&pSource);
	SafeRelease(&pDecoder);


	return S_OK;
}
