/*
	File: COptionsState.cpp
	Author: Gabriel Islas
	Class: SGD 1212
	Purpose: To handle all of the configuration settings for the game.
*/

#include "OptionsState.h"
#include "BitmapFont.h"
#include <iostream>
using namespace std;
#include <sstream>
#include <fstream>

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
#include "MainMenuState.h"

COptionsState* COptionsState::GetInstance( void )
{
	static COptionsState s_Instance;

	return &s_Instance;
}

COptionsState::COptionsState(void)
{

	// Set the data members to 'safe' values
	m_pD3D = nullptr;
	m_pDI = nullptr;
	m_pTM = nullptr;
	m_pXA = nullptr;

	m_nFontID	= -1;
	m_nCursorID	= -1;
	m_nBackgroundMusicID = -1;
	m_nSFXID	= -1;

	m_nDispVolumeMusic = 100;
	m_nDispVolumeSfx	= 100;
	m_fMusicVolume	= 1.0f;
	m_fSfxVolume	= 1.0f;
	m_nCursorY	= 80;
}


COptionsState::~COptionsState(void)
{
}

void COptionsState::Enter( void )
{
	// Store the SGD Wrapper singletons
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pXA = CSGD_XAudio2::GetInstance();

	m_nFontID	= m_pTM->LoadTexture(_T("resource/graphics/fonts/GI_MenuTitle.png"));
	m_nCursorID = m_pTM->LoadTexture(_T("resource/graphics/menu/GI_MenuCursor.png"));

	m_nBackgroundMusicID	= m_pXA->MusicLoadSong(_T("resource/sounds/GI_TitleMusic.xwm"));
	m_nSFXID	= m_pXA->SFXLoadSound(_T("resource/sounds/GI_SilencedPistol.wav"));

	m_fMusicVolume = m_pXA->MusicGetMasterVolume();
	m_fSfxVolume = m_pXA->SFXGetMasterVolume();
	m_nDispVolumeMusic = m_fMusicVolume * 100;
	m_nDispVolumeSfx	= m_fSfxVolume * 100;

	// Play the background music
	m_pXA->MusicPlaySong( m_nBackgroundMusicID, true);
}
void COptionsState::Exit( void )
{
	if( m_nFontID != -1)
	{
		m_pTM->UnloadTexture(m_nFontID);
		m_nFontID = -1;
	}
	if( m_nCursorID != -1 )
	{
		m_pTM->UnloadTexture(m_nCursorID);
		m_nCursorID = -1;
	}
	if( m_nBackgroundMusicID != -1)
	{
		m_pXA->MusicStopSong(m_nBackgroundMusicID);
		m_pXA->MusicUnloadSong(m_nBackgroundMusicID);
		m_nBackgroundMusicID = -1;
	}
	if( m_nSFXID != -1 )
	{
		m_pXA->SFXStopSound(m_nSFXID);
		m_pXA->SFXUnloadSound(m_nSFXID);
		m_nSFXID = -1;
	}

	ofstream fout;
	fout.open("resource/settings.sav");
	if( !fout.is_open() )
	{
		cerr << "File did not open!" << endl;
	}
	else
	{
		fout << m_nDispVolumeMusic << endl;
		fout << m_fMusicVolume << endl;
		fout << m_nDispVolumeSfx << endl;
		fout << m_fSfxVolume << endl;
	}
	fout.close();
	// Reset the data members to 'safe' values
	m_pD3D = nullptr;
	m_pDI = nullptr;
	m_pTM = nullptr;
	m_pXA = nullptr;
}

bool COptionsState::Input( void )
{
	if( m_pDI->KeyPressed( DIK_ESCAPE ) == true )
	{
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		return true;
	}

	if( m_pDI->KeyPressed(DIK_UP) == true)
	{
		m_nCursorY -= 30;
		m_pXA->SFXPlaySound(m_nSFXID, false);
	}
	else if( m_pDI->KeyPressed(DIK_DOWN) == true)
	{
		m_nCursorY += 30;
		m_pXA->SFXPlaySound(m_nSFXID, false);
	}

	if (m_pDI->KeyDown(DIK_LEFT) == true && m_nCursorY == 80 && m_fMusicVolume > 0.0f && m_nDispVolumeMusic > 0)
	{
		m_fMusicVolume -= 0.01f;
		m_nDispVolumeMusic -= 1;
	}
	else if( m_pDI->KeyDown(DIK_RIGHT ) == true && m_nCursorY == 80 && m_fMusicVolume < 1.0f 
		&& m_nDispVolumeMusic < 100)
	{
		m_fMusicVolume += 0.01f;
		m_nDispVolumeMusic += 1;
	}

	if (m_pDI->KeyDown(DIK_LEFT) == true && m_nCursorY == 110 && m_fSfxVolume > 0.0f && m_nDispVolumeSfx > 0)
	{
		m_fSfxVolume -= 0.01f;
		m_nDispVolumeSfx -= 1;
		
		m_pXA->SFXPlaySound(m_nSFXID, false);
	}
	else if( m_pDI->KeyDown(DIK_RIGHT ) == true && m_nCursorY == 110 && m_fSfxVolume < 1.0f 
		&& m_nDispVolumeSfx < 100)
	{
		m_fSfxVolume += 0.01f;
		m_nDispVolumeSfx += 1;
		
		m_pXA->SFXPlaySound(m_nSFXID, false);
	}
	return true;
}
void COptionsState::Update( float fElapsedTime )
{
	if( m_nCursorY > 110)
		m_nCursorY = 80;
	else if( m_nCursorY < 80)
		m_nCursorY = 110;

	if( m_fMusicVolume < 0.0f || m_nDispVolumeMusic < 0)
	{
		m_fMusicVolume = 0.0f;
		m_nDispVolumeMusic = 0;
	}

	if( m_fSfxVolume < 0.0f || m_nDispVolumeSfx < 0)
	{
		m_fSfxVolume = 0.0f;
		m_nDispVolumeSfx = 0;
	}

	m_pXA->MusicSetMasterVolume( m_fMusicVolume );
	m_pXA->SFXSetMasterVolume( m_fSfxVolume );
}
void COptionsState::Render( void )
{
	ostringstream oss;
	ostringstream oss2;
	CBitmapFont words;
	m_pTM->Draw(m_nCursorID, 70, m_nCursorY, 1.0f, 1.0f);
	words.Initialize(32, 33, 10, 6, m_nFontID, ' ', true);
	words.Print("OPTIONS", 100, 20, 1.0f, D3DCOLOR_XRGB( 0, 0, 255 ));

	oss << "MUSIC VOLUME: " << m_nDispVolumeMusic;
	oss2 << "SFX VOLUME: " << m_nDispVolumeSfx;
	words.Print(oss.str().c_str(), 90, 80, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
	words.Print(oss2.str().c_str(), 90, 110, 1.0f, D3DCOLOR_XRGB( 255, 255, 255));
}