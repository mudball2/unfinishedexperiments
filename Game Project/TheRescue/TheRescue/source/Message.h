/*
	File:		Message.h
	Course:		SGD 1212
	Author:		dmonroe
	Purpose:	CMessage base classs defines
				the type of messages
*/

#pragma once
#include "../SGD Wrappers/IMessage.h"

// Enumeration of message types 
enum MsgType { Msg_CreateTarget, Msg_CreateBullet, Msg_DestroyBullet, Msg_GameOver, Msg_GameWin, Msg_GameEnd};

class CMessage : public IMessage
{
public:

	CMessage(MsgType type)			{ m_nMessageID = type; }
	virtual ~CMessage(void)			{ /*do nothing*/		}

	//IMessage Interface:
	virtual MSGID	GetMessageID( void ) { return m_nMessageID; }

private:
	MSGID	m_nMessageID; 
};

