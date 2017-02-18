/*
	File:		GameOverMessage.h
	Course:		SGD 1212
	Author:		Gabriel Islas
	Purpose:	CGameOverMessage class is used to end the game
*/
#pragma once
#include "Message.h"

class CEntity;

class CGameOverMessage : public CMessage
{
public:
	CGameOverMessage(void);
	virtual ~CGameOverMessage(void);
};

