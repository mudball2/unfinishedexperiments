/*
	File:		ObjectManager.h
	Course:		SGD 1212
	Author:		dmonroe
	Purpose:	CObjectManager class stores & maintains
				game objects (player, enemies, projectiles)
*/

#pragma once


#include <list>

// Forward declaration
class IEntity;


////////////////////////////////////////////////////
// CObjectManager class
//	- stores game objects
//	- updates & renders all objects
//	- relies on reference counting for safe deletes
//	  of game objects
class CObjectManager
{
public:

	////////////////////////////////////////////////
	// SINGLETON!
	static	CObjectManager*	GetInstance( void );
	static	void DeleteInstance( void );


	////////////////////////////////////////////////
	// Core Methods
	void	AddObject		( IEntity* ptr );
	void	RemoveObject	( IEntity* ptr );
	void	RemoveAllObjects( void );

	void	UpdateAllObjects( float fElapsedTime );
	void	CheckCollisions( void );
	void	RenderAllObjects( void );


private:
	
	////////////////////////////////////////////////
	// SINGLETON!
	static	CObjectManager*	s_Instance;
	CObjectManager( void );
	~CObjectManager( void );
	CObjectManager( const CObjectManager& );
	CObjectManager& operator= ( const CObjectManager& );


	
	////////////////////////////////////////////////
	// Typedefs for 'easy' names
	typedef std::list< IEntity* >	ObjectList;
	typedef ObjectList::iterator	ObjectIter;

	
	////////////////////////////////////////////////
	// Members
	ObjectList	m_Objects;			// linked-list of IEntity*
	bool		m_bIterating;		// lock to prevent adding/removing during iteration
};

