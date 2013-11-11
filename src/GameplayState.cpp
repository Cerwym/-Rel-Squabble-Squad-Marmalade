#include <stdio.h>
#include <iostream>
#include "GameplayState.h" // The current state
#include "MainMenuState.h" // The state to load

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
	characters[0]->ShowColliderPos = true;

	characters[1] = new Sprite("nigel");
	characters[1]->SetCenter(CIwSVec2((int16)characters[1]->GetWidth() /2, (int16)characters[1]->GetHeight() /2));
	characters[1]->SetPosition(CIwFVec2(44, 100));
	characters[1]->ShowColliderPos = true;

	characters[2] = new Sprite("mandy");
	characters[2]->SetCenter(CIwSVec2((int16)characters[2]->GetWidth() /2, (int16)characters[2]->GetHeight() /2));
	characters[2]->SetPosition(CIwFVec2(64, 150));
	characters[2]->ShowColliderPos = true;

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

	m_throwingTarget = new Sprite("nigel_port");
	m_throwingTarget->ShowColliderPos = true;

	screenHeight = Iw2DGetSurfaceHeight();
	screenWidth = Iw2DGetSurfaceWidth();

	m_Font = Iw2DCreateFontResource("font_small");
	Iw2DSetFont(m_Font);

	TEMP_charIndex = 0;
	TEMP_isThrowing = false;
	TEMP_termActive = false;
	m_canThrow = false;
	m_SpacePressed = false;
	m_UpPressed = false;

	m_Cam = new Camera;
	m_Cam->SetPosition(CIwSVec2(0, 0));
	m_Cam->Position = CIwSVec2(0, 0);

	m_Level = new TileMap("levels\\levelproto.txt");
	m_MouseClicked = false;

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
		if (TEMP_charIndex == MANDY){
			characters[MANDY]->TEMP_JUSTJUMPED = true;}
		m_SpacePressed = true;
	}

	if ( (s3eKeyboardGetState(s3eKeyUp) & S3E_POINTER_STATE_DOWN) && m_UpPressed == false)
	{
		std::cout << "Up pressed" << std::endl;
		m_UpPressed = true;
	}
	
	// Input

	if( (s3ePointerGetState(S3E_POINTER_BUTTON_RIGHTMOUSE) & S3E_POINTER_STATE_DOWN))
	{
		characters[NIGEL]->Debug_PrintPos();
	}

    if( (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN))
    {
		for (int i = 0; i < 3; i++)
		{
			// Check collision with the character portraits
			if (m_Portraits[i]->isColliding(CIwFVec2((float)s3ePointerGetX(), (float)s3ePointerGetY())))
			{
				if (TEMP_charIndex != i)
				{
					m_Cam->SetPosition(CIwSVec2(static_cast<int16>(-characters[i]->GetPosition().x + (screenWidth /2)), static_cast<int16>(-characters[i]->GetPosition().y + (screenHeight - characters[i]->GetHeight()))));
					TEMP_charIndex = i; // Set the character to be the element that was collided against
				}
				if (characters[i]->isColliding(CIwFVec2((float)s3ePointerGetX(), (float)s3ePointerGetY())))
				{
					//
				}
			}
		}

		if (n_guiButtons[0]->isColliding((CIwFVec2((float)s3ePointerGetX(), (float)s3ePointerGetY())))) // Left Arrow Button
		{
			characters[TEMP_charIndex]->MoveBy(CIwSVec2(-10, 0));
			m_Cam->MoveBy(CIwSVec2(10,0));
		}

		if (n_guiButtons[1]->isColliding((CIwFVec2((float)s3ePointerGetX(), (float)s3ePointerGetY()))))
		{
			characters[TEMP_charIndex]->MoveBy(CIwSVec2(10, 0));
			m_Cam->MoveBy(CIwSVec2(-10, 0));
		}


		if (characters[DAVE]->isColliding(characters[NIGEL]->GetPosition()) && (m_canThrow == false) && TEMP_charIndex == DAVE)
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
					
					// change this to draw the current state
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
					Iw2DSurfaceShow();
				}

				TEMP_isThrowing = true;
				TEMP_charIndex = NIGEL;
			}
		}

		m_MouseClicked = true;
	}
	if (s3ePointerGetState(S3E_POINTER_BUTTON_LEFTMOUSE) == 4)
		m_MouseClicked = false;

	if (TEMP_isThrowing)
	{
		characters[NIGEL]->LerpTo(CIwFVec2(m_throwingTarget->GetPosition().x, m_throwingTarget->GetPosition().y), 0.05f);
		characters[NIGEL]->TEMP_ISFALLING = false;
		characters[NIGEL]->UpdateCollider();
		//Dirty fix, collision detection is not quite accurate
		float xComp = m_throwingTarget->GetPosition().x - characters[NIGEL]->GetPosition().x;
		float yComp = m_throwingTarget->GetPosition().y - characters[NIGEL]->GetPosition().y;
	
		if ((xComp >= -5.05 && xComp <= 5.05) && yComp >= -5.05 && yComp <= 5.05)
		{
			std::cout << "Reached target" << std::endl;
			m_canThrow = false;
			TEMP_isThrowing = false;
			characters[NIGEL]->TEMP_ISFALLING = true;
			characters[NIGEL]->TEMP_ISCOLLIDING = false;
		}

		if (characters[NIGEL]->isColliding(m_throwingTarget))
		{
			m_canThrow = false;
			TEMP_isThrowing = false;
			characters[NIGEL]->TEMP_ISFALLING = true;
			characters[NIGEL]->TEMP_ISCOLLIDING = false;
		}
	}

	// Temporary fix
	if (characters[NIGEL]->GetPosition().x >= 950 && characters[NIGEL]->GetPosition().x <= 1034)
	{
		if (characters[NIGEL]->GetPosition().y >= 244.5 && characters[NIGEL]->GetPosition().y <= 266.5)
		{
			std::cout << "hit the gap" << std::endl;
			characters[NIGEL]->TEMP_ISCOLLIDING = false;
		}
	}
}

void GameplayState::Update(StateEngine* state, double dt)
{
	CheckInterations();

	for (int i = 0; i < 3; i++)
	{
		characters[i]->Update(dt);
		characters[i]->UpdateCollider();
	}

	std::cout << "Dave's collision is " << characters[DAVE]->TEMP_ISCOLLIDING << std::endl;

	m_Cam->SetPosition(CIwSVec2(static_cast<int16>(-characters[TEMP_charIndex]->GetPosition().x + (screenWidth /2)), static_cast<int16>(-characters[TEMP_charIndex]->GetPosition().y + (screenHeight - characters[TEMP_charIndex]->GetHeight()))));

	if (s3eKeyboardGetState(s3eKeyUp) == 4)
		m_UpPressed = false;
	if (s3eKeyboardGetState(s3eKeySpace) == 4)
		m_SpacePressed = false;
}

void GameplayState::CheckInterations()
{
	int count = 0;

	// This one works. Characters -> elements to collide against; if collison -> break;
	for (int c = 0; c < 3; c++)
	{
		for (size_t m = 0; m < m_Level->GetMap().size(); m++)
		{
			bool value = m_Level->GetMap().at(m)->isColliding(characters[c]);

			if (value == true)
			{
				characters[c]->TEMP_ISCOLLIDING = true;
				break;
			}
			else
			{
				characters[c]->TEMP_ISCOLLIDING = false;
				break;
			}
		}
	}



	for (size_t s = 0; s < m_Level->GetMap().size(); s++)
	{
		for (int i = 0; i < 3; i++)
		{

			bool value = m_Level->GetMap().at(s)->isColliding(characters[i]);

			if (value == true)
			{
				//std::cout << "Colliding with something" << std::endl;
				characters[i]->TEMP_ISCOLLIDING = true;
				//i++;
				// do an for int = characters, if val = true, break.
			}
		}
	}

	for (size_t s = 0; s < m_Level->GetObjects().size(); s++)
	{
		GameObject *t = m_Level->GetObjects().at(s);
		t->UpdateCollider();

		if (characters[DAVE]->isColliding(t))
			std::cout << "NEW COLLISION ON OBJ" << std::endl;
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
				if (TEMP_charIndex == MANDY){
					std::cout << "Mandy is Colliding with terminal" << std::endl;}
				TEMP_termActive = true;
				m_activeTerminal = t;
			}
			else
			{
				TEMP_termActive = false;
			}
			//std::cout << "Terminal is " << TEMP_termActive << std::endl;
		}
	}

	// REMOVE THIS
	if (TEMP_termActive)
	{
		if (characters[NIGEL]->isColliding(m_activeTerminal->Child()))
		{
			std::cout << "Nigel -> ";characters[NIGEL]->Debug_PrintPos();
		}
		if (characters[DAVE]->isColliding(m_activeTerminal->Child()))
		{
			std::cout << "DAVE is colliding" << std::endl;
		}
	}


	if (TEMP_charIndex == MANDY)
	{
		if (TEMP_termActive)
		{
			if (m_UpPressed == true)
			{
				if (characters[DAVE]->isColliding(m_activeTerminal->Child()->GetPosition()))
				{
					std::cout << "DAVE is colliding" << std::endl;
					characters[DAVE]->SetPosition(CIwFVec2(m_activeTerminal->Child()->GetPosition().x, characters[DAVE]->GetPosition().y - 150));
				}

				std::cout << "Nigel -> ";characters[NIGEL]->Debug_PrintPos();
				std::cout << "Lift -> ";m_activeTerminal->Child()->Debug_PrintPos();
				if (characters[NIGEL]->isColliding(m_activeTerminal->Child()->GetPosition())) // +8
				{
					std::cout << "Nigel is colliding" << std::endl;
					characters[NIGEL]->SetPosition(CIwFVec2(m_activeTerminal->Child()->GetPosition().x, characters[NIGEL]->GetPosition().y - 135));
				}

				m_activeTerminal->Child()->SetPosition(CIwFVec2(m_activeTerminal->Child()->GetPosition().x, characters[MANDY]->GetPosition().y + 54));
				m_UpPressed = false;
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