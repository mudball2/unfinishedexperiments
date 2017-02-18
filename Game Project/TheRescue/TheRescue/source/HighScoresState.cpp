/*
	File: CHighScoresState.cpp
	Author: Gabriel Islas
	Class: SGD 1212
	Purpose: To display a set of high scores to the player.
*/
#include "HighScoresState.h"
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
#include "MainMenuState.h"
#include "HowToPlayState.h"
#include "OptionsState.h"

CHighScoresState* CHighScoresState::GetInstance( void )
{
	static CHighScoresState s_Instance;

	return &s_Instance;
}

CHighScoresState::CHighScoresState(void)
{
	// Set the data members to 'safe' values
	m_pD3D = nullptr;
	m_pDI = nullptr;
	m_pTM = nullptr;
	m_pXA = nullptr;

	m_nFontID = -1;
}


CHighScoresState::~CHighScoresState(void)
{
}

void CHighScoresState::Enter( void )
{
	// Store the SGD Wrapper singletons
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pXA = CSGD_XAudio2::GetInstance();

	ifstream fin;
	fin.open("resource/scores.sav");
	if(!fin.is_open())
	{
		ofstream fout;
		fout.open("resource/scores.sav");
			fout << "GABRIEL " << 1000 << endl;
			fout << "ANDREW " << 2000 << endl;
			fout << "DOUG " << 3000 << endl;
			fout << "ALEX " << 4000 << endl;
			fout << "TYLER " << 5000 << endl;
			fout << "JACOB " << 6000 << endl;
			fout << "JESSICA " << 7000 << endl;
			fout << "RYAN " << 8000 << endl;
			fout << "SAM " << 9000 << endl;
			fout << "CLOVER " << 10000; 
		fout.close();

		ifstream fin;
		fin.open("resource/scores.sav");

		int count = 0;
		while( !fin.eof() )
		{
			string name;
			int score;
			fin >> name;
			m_szNames[count] = name;
			fin >> score;
			m_nScores[count] = score;

			count++;
		}
		fin.close();
	}
	else
	{
		int count = 0;
		while( !fin.eof() )
		{
			string name;
			int score;
			fin >> name;
			m_szNames[count] = name;
			fin >> score;
			m_nScores[count] = score;

			count++;
		}
	}
	fin.close();


	m_nFontID = m_pTM->LoadTexture(_T("resource/graphics/fonts/GI_MenuTitle.png"));

	m_pPlayer = new CEntity();
}
void CHighScoresState::Exit( void )
{
	// Release the player
	if( m_pPlayer != nullptr )
	{
		m_pPlayer->Release();
		m_pPlayer = nullptr;
	}

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

bool CHighScoresState::Input( void )
{
	if( m_pDI->KeyPressed( DIK_ESCAPE ) == true )
	{
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		return true;
	}
	if( m_pDI->KeyPressed(DIK_RETURN ) == true )
	{
		ofstream fout;
		fout.open("resource/scores.sav");
		m_szNames[0] = m_szUserInput;
		m_nScores[0] = CGameplayState::GetInstance()->GetFinalScore();
		for( int i = 0; i < 10; i++)
		{
			if( i == 9 )
				fout << m_szNames[i] << " " << m_nScores[i];
			else
				fout << m_szNames[i] << " " << m_nScores[i] << endl;
		}
		fout.close();

		CGame::GetInstance()->ChangeState(CCreditsState::GetInstance());
		return true;
	}
	return true;
}
void CHighScoresState::Update( float fElapsedTime )
{ 
	//ossUser.write(m_szUserInput.c_str(), sizeof(m_szUserInput));

	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
	if(pDI->CheckBufferedKeysEx() != 0)
		m_szUserInput += pDI->CheckBufferedKeysEx();
	ossUser.write(m_szUserInput.c_str(), sizeof(m_szUserInput));
}
void CHighScoresState::Render( void )
{
	ostringstream oss;
	ostringstream oss2;
	CBitmapFont text;
	text.Initialize(32, 33, 10, 6, m_nFontID, ' ', true);
	text.Print("HIGH SCORES", 100, 20, 1.0f, D3DCOLOR_XRGB( 0, 0, 255 ));

	text.Print("NAME: " , 0, 70, 1.0f, D3DCOLOR_XRGB(0, 255, 0));

	text.Print(m_szUserInput.c_str(), 150, 70, 1.0f, D3DCOLOR_XRGB(255,255,255));

	for(int i = 9; i >= 0; i--)
	{


		if( m_nScores[i] < m_nScores[i+1])
		{
			oss << m_szNames[i] << "\n";
			oss2 << m_nScores[i] << "\n";
		}
		else if (m_nScores[i] > m_nScores[i+1] && i < 9 && i >= 0 )
		{
			swap(m_nScores[i+1], m_nScores[i]);
			swap(m_szNames[i+1], m_szNames[i]);
			oss << m_szNames[i] << "\n";
			oss2 << m_nScores[i] << "\n";

		}
		else
		{
			oss << m_szNames[i] << "\n";
			oss2 << m_nScores[i] << "\n";

		}
	}

	
		text.Print( oss.str().c_str(), 120, 200, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
		text.Print( oss2.str().c_str(), 400, 200, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
}