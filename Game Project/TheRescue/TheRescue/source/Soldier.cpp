/*
	File: CSoldier.cpp
	Class: SGD 1212
	Author: Gabriel Islas
	Purpose: To handle the basic soldier's functionality and behavior.
*/
#include "Soldier.h"


#include "Game.h"

#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/SGD_Math.h"
#include "../SGD Wrappers/CSGD_MessageSystem.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "ObjectManager.h"
#include "GameplayState.h"
#include "CreateBulletMessage.h"
#include "DestroyEntityMessage.h"

CSoldier::CSoldier(void)
{
	CEntity::m_nObjectType = Obj_Clover;

	m_nHealth = 5;

	CSGD_XAudio2* pXA = CSGD_XAudio2::GetInstance();
	m_nSFXID = pXA->SFXLoadSound(_T("resource/sounds/GI_soldier-aggro.wav"));

	SetDirection(false);
}


CSoldier::~CSoldier(void)
{
}


void CSoldier::Render( void )
{
	RECT rSoldierRect = { 0, 0, 64, 64 };

	SetPosY(300);
	if( GetDirection() == false )
	{
		CSGD_TextureManager::GetInstance()->Draw(
			GetImageID(), GetPosX(), GetPosY(),
			1.3f, 1.3f, &rSoldierRect);
	}
	else
	{
		CSGD_TextureManager::GetInstance()->Draw(
			GetImageID(), GetPosX(), GetPosY(),
			-1.3f, 1.3f, &rSoldierRect);
	}
}
void CSoldier::Update( float fElapsedTime )
{
	// Access the wrappers
	CSGD_DirectInput*	pDI = CSGD_DirectInput::GetInstance();
	CSGD_MessageSystem*	pMS = CSGD_MessageSystem::GetInstance();
	CObjectManager*		pOM = CObjectManager::GetInstance();
	CSGD_XAudio2*		pXA = CSGD_XAudio2::GetInstance();
	

	// Simple movement
	if(GetDirection() == true )
	{
		//m_bIsFacingLeft = true;
		SetVelX(-200);
		//SetPosX(GetPosX()+GetVelX()*fElapsedTime);
	}
	else if( GetDirection() == false )
	{
		if(!pXA->SFXIsSoundPlaying(m_nSFXID))
			pXA->SFXPlaySound(m_nSFXID, false);
		//m_bIsFacingLeft = false;
		SetVelX(200);
		//SetPosX(GetPosX()+GetVelX()*fElapsedTime);
	}

	if( GetDirection() == true && this->GetPosX()%10 == 0 )
	{
		// Send a create bullet message
		CCreateBulletMessage* pMsg = new CCreateBulletMessage(this);
		pMS->SendMsg( pMsg );
		pMsg = nullptr;
	}

	
	if( m_nHealth <= 0)
	{
		CEntity::SetScore(50);
		// Self-destruct
		CDestroyEntityMessage* pMsg = new CDestroyEntityMessage( this );
		CSGD_MessageSystem::GetInstance()->SendMsg( pMsg );
		pMsg = nullptr;
	}

	CEntity::Update(fElapsedTime);


	int width =  800;
	int height = 600;
	
	if( GetPosX()-64 < 0)
	{
		//SetPosX( 0.0f);
		//SetPosX(GetPosX()+5);
		SetDirection(false);
	}
	else if( GetPosX() + GetWidth()> width )
	{
		//SetPosX( (float)width - GetWidth()+64 );
		//SetPosX(GetPosX()-5);
		SetDirection(true);
	}
}

/*virtual */bool CSoldier::CheckCollision( IEntity* pOther )
{
	// Are we overlapping
	if( CEntity::CheckCollision( pOther ) == true )
	{

		if( pOther->GetType() == Obj_Bullet )
		{

			if(pOther->GetType() == Obj_Bullet)
				m_nHealth--;

			return true;
		}

	}
	
	return false;
}