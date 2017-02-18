/*
	File:		IGameState.h
	Course:		SGD 1212
	Author:		Gabriel Islas
	Purpose:	IGameState INTERFACE Class is the parent for 
				the different game states
*/

#pragma once

//Interface class
// - ONLY contains methods
// - Abstract Base Class
class IGameState
{
public:
	// Pure virtual destructor
	virtual ~IGameState(void)	= 0
	{	/* do nothing */	}


	// Pure virtual methods MUST be overriden by children classes
	virtual void Enter( void )	= 0;
	virtual void Exit( void )	= 0;

	virtual bool Input( void )	= 0;
	virtual void Update( float fElapsedTime ) = 0;
	virtual void Render( void ) = 0;
};

