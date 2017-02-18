/*
	File: CreateBulletMessage.cpp
	Class: SGD 1212
	Author: Gabriel Islas
	Purpose: To Create the bullet message as implied by the class name.
*/
#include "CreateBulletMessage.h"

#include "Entity.h"


CCreateBulletMessage::CCreateBulletMessage(CEntity* pOwner)
	: CMessage( Msg_CreateBullet )
{
	// Store the parameters
	m_pBOwner	= pOwner;
	m_pBOwner->AddRef();	
}


CCreateBulletMessage::~CCreateBulletMessage(void)
{
	// Release the reference
	m_pBOwner->Release();
	m_pBOwner = nullptr;
}
