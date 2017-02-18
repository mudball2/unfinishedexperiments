/*
	File: CBullet.h
	Class: SGD 1212
	Author: Gabriel Islas
	Purpose: To render and create behavior of the bullet through Message and Event system.
*/

#pragma once
#include "Entity.h"
class CBullet : public CEntity
{
	float		m_fCurrRotation;
	CEntity*	m_pBulletOwner;
public:
	CBullet(void);
	virtual ~CBullet(void);

	virtual void Render( void );
	virtual void Update( float fElapsedTime );
	virtual bool CheckCollision( IEntity* pOther );

	CEntity* GetOwner( void )	{ return m_pBulletOwner; }
	void SetOwner( CEntity* o); 
};

