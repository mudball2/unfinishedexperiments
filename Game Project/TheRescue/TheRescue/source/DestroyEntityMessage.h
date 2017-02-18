/*
	File:		DestroyEntityMessage.h
	Course:		SGD 1212
	Author:		Gabriel Islas
	Purpose:	CDestroyEntityMessage class is used to remove an entity from the Object Manager
*/

#pragma once
#include "Message.h"

// Forward class declaration
class CEntity;

class CDestroyEntityMessage : public CMessage
{
public:
	CDestroyEntityMessage(CEntity* pEntityToDestroy);
	virtual ~CDestroyEntityMessage(void);

	CEntity*	GetEntityDestroy( void ) const	{ return m_pEntity; }
private:
	CEntity*		m_pEntity;
};

