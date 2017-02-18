/*
	File:		Game.h
	Course:		SGD 1212
	Author:		Gabriel Islas
	Purpose:	CGame class controls the SGD Wrappers
*/

#include "Game.h"

// SGD Wrappers
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"

//States
#include "IGameState.h"
#include "MainMenuState.h"
#include "IntroState.h"
#include "GameplayState.h"


// SINGLETON: 
// - instantiate the static data member
/*static*/ CGame* CGame::s_Instance = nullptr;


/*static*/ CGame* CGame::GetInstance( void )
{
	// Allocate the data member if necessary
	if( s_Instance == nullptr)
		s_Instance = new CGame;

	//Return the instance
	return s_Instance;
}

/*static*/ void   CGame::DeleteInstance( void )
{
	// Deallocate the data member
	if( s_Instance != nullptr)
	{
		delete s_Instance;
		s_Instance = nullptr;
	}
}

//Cosntructor
CGame::CGame(void)
{
	// Set the data members to 'safe' values
	m_pD3D = nullptr;
	m_pDI = nullptr;
	m_pTM = nullptr;
	m_pXA = nullptr;
	
	m_nScreenWidth = 1;
	m_nScreenHeight = 1;
	m_bIsWindowed = true;;

	m_pCurrState = nullptr;
}


CGame::~CGame(void)
{
}


// Setup:
void CGame::Initialize( HWND hWnd, HINSTANCE hInstance, 
					int nWidth, int nHeight,
					bool bWindowed )
{
	// Store the SGD Wrapper singletons
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pXA = CSGD_XAudio2::GetInstance();
	
	// Store the parameters
	m_nScreenWidth = nWidth;
	m_nScreenHeight = nHeight;
	m_bIsWindowed = bWindowed;

	//Initialize the SGD Wrappers
	m_pD3D->InitDirect3D(hWnd, m_nScreenWidth, m_nScreenHeight, m_bIsWindowed );

	m_pDI->InitDirectInput(hWnd, hInstance, DI_KEYBOARD | DI_MOUSE | DI_JOYSTICKS );

	m_pTM->InitTextureManager( m_pD3D->GetDirect3DDevice(), m_pD3D->GetSprite());

	m_pXA->InitXAudio2();


	// Modified by Gabriel Islas
	// Start in the intro state
	ChangeState( CIntroState::GetInstance() );
}

// Play:
bool CGame::Main( void )
{
	//Handle input
	if( Input() == false )
		return false;

	//Update game
	Update();

	//Render game
	Render();
	return true;
}

// Cleanup:
void CGame::Shutdown( void )
{
	// Shutdown the current state
	ChangeState( nullptr );


	// Shutdown the SGD Wrappers in REVERSE-ORDER!
	if( m_pXA != nullptr)
	{
		m_pXA->ShutdownXAudio2();
		m_pXA = nullptr;
	}

	if( m_pTM != nullptr)
	{
		m_pTM->ShutdownTextureManager();
		m_pTM = nullptr;
	}

	if( m_pDI != nullptr)
	{
		m_pDI->ShutdownDirectInput();
		m_pDI = nullptr;
	}

	if( m_pD3D != nullptr)
	{
		m_pD3D->ShutdownDirect3D();
		m_pD3D = nullptr;
	}
}

bool CGame::Input( void )
{
	// Read the input hardware
	m_pDI->ReadDevices();

	// Check if the current state is valid
	if( m_pCurrState == nullptr)
		return false;

	if( m_pDI->KeyDown( DIK_LALT) == true && m_pDI->KeyPressed( DIK_RETURN) == true ) 
	{
		m_bIsWindowed = !m_bIsWindowed; 

		IDirect3D9* ptr;
		m_pD3D->GetDirect3DDevice()->GetDirect3D(&ptr);
		int size = ptr->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_R5G6B5);
		int width, height;

		for( int i = 0; i < size; i++)
		{
			D3DDISPLAYMODE mode = { };
			ptr->EnumAdapterModes( D3DADAPTER_DEFAULT, D3DFMT_R5G6B5, i, &mode );

			width = 800;
			height = 600;
		}
		m_pD3D->ChangeDisplayParam( width, height, m_bIsWindowed);
		return true;
	}

	// Let the current handle the input
	return m_pCurrState->Input();
}

void CGame::Update( void )
{
	// Refresh the audio
	m_pXA->Update();

	//Calculate the elapsed time between frames
	DWORD dwNow = GetTickCount();

	float fElapsedTime = static_cast<float>(dwNow - m_dwCurrTime) * 0.001f;

	m_dwCurrTime = dwNow;

	if (0.3f < fElapsedTime) fElapsedTime = 0.3f;

	// Update the current state
	m_pCurrState->Update( fElapsedTime );
}

void CGame::Render( void )
{
	// Clear the background
	m_pD3D->Clear(0, 0, 0);

	// Start the rendering
	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();

	// Render the current state
	m_pCurrState->Render();

	// End the rendering 
	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();

	// Present the backbuffer to the screen
	m_pD3D->Present();
}

void CGame::ChangeState( IGameState* pNewState )
{
	// Exit the current state
	if( m_pCurrState != nullptr )
		m_pCurrState->Exit();


	// Store te parameter
	m_pCurrState = pNewState;

	// Enter the new state
	if( m_pCurrState != nullptr )
		m_pCurrState->Enter();
}