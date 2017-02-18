/*
	File: GameplayState.cpp
	Class: SGD 1212
	Author: Gabriel Islas
	Purpose: To handle gameplay
*/

#include "GameplayState.h"
#include "BitmapFont.h"
#include <iostream>
#include <sstream>
using namespace std;

////////////////////////////////////////////////////
// SGD Wrappers
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_Math.h"
#include "../SGD Wrappers/CSGD_MessageSystem.h"

#include "ObjectManager.h"


// Game State Machine
#include "Game.h"
#include "MainMenuState.h"
#include "CreditsState.h"
#include "HighScoresState.h"


// Game objects
#include "Clover.h"
#include "Bullet.h"
#include "Sentinel.h"
#include "Soldier.h"

//Messages
#include "CreateBulletMessage.h"
#include "DestroyEntityMessage.h"


////////////////////////////////////////////////////
// SINGLETON
/*static*/	CGameplayState*	CGameplayState::GetInstance( void )
{
	// Store an instance in global memory
	static CGameplayState s_Instance;

	// Return the instance
	return &s_Instance;
}


////////////////////////////////////////////////////
// Constructor
CGameplayState::CGameplayState(void)
{
	// Set the data members to 'safe' values
	m_pD3D	= nullptr;
	m_pDI	= nullptr;
	m_pTM	= nullptr;
	m_pXA	= nullptr;
	m_pOF	= nullptr;
	m_pOM	= nullptr;
	m_pMS	= nullptr;


	// Invalid Asset = -1
	m_nCloverImageID			= -1;
	m_nSoldierImageID			= -1;
	m_nBackgroundImageID		= -1;
	m_nBackgroundMusicID		= -1;
	m_nSilencedShotSfxID		= -1;
	m_nBulletImageID			= -1;
	m_nPauseImageID				= -1;
	m_nPauseCursorID			= -1;
	m_nPauseFontID				= -1;
	m_nTooEasySfxID				= -1;
	m_fMusicVolume				= 1.0f;


	m_fFPSTimer	= 0.0f;
	m_unFPS		= 0;
	m_unFrames	= 0;

	m_bIsPaused = false;
	m_bYouWin	= false;
	m_bYouLose	= false;
	m_nCursorY	= 200;
}

////////////////////////////////////////////////////
// Destructor
CGameplayState::~CGameplayState(void)
{
}


////////////////////////////////////////////////////
// Enter
//	- load resources
//	- create game objects
/*virtual*/ void CGameplayState::Enter	( void )
{	
	// Store the SGD Wrapper singletons
	m_pD3D	= CSGD_Direct3D::GetInstance();
	m_pDI	= CSGD_DirectInput::GetInstance();
	m_pTM	= CSGD_TextureManager::GetInstance();
	m_pXA	= CSGD_XAudio2::GetInstance();
	m_pOF	= CSGD_Factory::GetInstance();
	m_pOM	= CObjectManager::GetInstance();
	m_pMS	= CSGD_MessageSystem::GetInstance();

	//RESET
	m_bIsPaused = false;
	m_bYouWin	= false;
	m_bYouLose	= false;

	////////////////////////////////////////////////
	// Game objects (Tokens)
	m_pPlayer	= nullptr;
	CEntity*				m_pBullet;
	m_pBullet	= nullptr;
	CEntity*				m_pSoldier;
	m_pSoldier	= nullptr;
	CEntity*				m_pSentinel;
	m_pSentinel	= nullptr;

	// Load the images
	m_nCloverImageID			= m_pTM->LoadTexture( 
								_T("resource/graphics/clover/GI_clover.png"));	

	m_nSentinelImageID		= m_pTM->LoadTexture(
								_T("resource/graphics/sentinel/GI_sentinel_aim.png"));

	m_nBackgroundImageID	= m_pTM->LoadTexture(_T("resource/graphics/levels/GI_reactorclean3.png"));

	// Load the background music
	m_nBackgroundMusicID	= m_pXA->MusicLoadSong(
								_T("resource/sounds/GI_Sneak.xwm") );


	// Load the sound effect
	m_nSilencedShotSfxID		= m_pXA->SFXLoadSound(
								_T("resource/sounds/GI_SilencedPistol.wav") );
	m_nTooEasySfxID				= m_pXA->SFXLoadSound(
								_T("resource/sounds/GI_clover-win.wav") );

	// Load the bullet image
	m_nBulletImageID			= m_pTM->LoadTexture(_T("resource/graphics/other/GI_bullet.png"));

	m_nSoldierImageID		= m_pTM->LoadTexture(
								_T("resource/graphics/soldier/GI_soldier_aim.png") );

	m_nPauseImageID			= m_pTM->LoadTexture(
								_T("resource/graphics/menu/GI_PauseBackground.png") );

	m_nPauseCursorID		= m_pTM->LoadTexture(
								_T("resource/graphics/menu/GI_MenuCursor.png" ), D3DCOLOR_XRGB(255, 255, 255) );

	m_nPauseFontID			= m_pTM->LoadTexture(
								_T("resource/graphics/fonts/GI_MenuItems.png" ) );

	// Play the background music
	m_pXA->MusicPlaySong( m_nBackgroundMusicID, true );

	// Register the game tokens with the Object Factory
	m_pOF->RegisterClassType< CEntity	>(	_T("CEntity")	);
	m_pOF->RegisterClassType< CClover	>(	_T("CClover")	);
	m_pOF->RegisterClassType< CSoldier	>(	_T("CSoldier")	);
	m_pOF->RegisterClassType< CSentinel	>(	_T("CSentinel")	);

	m_pPlayer = m_pOF->CreateObject( _T("CClover") );
	m_pSoldier = m_pOF->CreateObject( _T("CSoldier") );
	m_pSentinel = m_pOF->CreateObject( _T("CSentinel" ) );

	m_pPlayer->SetImageID( m_nCloverImageID );
	m_pPlayer->SetHeight( 64 );
	m_pPlayer->SetWidth( 64 );
	m_pSoldier->SetImageID( m_nSoldierImageID );
	m_pSoldier->SetHeight( 64 );
	m_pSoldier->SetWidth( 64 );
	m_pSentinel->SetImageID( m_nSentinelImageID);
	m_pSentinel->SetHeight( 64 );
	m_pSentinel->SetWidth( 64 );



	
	// Store the player in the object manager
	m_pOM->AddObject( m_pPlayer );

	//Store the enemies in the object manager
	m_pOM->AddObject( m_pSoldier );
	m_pOM->AddObject( m_pSentinel );



	//Release the enemies
	if( m_pSoldier != nullptr )
	{
		m_pSoldier->Release();
		m_pSoldier = nullptr;
	}
	if( m_pSentinel != nullptr )
	{
		m_pSentinel->Release();
		m_pSoldier = nullptr;
	}

		
	// Initialize the Message System
	m_pMS->InitMessageSystem( &MessageProc );

	m_pOF->RegisterClassType< CBullet	>(	_T("CBullet")	);


	m_pBullet = m_pOF->CreateObject( _T("CBullet") );

	m_pBullet->SetImageID( m_nBulletImageID );

	// Release the bullets
	if( m_pBullet != nullptr )
	{
		m_pBullet->Release();
		m_pBullet = nullptr;
	}

	// Reset the FPS
	m_fFPSTimer	= 0.0f;
	m_unFPS		= 60;
	m_unFrames	= 0;

}


////////////////////////////////////////////////////
// Exit
//	- release game objects
//	- unload resources
/*virtual*/ void CGameplayState::Exit	( void )
{
	// Shuts down the message system. 
	if ( m_pMS != nullptr )
	{
		m_pMS->ShutdownMessageSystem();
		m_pMS = nullptr;
	}

	// Shutdown the object manager
	if( m_pOM != nullptr )
	{
		m_pOM->RemoveAllObjects();
		CObjectManager::DeleteInstance();
		m_pOM = nullptr;
	}

	// Shutdown the object factory
	if( m_pOF != nullptr )
	{
		m_pOF->ShutdownObjectFactory();
		m_pOF = nullptr;
	}

	
	// Release the player
	if( m_pPlayer != nullptr )
	{
		m_pPlayer->Release();
		m_pPlayer = nullptr;
	}


	// Unload the assets
	if( m_nCloverImageID != -1 )
	{
		m_pTM->UnloadTexture( m_nCloverImageID );
		m_nCloverImageID = -1;
	}
	if( m_nSoldierImageID != -1)
	{
		m_pTM->UnloadTexture( m_nSoldierImageID );
		m_nSoldierImageID = -1;
	}
	if( m_nSentinelImageID != -1)
	{
		m_pTM->UnloadTexture( m_nSentinelImageID );
		m_nSentinelImageID = -1;
	}
	if( m_nBackgroundImageID != -1 )
	{
		m_pTM->UnloadTexture( m_nBackgroundImageID);
		m_nBackgroundImageID = -1;
	}
	if( m_nBulletImageID != -1 )
	{
		m_pTM->UnloadTexture( m_nBulletImageID );
		m_nBulletImageID = -1;
	}
	if( m_nPauseImageID != -1 )
	{
		m_pTM->UnloadTexture( m_nPauseImageID );
		m_nPauseImageID = -1;
	}
	if( m_nPauseFontID != -1 )
	{
		m_pTM->UnloadTexture( m_nPauseFontID );
		m_nPauseFontID = -1;
	}
	if( m_nPauseCursorID != -1 )
	{
		m_pTM->UnloadTexture( m_nPauseCursorID );
		m_nPauseFontID = -1;
	}

	if( m_nBackgroundMusicID != -1 )
	{
		m_pXA->MusicStopSong	( m_nBackgroundMusicID );
		m_pXA->MusicUnloadSong	( m_nBackgroundMusicID );
		m_nBackgroundMusicID = -1;
	}
	
	if( m_nSilencedShotSfxID != -1 )
	{
		m_pXA->SFXStopSound		( m_nSilencedShotSfxID );
		m_pXA->SFXUnloadSound	( m_nSilencedShotSfxID );
		m_nSilencedShotSfxID = -1;
	}
	if( m_nTooEasySfxID != -1 )
	{
		m_pXA->SFXStopSound( m_nTooEasySfxID );
		m_pXA->SFXUnloadSound( m_nTooEasySfxID );
		m_nTooEasySfxID = -1;
	}

	// Reset the data members to 'safe' values
	m_pD3D	= nullptr;
	m_pDI	= nullptr;
	m_pTM	= nullptr;
	m_pXA	= nullptr;
}


////////////////////////////////////////////////////
// Input
//	- handle the user's input
/*virtual*/ bool CGameplayState::Input	( void )
{

	if( m_pDI->KeyPressed( DIK_ESCAPE ) == true && m_bYouWin != true && m_bYouLose != true)
	{
		//CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		m_bIsPaused = !m_bIsPaused;
		return true;
	}

	if( m_bIsPaused == true )
	{
		if( m_pDI->KeyPressed( DIK_RETURN ) == true && m_nCursorY == 230)
		{
			CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
			return true;
		}

		if( m_pDI->KeyPressed( DIK_RETURN ) == true && m_nCursorY == 200 )
		{
			m_bIsPaused = !m_bIsPaused;
			return true;
		}

		if( m_pDI->KeyPressed(DIK_UP) == true)
		{
			m_nCursorY -= 30;
			m_pXA->SFXPlaySound(m_nSilencedShotSfxID, false);
		}
		else if( m_pDI->KeyPressed(DIK_DOWN) == true)
		{
			m_nCursorY += 30;
			m_pXA->SFXPlaySound(m_nSilencedShotSfxID, false);
		}
	}
	if( m_bYouWin == true )
	{

		if( m_pDI->KeyPressed( DIK_RETURN ) == true)
		{
			CGame::GetInstance()->ChangeState(CHighScoresState::GetInstance());
			return true;
		}
	}

	if( m_bYouLose == true )
	{
		if( m_pDI->KeyPressed( DIK_RETURN ) == true )
		{
			CGame::GetInstance()->ChangeState(CCreditsState::GetInstance());
			return true;
		}

	}

	return true;
}


////////////////////////////////////////////////////
// Update
//	- moves the game objects based on velocity
//	- checks collisions
/*virtual*/ void CGameplayState::Update	( float fElapsedTime )
{
	if( m_bIsPaused == false  && m_bYouWin == false && m_bYouLose == false)
	{
		m_pOM->UpdateAllObjects( fElapsedTime );
		m_pOM->CheckCollisions();

		//Process the messages and events.
		m_pMS->ProcessMessages();

	
		m_fMusicVolume = m_pXA->MusicGetMasterVolume();

		// Update the FPS
		m_fFPSTimer += fElapsedTime;
		if( m_fFPSTimer >= 1.0f )
		{
			m_unFPS		= m_unFrames;
			m_unFrames	= 0;
			m_fFPSTimer	= 0.0f;
		}
	}
	else
	{
		if( m_nCursorY > 230)
			m_nCursorY = 200;
		else if( m_nCursorY < 200)
			m_nCursorY = 230;
	}
}


////////////////////////////////////////////////////
// Render
//	- draw the game objects
/*virtual*/ void CGameplayState::Render	( void )
{	
	m_pTM->Draw(m_nBackgroundImageID, 0, 0, 1.0f, 2.5f, 0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(100, 255, 255, 255));
	CBitmapFont text;

	text.Initialize(16, 16, 20, 3, m_nPauseFontID, ' ', true );
	text.Print("LEVEL 3: RESCUE THE SOLDIER", 300, 0, 1.0f, D3DCOLOR_XRGB(255, 0, 0));
	//m_pD3D->Clear(255,255,255);
	//Instead of clearing white, show the background

	m_pOM->RenderAllObjects();

	if(m_bIsPaused == true )
	{

		m_pTM->Draw(m_nPauseImageID, 0, 0, 2.0f, 2.5f, 0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(200, 0, 0, 0));

		text.Print("PAUSED", 200, 150, 1.0f, D3DCOLOR_XRGB( 255, 255, 255 ));

		text.Print("RESUME", 250, 200, 1.0f, D3DCOLOR_XRGB( 255, 255, 255));

		text.Print("BACK TO MENU", 250, 230, 1.0f, D3DCOLOR_XRGB( 255, 255, 255));

		m_pTM->Draw(m_nPauseCursorID,200, m_nCursorY, 1.0f, 1.0f); 
	}

	if( m_bYouWin == true )
	{

		m_pTM->Draw(m_nPauseImageID, 0, 0, 2.0f, 2.5f, 0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(200, 0, 0, 0));

		text.Print("YOU WON!", 200, 150, 1.0f, D3DCOLOR_XRGB( 0, 0, 255 ));

	}

	if( m_bYouLose == true )
	{
		m_pTM->Draw(m_nPauseImageID, 0, 0, 2.0f, 2.5f, 0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(200, 0, 0, 0));

		text.Print("YOU LOSE...", 200, 150, 1.0f, D3DCOLOR_XRGB( 255, 0, 0 ));

	}
}


////////////////////////////////////////////////////
// MessageProc
//	- handles the game's messages
//	- static method does NOT have an invoking object!
//  - now the message proc is handling a few more things. :P
/*static*/ void CGameplayState::MessageProc( IMessage* pMsg )
{
	CGameplayState* self = CGameplayState::GetInstance();

	switch( pMsg->GetMessageID() )
	{
	case Msg_CreateTarget:
		{

		}
		break;

	case Msg_CreateBullet:
		{
			// Convert the message
			CCreateBulletMessage* pCreateMsg = dynamic_cast< CCreateBulletMessage* >( pMsg );


			CEntity* pFiringEntity = pCreateMsg->GetBullOwner();
			

			CEntity* pBullet = self->m_pOF->CreateObject( _T("CBullet") );

			pBullet->SetImageID( self->m_nBulletImageID );
			pBullet->SetWidth( 8 );
			pBullet->SetHeight( 8 );

			tVector2D vMove = { 1, 0 };

			if( pFiringEntity->GetDirection() == true )
			{
				pBullet->SetPosX( pFiringEntity->GetPosX()-64 );
				pBullet->SetPosY( pFiringEntity->GetPosY()+30 );
			}
			else
			{
				pBullet->SetPosX( pFiringEntity->GetPosX()+64 );
				pBullet->SetPosY( pFiringEntity->GetPosY()+30 );
			}

			tVector2D vOrientation = { 1, 0 };

			if(pFiringEntity->GetDirection() == true )
			{
				vOrientation.fX = -1;		
			}
			else
			{
				 // default orientation
			}

			vOrientation = vOrientation*300;

			// Store the velocity components
			pBullet->SetVelX( vOrientation.fX );
			pBullet->SetVelY( vOrientation.fY );

			self->m_pOM->AddObject( pBullet );

			// Release the LOCAL variable!
			pBullet->Release();
			pBullet = nullptr;


			// Play sfx
			if( self->m_pXA->SFXIsSoundPlaying( self->m_nSilencedShotSfxID ) == true )
				self->m_pXA->SFXStopSound( self->m_nSilencedShotSfxID );
			self->m_pXA->SFXPlaySound( self->m_nSilencedShotSfxID );
		}
		break;

	case Msg_DestroyBullet:
		{
			// Convert the message
			CDestroyEntityMessage* pDestroyMsg = dynamic_cast< CDestroyEntityMessage* >(pMsg);

			// Access the entity attached to the message
			CEntity* pEntity = pDestroyMsg->GetEntityDestroy();

			if(pEntity->GetType() == Obj_Sentinel )
				self->m_pPlayer->SetScore(2000);

			if(pEntity->GetType() == Obj_Soldier )
				self->m_pPlayer->SetScore(50);

			//Remove the entity from the object manager
			self->m_pOM->RemoveObject( pEntity );
			pEntity = nullptr;
		}
		break;

	case Msg_GameOver:
		{
			self->m_bYouLose = !self->m_bYouLose;
		}
		break;

	case Msg_GameWin:
		{
			
		    self->m_pXA->SFXPlaySound(self->m_nTooEasySfxID, false);
			self->m_nFinalScore = self->m_pPlayer->GetScore();
			self->m_bYouWin = !self->m_bYouWin;
		}
		break;

	case Msg_GameEnd:
		{

		}
		break;

	}
}