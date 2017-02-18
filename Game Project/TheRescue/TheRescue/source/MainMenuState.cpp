/*
	File: CMainMenuState.cpp
	Author: Gabriel Islas
	Class: SGD 1212
	Purpose: To handle all of the main menu actions
*/

#include "MainMenuState.h"
#include "BitmapFont.h"

// SGD Wrappers
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../SGD Wrappers/SGD_String.h"

// Game State Machine
#include "Game.h"
#include "GameplayState.h"
#include "CreditsState.h"
#include "HighScoresState.h"
#include "HowToPlayState.h"
#include "OptionsState.h"


// SINGLETON
/*static*/ CMainMenuState*	CMainMenuState::GetInstance( void )
{
	// Store an instance in global memory
	static CMainMenuState s_Instance;

	// Return the instance
	return &s_Instance;
}


CMainMenuState::CMainMenuState(void)
{
	// Set the data members to 'safe' values
	m_pD3D = nullptr;
	m_pDI = nullptr;
	m_pTM = nullptr;
	m_pXA = nullptr;

	// Invalid Asset = -1
	m_nMenuCursorID			= -1;
	m_nBackgroundMusicID    = -1;
	m_nSFXSelectID			= -1;
	m_nTitleFontID			= -1;
	m_nBackgroundImageID	= -1;
	m_nMenuItemsID			= -1;
	
	m_nCursorY = 100;

	m_fMusicVolume = 1.0f;
	m_nDispMusicVolume = 100;
}


CMainMenuState::~CMainMenuState(void)
{
}

void CMainMenuState::Enter( void )
{
	// Store the SGD Wrapper singletons
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pXA = CSGD_XAudio2::GetInstance();


	m_nMenuCursorID = m_pTM->LoadTexture(_T("resource/graphics/menu/GI_MenuCursor.png"), 
										D3DCOLOR_XRGB(255, 255, 255));

	m_nTitleFontID = m_pTM->LoadTexture(_T("resource/graphics/fonts/GI_MenuTitle.png"));

	m_nMenuItemsID = m_pTM->LoadTexture(_T("resource/graphics/fonts/GI_MenuItems.png"));

	m_nBackgroundImageID = m_pTM->LoadTexture(_T("resource/graphics/menu/GI_MenuBackground.png"));

	m_nBackgroundMusicID = m_pXA->MusicLoadSong( _T("resource/sounds/GI_TitleMusic.xwm") );

	m_nSFXSelectID = m_pXA->SFXLoadSound(_T("resource/sounds/GI_SilencedPistol.wav"));

	ifstream fin;
	fin.open("resource/settings.sav");
	if( fin.is_open() )
	{
		int dispMVol;
		float MVol;
		int dispSVol;
		float SVol;
		while(!fin.eof())
		{
			fin >> dispMVol;
			fin >> MVol;
			fin >> dispSVol;
			fin >> SVol;
		}
		m_pXA->MusicSetMasterVolume(MVol);
		m_pXA->SFXSetMasterVolume(SVol);
	}
	fin.close();

	// Play the background music
	m_pXA->MusicPlaySong( m_nBackgroundMusicID, true);

}
void CMainMenuState::Exit( void )
{
	// Unload the assets
	if( m_nMenuCursorID != -1)
	{
		m_pTM->UnloadTexture(m_nMenuCursorID);
		m_nMenuCursorID = -1;
	}
	if( m_nTitleFontID != -1)
	{
		m_pTM->UnloadTexture(m_nTitleFontID);
		m_nTitleFontID = -1;
	}
	if( m_nMenuItemsID != -1)
	{
		m_pTM->UnloadTexture(m_nMenuItemsID);
		m_nMenuItemsID = -1;
	}
	if( m_nBackgroundImageID != -1)
	{
		m_pTM->UnloadTexture(m_nBackgroundImageID);
		m_nBackgroundImageID = -1;
	}
	if( m_nBackgroundMusicID != -1)
	{
		m_pXA->MusicStopSong(m_nBackgroundMusicID);
		m_pXA->MusicUnloadSong(m_nBackgroundMusicID);
		m_nBackgroundMusicID = -1;
	}
	if( m_nSFXSelectID != -1)
	{
		m_pXA->SFXStopSound(m_nSFXSelectID);
		m_pXA->SFXUnloadSound(m_nSFXSelectID);
		m_nSFXSelectID = -1;
	}


	// Reset the data members to 'safe' values
	m_pD3D = nullptr;
	m_pDI = nullptr;
	m_pTM = nullptr;
	m_pXA = nullptr;
}

bool CMainMenuState::Input( void )
{

	if( m_pDI->KeyPressed( DIK_ESCAPE ) == true)
		m_nCursorY = 280;

	if( m_pDI->KeyPressed( DIK_RETURN ) == true && m_nCursorY == 280)
		return false;

	if( m_pDI->KeyPressed( DIK_RETURN ) == true && m_nCursorY == 100 )
	{
		CGame::GetInstance()->ChangeState(CGameplayState::GetInstance());
		return true;
	}

	if( m_pDI->KeyPressed( DIK_RETURN ) == true && m_nCursorY == 130 )
	{
		//TODO: Load game
	}

	if( m_pDI->KeyPressed(DIK_RETURN ) == true && m_nCursorY == 160 )
	{
		CGame::GetInstance()->ChangeState(CHowToPlayState::GetInstance());
		return true;
	}

	if( m_pDI->KeyPressed( DIK_RETURN ) == true && m_nCursorY == 190)
	{
		CGame::GetInstance()->ChangeState(CHighScoresState::GetInstance());
		return true;
	}

	if( m_pDI->KeyPressed( DIK_RETURN ) == true && m_nCursorY == 220)
	{
		CGame::GetInstance()->ChangeState(COptionsState::GetInstance());
		return true;
	}

	if( m_pDI->KeyPressed( DIK_RETURN ) == true && m_nCursorY == 250 )
	{
		CGame::GetInstance()->ChangeState(CCreditsState::GetInstance());
		return true;
	}

	if( m_pDI->KeyPressed(DIK_UP) == true)
	{
		m_nCursorY -= 30;
		m_pXA->SFXPlaySound(m_nSFXSelectID, false);
	}
	else if( m_pDI->KeyPressed(DIK_DOWN) == true)
	{
		m_nCursorY += 30;
		m_pXA->SFXPlaySound(m_nSFXSelectID, false);
	}

#if 0
	if (m_pDI->KeyPressed(DIK_LEFT) == true && m_nCursorY == 110 && m_fMusicVolume > 0.0f && m_nDispMusicVolume > 0)
	{
		m_fMusicVolume -= 0.05f;
		m_nDispMusicVolume -= 5;
	}
	else if( m_pDI->KeyPressed(DIK_RIGHT ) == true && m_nCursorY == 110 && m_fMusicVolume < 1.0f 
		&& m_nDispMusicVolume < 100)
	{
		m_fMusicVolume += 0.05f;
		m_nDispMusicVolume += 5;
	}
#endif

	return true;
}

void CMainMenuState::Update( float fElapsedTime )
{
	if( m_nCursorY > 280)
		m_nCursorY = 100;
	else if( m_nCursorY < 100)
		m_nCursorY = 280;/*

	if( m_fMusicVolume < 0.0f || m_nDispMusicVolume < 0)
	{
		m_fMusicVolume = 0.0f;
		m_nDispMusicVolume = 0;
	}

	m_pXA->MusicSetMasterVolume( m_fMusicVolume );*/
}

void CMainMenuState::Render( void )
{
	m_pTM->Draw( m_nBackgroundImageID, -150, -300, 2.0f, 2.0f);
	CBitmapFont title;
	CBitmapFont items;
	title.Initialize(32, 33, 10, 6, m_nTitleFontID, ' ', true);
	title.Print("THE RESCUE", 50, 50, 1.0f, D3DCOLOR_XRGB(0, 0, 255));
	items.Initialize(16, 16, 20, 3, m_nMenuItemsID, ' ', true);
	items.Print("START GAME", 70, 100, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
	items.Print("LOAD GAME", 70, 130, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
	items.Print("HOW TO PLAY", 70, 160, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
	items.Print("HIGH SCORES", 70, 190, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
	items.Print("OPTIONS", 70, 220, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
	items.Print("CREDITS", 70, 250, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
	items.Print("EXIT", 70, 280, 1.0f, D3DCOLOR_XRGB(255, 255, 255));

	m_pD3D->Clear(0,0,0);

	m_pTM->Draw( m_nMenuCursorID, 50, m_nCursorY );
}