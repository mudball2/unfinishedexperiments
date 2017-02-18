/*
	File:		Entity.cpp
	Course:		SGD 1212
	Author:		dmonroe
	Purpose:	CEntity concrete class is the parent for
				the different game objects (player, enemy, projectiles),
				storing the shared data members
*/

#include "Entity.h"

#include "../SGD Wrappers/CSGD_TextureManager.h"

#include <cassert>


////////////////////////////////////////////////////
// CONSTRUCTOR
CEntity::CEntity(void)
{
	m_nPosX		= 0;	
	m_nPosY		= 0;
		
	m_nVelX		= 0;	
	m_nVelY		= 0;
	
	m_nImageID	= -1;

	m_unRefCount = 1;

	//Modified by Gabriel Islas
	m_nWidth	= 1;
	m_nHeight	= 1;
	m_nScore	= 0;

	//Modified by Gabriel Islas
	m_nObjectType = Obj_Base;
}


////////////////////////////////////////////////////
// DESTRUCTOR
CEntity::~CEntity(void)
{
	/* do nothing ... yet */
}


////////////////////////////////////////////////////
// Update
//	- adjusts the position by the velocity each frame
/*virtual*/ void CEntity::Update( float fElapsedTime )
{
	m_nPosX += m_nVelX*fElapsedTime;
	m_nPosY += m_nVelY*fElapsedTime;
}

////////////////////////////////////////////////////
// Render
//	- renders the game object at its position (top-left corner)
/*virtual*/ void CEntity::Render( void )
{
	assert( m_nImageID != -1 && "CEntity::Render - does not have an image!" );

	CSGD_TextureManager::GetInstance()->Draw(
		m_nImageID, m_nPosX, m_nPosY );
}


	
////////////////////////////////////////////////////
// AddRef
//	- increases the reference count to indicate another
//	  class is holding a pointer to this object
/*virtual*/ void CEntity::AddRef ( void )
{
	// Increase the count
	++m_unRefCount;
}


////////////////////////////////////////////////////
// Release
//	- decreases the reference count to indicate a class
//	  is no longer holding a pointer to this object
//	- self destructs when the reference count is 0
//	  to clean up memory when this object is no longer
//	  being used
/*virtual*/ void CEntity::Release( void )
{
	// Decrease the count
	--m_unRefCount;

	// Self-destruct when count reaches 0!
	if( m_unRefCount == 0 )
		delete this;
}


//Modified by Gabriel Islas
/*virtual*/ int CEntity::GetType ( void ) const
{
	return m_nObjectType;
}

/*virtual*/ RECT CEntity::GetRect( void ) const
{
	RECT rct = {m_nPosX, m_nPosY, m_nPosX+m_nWidth, m_nPosY+m_nHeight};
	return rct;
}

////////////////////////////////////////////////////
// CheckCollision
//	- return true if the objects are overlapping
/*virtual*/ bool CEntity::CheckCollision( IEntity* pOther )
{
	// Must store the overlapping region
	RECT rOverlap	= { };
	RECT rSelf		= this->GetRect();
	RECT rOther		= pOther->GetRect();

	// Return the result of IntersectRect
	return ( IntersectRect( &rOverlap, &rSelf, &rOther ) != FALSE );
}
