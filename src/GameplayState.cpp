#include <stdio.h>
#include <iostream>
#include "GameplayState.h" // The current state
#include "MainMenuState.h" // The state to load (1)
#include "GameoverState.h" // The state to load (2)

// For me
#define DAVE 0
#define NIGEL 1
#define MANDY 2

GameplayState GameplayState::m_GameplayState;

void GameplayState::Init()
{
	IwGetResManager()->LoadGroup("sprites.group");

	// Dave (big), Nigel (small), Mandy (girl)
	characters[0] = new Sprite("dave");
	characters[0]->SetCenter(CIwSVec2((int16)characters[0]->GetWidth() /2 , (int16)characters[0]->GetHeight() /2));
	characters[0]->SetPosition(CIwFVec2(50,0));

	characters[1] = new Sprite("nigel");
	characters[1]->SetCenter(CIwSVec2((int16)characters[1]->GetWidth() /2, (int16)characters[1]->GetHeight() /2));
	characters[1]->SetPosition(CIwFVec2(414, 100));

	characters[2] = new Sprite("mandy");
	characters[2]->SetCenter(CIwSVec2((int16)characters[2]->GetWidth() /2, (int16)characters[2]->GetHeight() /2));
	characters[2]->SetPosition(CIwFVec2(64, 150));

	m_Portraits[0] = new Sprite("dave_port");
	m_Portraits[0]->SetPosition(CIwFVec2(130,0));
	m_Portraits[0]->BuildCollision("portraits\\dave_port.png");

	m_Portraits[1] = new Sprite("nigel_port");
	m_Portraits[1]->SetPosition(CIwFVec2(210,0));
	m_Portraits[1]->BuildCollision("portraits\\nigel_port.png");

	m_Portraits[2] = new Sprite("mandy_port");
	m_Portraits[2]->SetPosition(CIwFVec2(290,0));
	m_Portraits[2]->BuildCollision("portraits\\mandy_port.png");

	n_guiButtons[0] = new Sprite("touchScreenMoveL");
	n_guiButtons[0]->SetPosition(CIwFVec2(0, 260));
	n_guiButtons[0]->BuildCollision("textures\\touchScreenMoveL.bmp");

	n_guiButtons[1] = new Sprite("touchScreenMoveR");
	n_guiButtons[1]->SetPosition(CIwFVec2(414, 260));
	n_guiButtons[1]->BuildCollision("textures\\touchScreenMoveR.bmp");

	m_throwingTarget = new Sprite("target_sprite");

	screenHeight = Iw2DGetSurfaceHeight();
	screenWidth = Iw2DGetSurfaceWidth();

	m_Font = Iw2DCreateFontResource("font_small");
	Iw2DSetFont(m_Font);

	for (int i = 0; i < 3; i++)
		characters[i]->ShowColliderPos = true;

	m_CharacterIndex = 0;
	m_isThrowing = false;
	m_isTermActive = false;
	m_canThrow = false;
	m_SpacePressed = false;
	m_UpPressed = false;

	m_Cam = new Camera;
	m_Cam->SetPosition(CIwSVec2(0, 0));
	m_Cam->Position = CIwSVec2(0, 0);

	m_Level = new TileMap("levels\\levelproto.txt");
	m_MouseClicked = false;

	if (s3eAudioIsCodecSupported(S3E_AUDIO_CODEC_MP3))
		s3eAudioPlay("audio\\bgmusic.mp3", 0);

	printf("GameplayState initialized\n");
}

void GameplayState::Destroy()
{
	for (int i = 0; i < 3; i++)
	{
		delete characters[i];
		delete m_Portraits[i];
	}

	for (int i = 0; i < 2; i++)
		delete n_guiButtons[i];

	if (m_Font != NULL)
		delete m_Font;

	delete m_Cam;

	IwGetResManager()->DestroyGroup("Sprites");
	printf("GameplayState Destroyed\n");

}

void GameplayState::Pause()
{

}

void GameplayState::Resume()
{

}

void GameplayState::HandleEvent(StateEngine* state)
{
	if ( (s3eKeyboardGetState(s3eKeySpace) & S3E_POINTER_STATE_DOWN) && m_SpacePressed == false)
	{
		std::cout << "Space pressed" << std::endl;
		if (m_CharacterIndex == MANDY){
			characters[MANDY]->TEMP_JUSTJUMPED = true;}
		m_SpacePressed = true;
	}

	if ( (s3eKeyboardGetState(s3eKeyUp) & S3E_POINTER_STATE_DOWN) && m_UpPressed == false)
	{
		std::cout << "Up pressed" << std::endl;
		m_UpPressed = true;
	}
	
    if( (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN))
    {
		for (int i = 0; i < 3; i++)
		{
			// Check collision with the character portraits
			if (m_Portraits[i]->isColliding(CIwFVec2((float)s3ePointerGetX(), (float)s3ePointerGetY())))
			{
				if (m_CharacterIndex != i)
				{
					m_Cam->SetPosition(CIwSVec2(static_cast<int16>(-characters[i]->GetPosition().x + (screenWidth /2)), static_cast<int16>(-characters[i]->GetPosition().y + (screenHeight - characters[i]->GetHeight()))));
					m_CharacterIndex = i; // Set the character to be the element that was collided against
				}
			}
		}

		if (n_guiButtons[0]->isColliding((CIwFVec2((float)s3ePointerGetX(), (float)s3ePointerGetY())))) // Left Arrow Button
		{
			characters[m_CharacterIndex]->MoveBy(CIwSVec2(-10, 0));
			m_Cam->MoveBy(CIwSVec2(10,0));
		}

		if (n_guiButtons[1]->isColliding((CIwFVec2((float)s3ePointerGetX(), (float)s3ePointerGetY()))))
		{
			characters[m_CharacterIndex]->MoveBy(CIwSVec2(10, 0));
			m_Cam->MoveBy(CIwSVec2(-10, 0));
		}


		if (characters[DAVE]->isColliding(characters[NIGEL]->GetPosition()) && (m_canThrow == false) && m_CharacterIndex == DAVE)
		{
			if (characters[NIGEL]->isColliding((CIwFVec2(s3ePointerGetX() - (float)m_Cam->GetPosition().x , s3ePointerGetY() - (float)m_Cam->GetPosition().y ))))
			{
				while (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN)
				{
					// Continue to poll input
					s3ePointerUpdate();
					s3eKeyboardUpdate();
					m_canThrow = true;
					m_throwingTarget->SetPosition((CIwFVec2(s3ePointerGetX() - (float)m_Cam->GetPosition().x , s3ePointerGetY() - (float)m_Cam->GetPosition().y )));
					m_throwingTarget->UpdateCollider();
					
					// Draw the current state or else we'll be in a loop that will show no updated frame
					state->Draw();
				}

				m_isThrowing = true;
				m_CharacterIndex = NIGEL;
			}
		}

		m_MouseClicked = true;
	}
	if (s3ePointerGetState(S3E_POINTER_BUTTON_LEFTMOUSE) == 4)
		m_MouseClicked = false;

	if (m_isThrowing)
	{
		characters[NIGEL]->LerpTo(CIwFVec2(m_throwingTarget->GetPosition().x, m_throwingTarget->GetPosition().y), 0.05f);
		characters[NIGEL]->TEMP_ISFALLING = false;
		characters[NIGEL]->UpdateCollider();
		if (characters[NIGEL]->isColliding(m_throwingTarget))
		{
			m_canThrow = false;
			m_isThrowing = false;
			characters[NIGEL]->TEMP_ISFALLING = true;
			characters[NIGEL]->TEMP_ISCOLLIDING = false;
		}
	}
}

void GameplayState::Update(StateEngine* state, double dt)
{
	CheckInterations(state);

	characters[DAVE]->Update(dt);
	characters[MANDY]->Update(dt);
	if (!m_isThrowing)
		characters[NIGEL]->Update(dt);

	// Set the camera's position to the currently controlled player, at the bottom and (roughly) towards the center of the screen.
	m_Cam->SetPosition(CIwSVec2(static_cast<int16>(-characters[m_CharacterIndex]->GetPosition().x + (screenWidth /2)), static_cast<int16>(-characters[m_CharacterIndex]->GetPosition().y + (screenHeight - characters[m_CharacterIndex]->GetHeight()))));

	if (s3eKeyboardGetState(s3eKeyUp) == 4)
		m_UpPressed = false;
	if (s3eKeyboardGetState(s3eKeySpace) == 4)
		m_SpacePressed = false;
}

void GameplayState::CheckInterations(StateEngine* state)
{
	int count = 0;
	int exitCount = 0;
	// Check if the characters are colliding with any floor tiles, if they are set their collision value to true and break, if not continue to poll the remaining tiles.
	for (int c = 0; c < 3; c++)
	{
		for (size_t m = 0; m < m_Level->GetMap().size(); m++)
		{
			bool v = m_Level->GetMap().at(m)->isColliding(characters[c]);

			if (v == true)
			{
				characters[c]->TEMP_ISCOLLIDING = true;
				break;
			}
			else
			{
				characters[c]->TEMP_ISCOLLIDING = false;
			}
		}
	}

	for (size_t s = 0; s < m_Level->GetObjects().size(); s++)
	{
		GameObject *t = m_Level->GetObjects().at(s);
		t->UpdateCollider();
		// Test if any of the characters hit this object
		for (int i = 0; i < 3; i++)
		{
			if (t->GetType() == Button)
			{
				if (characters[i]->isColliding(t))
				{
					t->Child()->IsActive = false;
					count++;
				}
				else
				{
					if (count == 0)
						t->Child()->IsActive = true;
				}
			}
		}

		if (t->GetType() == Terminal)
		{
			if (characters[MANDY]->isColliding(t))
			{
				m_isTermActive = true;
				m_activeTerminal = t;
			}
			else
				m_isTermActive = false;
		}

		// Check to see if all the characters have reached the exit, if this is true, load the game over screen
		for (int i = 0; i < 3; i++)
		{
			if (t->GetType() == Exit)
			{
				if (characters[i]->isColliding(t))
					exitCount++;
			}

			if (exitCount == 3)
				state->ChangeState(GameoverState::Instance());
		}
	}

	if (m_CharacterIndex == MANDY)
	{
		if (m_isTermActive)
		{
			if (m_UpPressed == true)
			{
				if (characters[DAVE]->isColliding(m_activeTerminal->Child()))
					characters[DAVE]->SetPosition(CIwFVec2(m_activeTerminal->Child()->GetPosition().x, characters[DAVE]->GetPosition().y - 150));

				if (characters[NIGEL]->isColliding(m_activeTerminal->Child()->GetPosition()))
					characters[NIGEL]->SetPosition(CIwFVec2(m_activeTerminal->Child()->GetPosition().x, characters[NIGEL]->GetPosition().y - 110));

				m_activeTerminal->Child()->SetPosition(CIwFVec2(m_activeTerminal->Child()->GetPosition().x, characters[MANDY]->GetPosition().y + 54));
				//m_UpPressed = false;
			}
		}
	}
}

void GameplayState::Draw(StateEngine* state)
{
	m_Level->Draw();

	for (int i = 0; i <3; i++)
	{
		characters[i]->Draw();
		m_Portraits[i]->Draw(m_Cam->GetPosition());		
	}

	if (m_canThrow)
		m_throwingTarget->Draw();

	for (int i = 0; i < 2; i++)
		n_guiButtons[i]->Draw(m_Cam->GetPosition());
}