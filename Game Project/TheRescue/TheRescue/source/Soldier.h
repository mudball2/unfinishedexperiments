/*
	File: CSoldier.h
	Class: SGD 1212
	Author: Gabriel Islas
	Purpose: To handle the basic soldier's functionality and behavior.
*/
#pragma once
#include "Entity.h"
class CSoldier : public CEntity
{
	int		m_nHealth;
	int		m_nSFXID;
public:
	CSoldier(void);
	virtual ~CSoldier(void);

	virtual void Render( void ); 
	virtual void Update( float fElapsedTime );
	virtual bool CheckCollision( IEntity* pOther );
};

