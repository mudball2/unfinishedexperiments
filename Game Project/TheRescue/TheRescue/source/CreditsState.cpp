/*
	File: CCreditsState.h
	Author: Gabriel Islas
	Class: SGD 1212
	Purpose: To display scrolling credits of the game to the user.
*/
#include "CreditsState.h"


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

CCreditsState* CCreditsState::GetInstance( void )
{
	static CCreditsState s_Instance;

	return &s_Instance;
}

CCreditsState::CCreditsState(void)
{
	// Set the data members to 'safe' values
	m_pD3D = nullptr;
	m_pDI = nullptr;
	m_pTM = nullptr;
	m_pXA = nullptr;

	m_nLettersID	= -1;
	m_nBackgroundMusicID	= -1;

	m_nPosY	= 550;
	m_nVelY = 1; 
	for(int i = 0; i < 45; i++)
		cred[i] = "";
}


CCreditsState::~CCreditsState(void)
{
}

void CCreditsState::Enter( void )
{
	// Store the SGD Wrapper singletons
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pXA = CSGD_XAudio2::GetInstance();


	m_nPosY	= 550;

	m_nLettersID = m_pTM->LoadTexture(_T("resource/graphics/fonts/GI_MenuItems.png"));
	m_nBackgroundMusicID	= m_pXA->MusicLoadSong( _T("resource/sounds/GI_Credits.xwm") );

	// Play the background music
	m_pXA->MusicSetMasterVolume( m_pXA->MusicGetMasterVolume() );
	m_pXA->MusicPlaySong( m_nBackgroundMusicID, true);

	
	credits.Initialize(16, 16, 20, 3, m_nLettersID, ' ', true);

	int count = 0;
	output.open("resource/Credits_Gi.txt");
	if(output.is_open())
	{
		while(!output.eof())
		{
			count++;
			output >> s;
			if( count <= 45)
				cred[count - 1] = s;
		}
	}
}
void CCreditsState::Exit( void )
{
	if( m_nLettersID != -1)
	{
		m_pTM->UnloadTexture(m_nLettersID);
		m_nLettersID = -1;
	}
	if( m_nBackgroundMusicID != -1)
	{
		m_pXA->MusicStopSong(m_nBackgroundMusicID);
		m_pXA->MusicUnloadSong(m_nBackgroundMusicID);
		m_nBackgroundMusicID = -1;
	}
	for(int i = 0; i < 45; i++)
		cred[i] = "";

	output.close();

	// Reset the data members to 'safe' values
	m_pD3D = nullptr;
	m_pDI = nullptr;
	m_pTM = nullptr;
	m_pXA = nullptr;
}

bool CCreditsState::Input( void )
{
	if( m_pDI->KeyPressed( DIK_ESCAPE ) == true )
	{
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		return true;
	}
	return true;
}
void CCreditsState::Update( float fElapsedTime )
{
	m_nPosY -= m_nVelY * fElapsedTime;
	if(m_nPosY < -1000 )
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
}
void CCreditsState::Render( void )
{
	for( int i = 0; i < 45; i++)
		credits.Print(cred[i].c_str(), 50, m_nPosY+(i*20), 1.0f, D3DCOLOR_XRGB(255, 0, 0 ) );
	m_nPosY--;
}