/*
	File: AnimInfo.cpp
	Author: Gabriel Islas
	Class: SGP
	Purpose: To manage the copies of the animations for all game objects to use. 
*/
#include "AnimInfo.h"

AnimInfo::AnimInfo()
{
	curFrame = 0;
	curAnim = 0;
	m_fTimeOnFrame = 0.0f;
}

AnimInfo::~AnimInfo()
{
}