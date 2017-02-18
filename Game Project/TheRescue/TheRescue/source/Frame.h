/*
	File: Frame.h
	Author: Gabriel Islas
	Class: SGP
	Purpose: Stores all of the information about a current frame. 
*/
#ifndef FRAME_H
#define FRAME_H

#include <Windows.h>

class Frame
{
	//Private data members
	RECT ImageRect;
	RECT CollisionRect;
	POINT Anchor;
	float m_fDuration;
public:
	Frame(int AnchorX, 
		  int AnchorY, 
		  int PosX, 
		  int PosY, 
		  int width, 
		  int height, 
		  int cX, 
		  int cY, 
		  int cWidth, 
		  int cHeight, 
		  double nTime );

	~Frame();

	//Accessors
	POINT GetAnchor() { return Anchor; }
	RECT GetImageRect() { return ImageRect; }
	RECT GetCollisionRect() { return CollisionRect; }
	float GetDuration() { return m_fDuration; }
	float GetWidth() { return static_cast<float>(ImageRect.right-ImageRect.left); }
	float GetHeight() { return static_cast<float>(ImageRect.bottom-ImageRect.top); }

};


#endif