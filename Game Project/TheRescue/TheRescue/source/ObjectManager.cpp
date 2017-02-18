/*
	File:		ObjectManager.cpp
	Course:		SGD 1212
	Author:		dmonroe
	Purpose:	CObjectManager class stores & maintains
				game objects (player, enemies, projectiles)
*/

#include "ObjectManager.h"

#include "IEntity.h"

#include <cassert>



////////////////////////////////////////////////////
// SINGLETON:
//	- instantiate the static data member
/*static*/	CObjectManager*	CObjectManager::s_Instance = nullptr;


/*static*/	CObjectManager*	CObjectManager::GetInstance( void )
{
	if( s_Instance == nullptr )
		s_Instance = new CObjectManager;

	return s_Instance;
}

/*static*/	void CObjectManager::DeleteInstance( void )
{
	delete s_Instance;
	s_Instance = nullptr;
}



////////////////////////////////////////////////////
// CONSTRUCTOR
CObjectManager::CObjectManager(void)
{
	m_bIterating = false;
}

////////////////////////////////////////////////////
// DESTRUCTOR
CObjectManager::~CObjectManager(void)
{
}


////////////////////////////////////////////////////
// AddObject
//	- adds the object to the linked list,
//	  holding a reference to the object
void CObjectManager::AddObject( IEntity* ptr )
{
	// Verify the parameter
	assert( ptr != nullptr && "CObjectManager::AddObject - parameter cannot be null!" );

	// Verify the object manager is NOT in the 
	// middle of iterating the list
	assert( m_bIterating == false && "CObjectManager::AddObject - cannot add while iterating!" );



	// 'Hold' a reference
	ptr->AddRef();


	// Add the object to our list
	m_Objects.push_back( ptr );
}


////////////////////////////////////////////////////
// RemoveObject
//	- removes the object from the linked list,
//	  releasing the reference
void CObjectManager::RemoveObject( IEntity* ptr )
{	 
	// Verify the parameter
	assert( ptr != nullptr && "CObjectManager::RemoveObject - parameter cannot be null!" );

	// Verify the object manager is NOT in the 
	// middle of iterating the list
	assert( m_bIterating == false && "CObjectManager::RemoveObject - cannot remove while iterating!" );


	// Find the object in the list
	for( ObjectIter iter = m_Objects.begin(); iter != m_Objects.end(); ++iter )
	{
		if( (*iter) == ptr )
		{
			// Release the reference
			(*iter)->Release();


			// Remove the object
			m_Objects.erase( iter );
			break;
		}
	}

}	 

////////////////////////////////////////////////////
// RemoveAllObjects
//	- releases the references to all objects in
//	  the linked list & then clears the list
void CObjectManager::RemoveAllObjects( void )
{	 
	// Verify the object manager is NOT in the 
	// middle of iterating the list
	assert( m_bIterating == false && "CObjectManager::RemoveAllObjects - cannot remove while iterating!" );


	// Release every object
	for( ObjectIter iter = m_Objects.begin(); iter != m_Objects.end(); ++iter )
	{
		(*iter)->Release();
	}


	// Collapse the list
	m_Objects.clear();
}	 
	 
////////////////////////////////////////////////////
// UpdateAllObjects
//	- updates all objects in the linked list
void CObjectManager::UpdateAllObjects( float fElapsedTime )
{
	// Verify the object manager is NOT in the 
	// middle of iterating the list
	assert( m_bIterating == false && "CObjectManager::UpdateAllObjects - cannot update while iterating!" );

	// 'Lock' the object manager
	m_bIterating = true;

	for( ObjectIter iter = m_Objects.begin(); iter != m_Objects.end(); ++iter )
	{
		(*iter)->Update(fElapsedTime);
	}


	// 'Unlock' the object manager
	m_bIterating = false;
}

////////////////////////////////////////////////////
// CheckCollisions
//	- checks every object in the list against every
//	  other object OF A DIFFERENT TYPE
void CObjectManager::CheckCollisions( void )
{
	// Verify the object manager is NOT in the 
	// middle of iterating the list
	assert( m_bIterating == false && "CObjectManager::UpdateAllObjects - cannot update while iterating!" );

	// 'Lock' the object manager
	m_bIterating = true;


	// Iterate O(n^2)
	for( ObjectIter iterI = m_Objects.begin(); iterI != m_Objects.end(); ++iterI )
	{
		for( ObjectIter iterJ = m_Objects.begin(); iterJ != m_Objects.end(); ++iterJ )
		{
			// Check for different TYPES
			if( (*iterI)->GetType() != (*iterJ)->GetType() )
			{
				// CheckCollision returns true to stop the iteration
				if( (*iterI)->CheckCollision( *iterJ ) == true )
					break;
			}
		}
	}


	// 'Unlock' the object manager
	m_bIterating = false;
}

////////////////////////////////////////////////////
// RenderAllObjects
//	- renders all objects in the linked list
void CObjectManager::RenderAllObjects( void )
{
	// Verify the object manager is NOT in the 
	// middle of iterating the list
	assert( m_bIterating == false && "CObjectManager::RenderAllObjects - cannot render while iterating!" );

	// 'Lock' the object manager
	m_bIterating = true;

	for( ObjectIter iter = m_Objects.begin(); iter != m_Objects.end(); ++iter )
	{
		(*iter)->Render();
	}


	// 'Unlock' the object manager
	m_bIterating = false;
}
