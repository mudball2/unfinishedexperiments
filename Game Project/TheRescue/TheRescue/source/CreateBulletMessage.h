/*
	File: CreateBulletMessage.h
	Class: SGD 1212
	Author: Gabriel Islas
	Purpose: To Create the bullet message as implied by the class name.
*/
#pragma once
#include "Message.h"

class CEntity;

class CCreateBulletMessage : public CMessage
{
	CEntity*	m_pBOwner;
public:
	CCreateBulletMessage(CEntity* pOwner);
	virtual ~CCreateBulletMessage(void);

	//Accessor
	CEntity* GetBullOwner(void) { return m_pBOwner; }
};

