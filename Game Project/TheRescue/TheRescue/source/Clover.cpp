/*
	File: Clover.cpp
	Class: SGD 1212
	Author: Gabriel Islas
	Purpose: To handle all the movement and actions for the player token.
*/
#include "Clover.h"
#include "BitmapFont.h"
//Include file to have the animations for the character.
#include "CCellAnimation.h"


#include "Game.h"

#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../SGD Wrappers/SGD_Math.h"
#include "../SGD Wrappers/CSGD_MessageSystem.h"
#include "ObjectManager.h"
#include "GameplayState.h"
#include "CreateBulletMessage.h"
#include "GameOverMessage.h"
#include "MessageGameWin.h"
#include "DestroyEntityMessage.h"
#include <iostream>
#include <sstream>
using namespace std;

CClover::CClover(void) 
{
	CEntity::m_nObjectType = Obj_Clover;

	//m_bIsFacingLeft = false;
	SetDirection(false);

	m_bIsHit = false;
	m_nHealth = 450;

	// load sfx
	CSGD_XAudio2* pXA = CSGD_XAudio2::GetInstance();
	m_nHurtSfxID = pXA->SFXLoadSound( _T("resource/sounds/GI_clover-hurt.wav") );
	m_nLoseSfxID = pXA->SFXLoadSound( _T("resource/sounds/GI_clover-lose.wav") );

	// load fonts
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	m_nFontID = pTM->LoadTexture(_T("resource/graphics/fonts/GI_MenuTitle.png") );

}


CClover::~CClover(void)
{
	// unload sfx
	CSGD_XAudio2* pXA = CSGD_XAudio2::GetInstance();

	if( m_nHurtSfxID != -1 )
	{
		pXA->SFXStopSound( m_nHurtSfxID );
		pXA->SFXUnloadSound( m_nHurtSfxID );
		m_nHurtSfxID = -1;
	}

	if( m_nLoseSfxID != -1 )
	{
		pXA->SFXStopSound( m_nLoseSfxID );
		pXA->SFXUnloadSound( m_nLoseSfxID );
		m_nLoseSfxID = -1;
	}

	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	if( m_nFontID != -1)
	{
		pTM->UnloadTexture(m_nFontID);
		m_nFontID = -1;
	}
}

void CClover::Render( void )
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	RECT rCloverRect = { 0, 0, 64, 64 };
	RECT rHealth = { 120, 40, m_nHealth, 60 };
	ostringstream oss;
	
	CBitmapFont score;
	score.Initialize(32, 33, 10, 6, m_nFontID, ' ', true);

	oss << "SCORE:" << CEntity::GetScore();

	score.Print(oss.str().c_str(), 0, 0, 0.8f, D3DCOLOR_XRGB(255, 255, 255));

	oss.clear();

	score.Print("HEALTH:", 0, 40, 0.5f, D3DCOLOR_XRGB(0, 255, 0));
	pD3D->DrawRect(rHealth, 0, 255, 0);


	SetPosY(300);
	if( GetDirection() == false )
	{
		CSGD_TextureManager::GetInstance()->Draw(
			GetImageID(), GetPosX(), GetPosY(),
			1.0f, 1.0f, &rCloverRect);
	}
	else
	{
		CSGD_TextureManager::GetInstance()->Draw(
			GetImageID(), GetPosX(), GetPosY(),
			-1.0f, 1.0f, &rCloverRect);
	}
}



void CClover::Update( float fElapsedTime )
{
	// Access the wrappers
	CSGD_DirectInput*	pDI = CSGD_DirectInput::GetInstance();
	CSGD_MessageSystem*	pMS = CSGD_MessageSystem::GetInstance();
	CObjectManager*		pOM = CObjectManager::GetInstance();
	

	// Simple movement
	if( pDI->KeyDown( DIK_LEFT ) == true )
	{
		SetDirection(true);
		SetVelX(-200);
		//SetPosX(GetPosX()-GetVelX()*fElapsedTime);
	}
	else if( pDI->KeyDown( DIK_RIGHT ) == true )
	{
		SetDirection(false);
		SetVelX(200);
		//SetPosX(GetPosX()+GetVelX()*fElapsedTime);
	}
	else
	{
		SetVelX(0);
	}

	if( pDI->KeyPressed( DIK_SPACE ) == true )
	{
		// Send a create bullet message
		CCreateBulletMessage* pMsg = new CCreateBulletMessage(this);
		pMS->SendMsg( pMsg );
		pMsg = nullptr;
	}

	CEntity::Update(fElapsedTime);


	int width =  800;
	int height = 600;
	
	if( GetPosX()-64 < 0)
	{
		//SetPosX( 0.0f);
		SetPosX(GetPosX()+10);
	}
	else if( GetPosX() + GetWidth()> width )
	{
		//SetPosX( (float)width - GetWidth()+64 );
		SetPosX(GetPosX()-10);
	}

	CSGD_XAudio2* pXA = CSGD_XAudio2::GetInstance();
	if( m_bIsHit == true && !pXA->SFXIsSoundPlaying(m_nHurtSfxID) )
	{
		pXA->SFXPlaySound(m_nHurtSfxID, false);
		m_bIsHit = !m_bIsHit;
	}

	if(m_nHealth <= 120 )
	{
		m_bIsHit = false;
		pXA->SFXStopSound(m_nHurtSfxID);
		pXA->SFXPlaySound(m_nLoseSfxID, false);
		m_nHealth = 121;

		//// Self-destruct
		//CDestroyEntityMessage* pMsg = new CDestroyEntityMessage( this );
		//CSGD_MessageSystem::GetInstance()->SendMsg( pMsg );
		//pMsg = nullptr;

		CGameOverMessage* pMsg = new CGameOverMessage();
		CSGD_MessageSystem::GetInstance()->SendMsg( pMsg );
		pMsg = nullptr;
	}

	if( CEntity::GetScore() == 2000 )
	{
		CMessageGameWin* pMsg = new CMessageGameWin();
		CSGD_MessageSystem::GetInstance()->SendMsg( pMsg);
		pMsg = nullptr;
	}
}

/*virtual */bool CClover::CheckCollision( IEntity* pOther )
{
	// Are we overlapping
	if( CEntity::CheckCollision( pOther ) == true )
	{

		if( pOther->GetType() == Obj_Soldier || pOther->GetType() == Obj_Sentinel 
			|| pOther->GetType() == Obj_Bullet )
		{
			m_bIsHit = !m_bIsHit;

			if(pOther->GetType() == Obj_Bullet)
				m_nHealth -= 10;

			return true;
		}

	}
	
	return false;
}