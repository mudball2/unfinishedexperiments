/*
	File:		Entity.h
	Course:		SGD 1212
	Author:		dmonroe
	Purpose:	CEntity concrete class is the parent for
				the different game objects (player, enemy, projectiles),
				storing the shared data members
*/

#pragma once

#include "IEntity.h"

enum Obj_Type { Obj_Base, Obj_Clover, Obj_Soldier, Obj_Sentinel, Obj_Bullet };


////////////////////////////////////////////////////
// CEntity class
//	- moving object w/ position & velocity
//	- stores image id
//	- uses reference counting for safe deletes
class CEntity :	public IEntity
{
public:
	CEntity( void );
	virtual ~CEntity( void );

	
	////////////////////////////////////////////////
	// IEntity Interface:
	virtual void Update( float fElapsedTime );
	virtual void Render( void );
	
	virtual void AddRef ( void );
	virtual void Release( void );

	//Modified by Gabriel Islas
	virtual bool CheckCollision( IEntity* pOther );
	virtual int GetType ( void ) const;
	virtual RECT GetRect( void ) const;

	
	////////////////////////////////////////////////
	// ACCESSORS
	int		GetPosX		( void ) const	{	return m_nPosX;		}
	int		GetPosY		( void ) const	{	return m_nPosY;		}
	int		GetVelX		( void ) const	{	return m_nVelX;		}
	int		GetVelY		( void ) const	{	return m_nVelY;		}
	int		GetImageID	( void ) const	{	return m_nImageID;	}
	int     GetWidth	( void ) const  {	return m_nWidth;	}
	int		GetHeight	( void ) const  {	return m_nHeight;	}
	bool	GetDirection( void ) const	{	return m_bIsFacingLeft; }
	int		GetScore	( void ) const	{   return m_nScore;	}


	// MUTATORS
	void	SetPosX		( int x	 )		{	m_nPosX		= x;	}
	void	SetPosY		( int y  )		{	m_nPosY		= y;	}
	void	SetVelX		( int vx )		{	m_nVelX		= vx;	}
	void	SetVelY		( int vy )		{	m_nVelY		= vy;	}
	void	SetImageID	( int id )		{	m_nImageID	= id;	}
	void	SetWidth	( int w  )		{	m_nWidth	= w;	}
	void	SetHeight	( int h	 )		{	m_nHeight	= h;	}
	void	SetDirection( bool d )		{	m_bIsFacingLeft = d;}
	void	SetScore	( int s )		{	m_nScore = s;		}


private:
	
	////////////////////////////////////////////////
	// position
	float					m_nPosX;		// top-left corner == (0, 0)
	float					m_nPosY;

	// velocity
	float				m_nVelX;		// pixels per frame
	float				m_nVelY;

	// image
	int					m_nImageID;
	
	// reference count
	unsigned int		m_unRefCount;

	//Modified by Gabriel Islas
	int					m_nWidth;
	int					m_nHeight;
	bool				m_bIsFacingLeft;
	int					m_nScore;


//Modified by Gabriel Islas
protected:

	int m_nObjectType;
};

