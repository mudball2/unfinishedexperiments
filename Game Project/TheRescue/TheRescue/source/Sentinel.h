/*
	File: CSentinel.h
	Class: SGD 1212
	Author: Gabriel Islas
	Purpose: To handle all of the functionality and behavior for the Sentinel soldier
*/
#pragma once
#include "Entity.h"
class CSentinel : public CEntity
{
	bool m_bAlert;
	int m_nHealth;
	int m_nSFXID;
public:
	CSentinel(void);
	virtual ~CSentinel(void);

	virtual void Render( void ); 
	virtual void Update( float fElapsedTime );
	virtual bool CheckCollision( IEntity* pOther );
};

