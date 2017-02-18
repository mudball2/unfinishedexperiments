/*
	File: Frame.cpp
	Author: Gabriel Islas
	Class: SGP
	Purpose: Stores all of the information about a current frame. 
*/
#include "Frame.h"
//#include "stdafx.h"

Frame::Frame(int AnchorX, int AnchorY, int PosX, int PosY, int width, int height, int cX, 
		  int cY, int cWidth, int cHeight, double nTime )
{
	// Setting the Anchor POINT's variables to the info that we transfered from the XML file
	Anchor.x = AnchorX;
	Anchor.y = AnchorY;

	// Setting the image RECT using the position and the width and height parameters
	ImageRect.left	= PosX;
	ImageRect.top	= PosY;
	ImageRect.right	= ImageRect.left+width;
	ImageRect.bottom= ImageRect.top+height;

	// Setting up the collision RECT
	CollisionRect.left	= cX;
	CollisionRect.top	= cY;
	CollisionRect.right	= cWidth;
	CollisionRect.bottom= cHeight;

	// Setting the duration to the time
	m_fDuration = (float)nTime;
}

Frame::~Frame()
{

}