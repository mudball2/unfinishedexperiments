/*
	File:		GameEndMessage.h
	Course:		SGD 1212
	Author:		Gabriel Islas
	Purpose:	CGameOverMessage class is used to trigger the final events after gameplay.
*/
#pragma once
#include "Message.h"
class CGameEndMessage : public CMessage
{
public:
	CGameEndMessage(void);
	virtual ~CGameEndMessage(void);
};

