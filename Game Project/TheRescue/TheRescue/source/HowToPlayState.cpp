/*
	File: CHowToPlayState.cpp
	Author: Gabriel Islas
	Class: SGD 1212
	Purpose: To show a slideshow of instructions for the game.
*/
#include "HowToPlayState.h"
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
#include "MainMenuState.h"
#include "OptionsState.h"


CHowToPlayState* CHowToPlayState::GetInstance( void )
{
	static CHowToPlayState s_Instance;

	return &s_Instance;
}

CHowToPlayState::CHowToPlayState(void)
{
	// Set the data members to 'safe' values
	m_pD3D = nullptr;
	m_pDI = nullptr;
	m_pTM = nullptr;
	m_pXA = nullptr;

	m_nLettersID	= -1; 
	m_nBackgroundMusicID	= -1;
	m_nBackgroundImage1ID	= -1;
	m_nBackgroundImage2ID	= -1;
	m_nBackgroundImage3ID	= -1;
	m_nBackgroundImage4ID	= -1;
	m_nBackgroundImage5ID	= -1;
	m_nBackgroundImage6ID	= -1;
	m_nCurPage	= 0;
}


CHowToPlayState::~CHowToPlayState(void)
{
}

void CHowToPlayState::Enter( void )
{
	// Store the SGD Wrapper singletons
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pXA = CSGD_XAudio2::GetInstance();

	m_nLettersID = m_pTM->LoadTexture(_T("resource/graphics/fonts/GI_MenuItems.png"));
	m_nBackgroundMusicID	= m_pXA->MusicLoadSong( _T("resource/sounds/GI_TitleMusic.xwm") );
	m_nBackgroundImage1ID	= m_pTM->LoadTexture(_T("resource/graphics/menu/GI_Slideshow1.png"));
	m_nBackgroundImage2ID	= m_pTM->LoadTexture(_T("resource/graphics/menu/GI_Slideshow2.png"));
	m_nBackgroundImage3ID	= m_pTM->LoadTexture(_T("resource/graphics/powerups/GI_health.png"));
	m_nBackgroundImage4ID	= m_pTM->LoadTexture(_T("resource/graphics/powerups/GI_magnum.png"));
	m_nBackgroundImage5ID	= m_pTM->LoadTexture(_T("resource/graphics/soldier/GI_soldier_aim.png"));
	m_nBackgroundImage6ID	= m_pTM->LoadTexture(_T("resource/graphics/sentinel/GI_sentinel_aim.png"));

	// Play the background music
	m_pXA->MusicSetMasterVolume( m_pXA->MusicGetMasterVolume() );
	m_pXA->MusicPlaySong( m_nBackgroundMusicID, true);
}
void CHowToPlayState::Exit( void )
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
	if( m_nBackgroundImage1ID != -1 )
	{
		m_pTM->UnloadTexture(m_nBackgroundImage1ID);
		m_nBackgroundImage1ID = -1;
	}
	if( m_nBackgroundImage2ID != -1 )
	{
		m_pTM->UnloadTexture(m_nBackgroundImage2ID);
		m_nBackgroundImage2ID = -1;
	}
	if( m_nBackgroundImage3ID != -1 )
	{
		m_pTM->UnloadTexture(m_nBackgroundImage3ID);
		m_nBackgroundImage3ID = -1;
	}
	if( m_nBackgroundImage4ID != -1 )
	{
		m_pTM->UnloadTexture(m_nBackgroundImage4ID);
		m_nBackgroundImage4ID = -1;
	}
	if( m_nBackgroundImage5ID != -1 )
	{
		m_pTM->UnloadTexture(m_nBackgroundImage5ID);
		m_nBackgroundImage5ID = -1;
	}
	if( m_nBackgroundImage6ID != -1 )
	{
		m_pTM->UnloadTexture(m_nBackgroundImage6ID);
		m_nBackgroundImage6ID = -1;
	}

	// Reset the data members to 'safe' values
	m_pD3D = nullptr;
	m_pDI = nullptr;
	m_pTM = nullptr;
	m_pXA = nullptr;
}

bool CHowToPlayState::Input( void )
{

	if( m_pDI->KeyPressed( DIK_ESCAPE ) == true )
	{
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		return true;
	}
	if( m_pDI->KeyPressed( DIK_RIGHT ) == true )
	{
		m_nCurPage++;
	}
	if( m_pDI->KeyPressed( DIK_LEFT ) == true )
	{
		m_nCurPage--;
	}
	return true;
}
void CHowToPlayState::Update( float fElapsedTime )
{
	if( m_nCurPage > 2 )
		m_nCurPage = 2;
	if( m_nCurPage < 0 )
		m_nCurPage = 0;
}
void CHowToPlayState::Render( void )
{
	CBitmapFont words;
	words.Initialize(16, 16, 20, 3, m_nLettersID, ' ', true);
	words.Print("HOW TO PLAY", 100, 20, 2.0f, D3DCOLOR_XRGB( 0, 0, 255 ));

	if( m_nCurPage == 0)
	{
		m_pTM->Draw(m_nBackgroundImage1ID, 100, 200, 1.0f, 1.0f);
		words.Print("BRIEFING:", 150, 90, 1.0f, D3DCOLOR_XRGB( 255, 255, 255) );
		words.Print("WELCOME AGENT. YOU ARE TO INFILTRATE" , 
			150, 120, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
		words.Print("AN ENEMY BASE TO RESCUE A SOLDIER THAT", 150, 140, 1.0f, D3DCOLOR_XRGB( 255, 255, 255));
		words.Print("POSSESSES VALUABLE KNOWLEDGE ABOUT THE ", 150, 160, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
		words.Print("BLACK BOX. GET IN. GET OUT. WE WERE NEVER", 150, 180, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
		words.Print("THERE. GOOD LUCK! ", 150, 200, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
		words.Print("CONTROLS:", 150, 240, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
		words.Print("-MOVE WITH THE ARROW KEYS", 150, 260, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
		words.Print("-SHOOT WITH SPACE BAR", 150, 280, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
		words.Print(">", 750, 550, 2.0f, D3DCOLOR_XRGB(0, 255, 0));
	}
	if( m_nCurPage == 1)
	{
		m_pTM->Draw(m_nBackgroundImage2ID, 100, 200, 1.0f, 1.0f);
		words.Print("SPECIAL CONTROLS:", 150, 90, 1.0f, D3DCOLOR_XRGB( 255, 255, 255) );
		words.Print("-TO PERFORM A WALL HUG PRESS E", 150, 140, 1.0f, D3DCOLOR_XRGB( 255, 255, 255));
		words.Print("-TO HANG FROM A LEDGE OR PULL UP: Q", 150, 160, 1.0f, D3DCOLOR_XRGB( 255, 255, 255) );
		words.Print("-TO SWITCH WEAPON PRESS W", 150, 180, 1.0f, D3DCOLOR_XRGB(255, 255, 255) );
		words.Print("<", 20, 550, 2.0f, D3DCOLOR_XRGB(0, 255, 0));
		words.Print(">", 750, 550, 2.0f, D3DCOLOR_XRGB(0, 255, 0));
	}
	if( m_nCurPage == 2)
	{
		m_pTM->Draw(m_nBackgroundImage3ID, 150, 150, 1.0f, 1.0f);
		m_pTM->Draw(m_nBackgroundImage4ID, 200, 150, 1.0f, 1.0f);
		m_pTM->Draw(m_nBackgroundImage5ID, 150, 250, 1.0f, 1.0f);
		m_pTM->Draw(m_nBackgroundImage6ID, 200, 250, 1.0f, 1.0f);
		words.Print("PICK UP THESE:", 150, 90, 1.0f, D3DCOLOR_XRGB( 255, 255, 255) );
		words.Print("AVOID+SHOOT THESE:", 150, 200, 1.0f, D3DCOLOR_XRGB( 255, 255 ,255 ) );
		words.Print("<", 20, 550, 2.0f, D3DCOLOR_XRGB(0, 255, 0));
		//words.Print(">", 750, 550, 2.0f, D3DCOLOR_XRGB(0, 255, 0));
	}
	m_pD3D->Clear(0, 0, 0);
}