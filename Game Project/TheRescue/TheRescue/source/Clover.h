/*
	File: Clover.h
	Class: SGD 1212
	Author: Gabriel Islas
	Purpose: To handle all the movement and actions for the player token.
*/
#pragma once
#include "Entity.h"

class CObjectManager;

class CClover : public CEntity
{
	//sfx
	int			m_nHurtSfxID;
	int			m_nLoseSfxID;
	bool		m_bIsHit;

	//attributes
	int			m_nHealth;

	//font assets
	int			m_nFontID;

public:
	CClover(void);
	virtual ~CClover(void);

	virtual void Render( void ); 
	virtual void Update( float fElapsedTime );
	virtual bool CheckCollision( IEntity* pOther );

	int GetHealth(void) const { return m_nHealth; }
};

