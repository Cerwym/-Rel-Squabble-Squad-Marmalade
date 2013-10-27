#ifndef GAME_H
#define GAME_H

#include <vector>
#include "sprite.h"
#include "camera.h"

class CGame
{
public:
    CGame();
    ~CGame();

    // update will be called a fixed number of times per second 
    // regardless of visual framerate
    void Update();
    // render will be called as fast as possible (but not faster 
    // than the update rate)
    void Render();

	void DrawBackGround(CIw2DImage* img, int x0, int y0, int w, int h);

protected:
	void DrawTouchButtons();
	// Draw a sprite, centered at the specified position with a given size
	void DrawSpriteCentered(CIw2DImage*, int, int, int);

private:
    CIwFVec2 m_Position;
    CIwSVec2 m_Size;

	uint64 m_LastUpdate;

	int m_TimeToOneSec;
	int m_CountUpdates;
	int m_UpdatesPerSec;

	int screenWidth;
	int screenHeight;

	CIw2DFont* m_Font;

	// Temporary stuff
	CIw2DImage* m_Layer1;
	CIw2DImage* m_Layer2;
	CIw2DImage* m_Layer3;
	CIw2DImage* m_Layer4;
	CIw2DImage* m_Floor;
	CIw2DImage* guiButtons[2];

	Sprite* characters[3];

	int TEMP_charIndex;
	bool TEMP_isThrowing;
	CIwFVec2 TEMP_target;

	std::vector<CIwFVec2> layerLocs;

	Camera* m_Cam;

};

#endif
