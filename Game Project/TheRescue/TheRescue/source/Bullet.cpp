/*
	File: CBullet.cpp
	Class: SGD 1212
	Author: Gabriel Islas
	Purpose: To render and create behavior of the bullet through Message and Event system.
*/


#include "Bullet.h"

#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_MessageSystem.h"
#include "DestroyEntityMessage.h"

#include "Game.h"


CBullet::CBullet(void)
{
	CEntity::m_nObjectType = Obj_Bullet;

	m_fCurrRotation = 0.0f;

	m_pBulletOwner = nullptr;
}


CBullet::~CBullet(void)
{
	// Release the target
	SetOwner( nullptr );
}


/*virtual*/ void CBullet::Render( void )
{
	RECT rBulletRegion = { 0, 0, 8, 8 };

	CSGD_TextureManager::GetInstance()->Draw(
		GetImageID(), GetPosX(), GetPosY(), 1.0f, 1.0f,
		&rBulletRegion, 32, 32);
}

void CBullet::Update( float fElapsedTime )
{
	// Move the object
	CEntity::Update( fElapsedTime );

	// Check if the object is off screen
	RECT rOverlap = { };
	RECT rSelf	  = GetRect();
	RECT rScreen  = { 0, 0,
		CGame::GetInstance()->GetWidth(),
		CGame::GetInstance()->GetHeight()
	};

	if( IntersectRect( &rOverlap, &rSelf, &rScreen ) == FALSE )
	{
		// Self-destruct
		CDestroyEntityMessage* pMsg = new CDestroyEntityMessage( this );
		CSGD_MessageSystem::GetInstance()->SendMsg( pMsg );
		pMsg = nullptr;
	}
}

void CBullet::SetOwner( CEntity* o)
{
	// Release the existing target
	if( m_pBulletOwner != nullptr )
		m_pBulletOwner->Release();

	// Store the parameter
	m_pBulletOwner = o;


	// Add a reference to the new target
	if( m_pBulletOwner != nullptr )
		m_pBulletOwner->AddRef();
}

////////////////////////////////////////////////////
// CheckCollision
//	- handles collision detection & response
//	- returns true to stop checking for collisions
/*virtual*/ bool CBullet::CheckCollision( IEntity* pOther )
{
	// Are we overlapping
	if( CEntity::CheckCollision( pOther ) == true )
	{
		// Did we hit a turret or ship?
		if( pOther->GetType() == Obj_Clover 
			|| pOther->GetType() == Obj_Soldier || pOther->GetType() == Obj_Sentinel )
		{
			// Self-destruct
			CDestroyEntityMessage* pMsg = new CDestroyEntityMessage( this );
			CSGD_MessageSystem::GetInstance()->SendMsg( pMsg );
			pMsg = nullptr;

			return true;
		}

	}
	
	return false;
}