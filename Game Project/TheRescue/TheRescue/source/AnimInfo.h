/*
	File: AnimInfo.h
	Author: Gabriel Islas
	Class: SGP
	Purpose: To manage the copies of the animations for all game objects to use. 
*/
#ifndef ANIM_INFO_H
#define ANIM_INFO_H

class AnimInfo
{

	//Private data members 
	int curFrame;
	int curAnim;
	float m_fTimeOnFrame;
public:
	//Constructor
	AnimInfo();
	//Destructor
	~AnimInfo();

	//Accessors/Mutators
	int GetCurFrame() { return curFrame;}
	int GetCurAnim() { return curAnim;}
	float GetTimeOnFrame() { return m_fTimeOnFrame; }
	void SetTimeOnFrame(float fTimeOnFrame) { m_fTimeOnFrame = fTimeOnFrame;}
	void AddTimeOnFrame(float fTimeOnFrame) { m_fTimeOnFrame += fTimeOnFrame;}
	void UpdateCurrentFrame() { curFrame++; }
	void SetCurFrame(int cFrame) { curFrame = cFrame;}
	void SetCurAnim( int anim) { curAnim = anim, curFrame = 0, m_fTimeOnFrame = 0; } //Set the anim and reset the
	// other attributes. 


};

#endif