/*
	File: CIntroState.cpp
	Author: Gabriel Islas
	Class: SGD 1212
	Purpose: To display an intro based on a timer. Can be skipped with any 
*/

#include "IntroState.h"
#include "BitmapFont.h"


// SGD Wrappers
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"

// Game State Machine
#include "Game.h"
#include "GameplayState.h"
#include "MainMenuState.h"
#include "HighScoresState.h"
#include "HowToPlayState.h"
#include "OptionsState.h"

CIntroState* CIntroState::GetInstance( void )
{
	static CIntroState s_Instance;

	return &s_Instance;
}

CIntroState::CIntroState(void)
{
	m_pD3D = nullptr;
	m_pDI = nullptr;
	m_pTM = nullptr;
	m_pXA = nullptr;

	m_nFontID = -1;

	m_nTextOpacity = 0;
}


CIntroState::~CIntroState(void)
{
}


void CIntroState::Enter( void )
{
	m_pD3D	= CSGD_Direct3D::GetInstance();
	m_pDI	= CSGD_DirectInput::GetInstance();
	m_pTM	= CSGD_TextureManager::GetInstance();
	m_pXA	= CSGD_XAudio2::GetInstance();

	m_nFontID = m_pTM->LoadTexture(_T("resource/graphics/fonts/GI_MenuItems.png"));
}

void CIntroState::Exit( void )
{
	if( m_nFontID != -1)
	{
		m_pTM->UnloadTexture(m_nFontID);
		m_nFontID = -1;
	}

	// Reset the data members to 'safe' values
	m_pD3D = nullptr;
	m_pDI = nullptr;
	m_pTM = nullptr;
	m_pXA = nullptr;
}

bool CIntroState::Input( void )
{
	if( m_pDI->CheckKeys() != 0 )
	{
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		return true;
	}

	return true;
}
void CIntroState::Update( float fElapsedTime )
{
	static float m_fElapsedTime = 0; 
	m_fElapsedTime += fElapsedTime;

	m_nTextOpacity++;
	//if( m_nTextOpacity == 0 || m_nTextOpacity < 0 )
	//{
	//	m_nTextOpacity += 15; //+= m_fElapsedTime;
	//	//CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
	//} 
	if( m_nTextOpacity >= 255 )
	{
		m_nTextOpacity--;
	}

	if(m_fElapsedTime > 5.0f)
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
}
void CIntroState::Render( void )
{
	m_pD3D->Clear(0, 0, 0); // Clear the screen to black. 

	CBitmapFont text;
	text.Initialize(16, 16, 20, 3, m_nFontID, ' ', true);
	text.Print("MUDBALL GAMES PRESENTS", 200, 250, 1.0f, D3DCOLOR_ARGB(m_nTextOpacity, 0, 255, 0));
}