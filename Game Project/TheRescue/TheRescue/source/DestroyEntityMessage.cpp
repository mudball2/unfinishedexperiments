/*
	File:		DestroyEntityMessage.cpp
	Course:		SGD 1212
	Author:		Gabriel Islas
	Purpose:	CDestroyEntityMessage class is used to remove an entity from the Object Manager
*/
#include "DestroyEntityMessage.h"

#include "Entity.h"

#include <cassert>

CDestroyEntityMessage::CDestroyEntityMessage(CEntity* pEntityToDestroy)
	: CMessage( Msg_DestroyBullet )
{
	//Validate the parameter
	assert( pEntityToDestroy != nullptr && "CDestroyEntityMessage - cannot store null pointer" );


	//Store the parameter
	m_pEntity = pEntityToDestroy;

	// Add a reference
	m_pEntity->AddRef();
}


CDestroyEntityMessage::~CDestroyEntityMessage(void)
{
	// Release the reference
	m_pEntity->Release();
	m_pEntity = nullptr;
}
