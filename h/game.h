#ifndef GAME_H
#define GAME_H

#include <vector>
#include "sprite.h"
#include "GameObject.h"
#include "camera.h"
#include "TileMap.h"

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
	void DrawText(int16, int16);
	void CheckInterations();

private:
    CIwFVec2 m_Position;
    CIwSVec2 m_Size;

	uint64 m_LastUpdate;

	int m_TimeToOneSec;
	int m_CountUpdates;
	int m_UpdatesPerSec;

	int screenWidth;
	int screenHeight;

	bool m_MouseClicked;
	bool m_SpacePressed;
	bool m_UpPressed;

	Sprite* m_throwingTarget;
	Sprite* n_guiButtons[2];
	Sprite* characters[3];
	Sprite* m_Portraits[3];
	GameObject* m_activeTerminal;

	CIw2DFont* m_Font;

	int TEMP_charIndex;
	bool TEMP_termActive;
	bool TEMP_isThrowing;
	bool m_canThrow;
	CIwFVec2 TEMP_target;

	Camera* m_Cam;
	TileMap* m_Level;
};

#endif
