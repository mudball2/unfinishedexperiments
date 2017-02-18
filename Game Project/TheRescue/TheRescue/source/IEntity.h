/*
	File:		IEntity.h
	Course:		SGD 1212
	Author:		dmonroe
	Purpose:	IEntity interface class is the parent for
				the different game objects (player, enemy, projectiles)
*/

#pragma once

#include <Windows.h>


////////////////////////////////////////////////////
// Interface class
//	- ONLY contains pure virtual methods
//	- Abstract Base Class
class IEntity
{
public:

	////////////////////////////////////////////////
	// Pure virtual destructor
	virtual ~IEntity( void )		= 0
	{	/* do nothing */	}
	

	////////////////////////////////////////////////
	// IEntity Interface:
	virtual void Update ( float fElapsedTime )	= 0;
	virtual void Render ( void )	= 0;

	virtual void AddRef ( void )	= 0;
	virtual void Release( void )	= 0;
	

	//Modified by Gabriel Islas
	virtual bool CheckCollision( IEntity* pOther )	= 0;
	virtual int GetType ( void ) const = 0;
	virtual RECT GetRect( void ) const = 0;
};

