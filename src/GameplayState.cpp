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
	
	m_Portraits[0] = new Sprite("dave_port", true);
	m_Portraits[0]->SetPosition(CIwFVec2(130,0));
	m_Portraits[0]->BuildCollision("portraits\\dave_port.png");

	m_Portraits[1] = new Sprite("nigel_port", true);
	m_Portraits[1]->SetPosition(CIwFVec2(210,0));
	m_Portraits[1]->BuildCollision("portraits\\nigel_port.png");

	m_Portraits[2] = new Sprite("mandy_port", true);
	m_Portraits[2]->SetPosition(CIwFVec2(290,0));
	m_Portraits[2]->BuildCollision("portraits\\mandy_port.png");

	n_guiButtons[0] = new Sprite("touchScreenMoveL", true);
	n_guiButtons[0]->SetPosition(CIwFVec2(0, 260));
	n_guiButtons[0]->BuildCollision("textures\\touchScreenMoveL.bmp");

	n_guiButtons[1] = new Sprite("touchScreenMoveR", true);
	n_guiButtons[1]->SetPosition(CIwFVec2(414, 260));
	n_guiButtons[1]->BuildCollision("textures\\touchScreenMoveR.bmp");


	m_throwingTarget = new Sprite("target_sprite", true);

	screenHeight = Iw2DGetSurfaceHeight();
	screenWidth = Iw2DGetSurfaceWidth();

	m_Font = Iw2DCreateFontResource("font_small");
	Iw2DSetFont(m_Font);

	m_CharacterIndex = 0;
	m_isThrowing = false;
	m_isTermActive = false;
	m_canThrow = false;
	m_SpacePressed = false;
	m_UpPressed = false;
	m_gameOver = false;
	m_MouseClicked = false;
	TEMP_HASPLAYED = false;


	m_Cam = new Camera;
	m_Cam->SetPosition(CIwSVec2(0, 0));
	m_Cam->Position = CIwSVec2(0, 0);

	m_Level = new TileMap("levels\\levelproto1.txt", "levels\\levelrelationships1.txt");
	SpawnCharacters();

	m_Layers.push_back(new Sprite("layer1", false));
	m_Layers.push_back(new Sprite("layer2", false));
	m_Layers.push_back(new Sprite("layer3", false));
	m_Layers.push_back(new Sprite("layer4", false));

	if (s3eAudioIsCodecSupported(S3E_AUDIO_CODEC_MP3))
		s3eAudioPlay("audio\\bgmusic.mp3", 0);

	buttonSound = static_cast<CIwSoundSpec*>(IwGetResManager()->GetResNamed("button_clicked", "CIwSoundSpec"));
	buttonSoundInst = NULL;
	terminalSound = static_cast<CIwSoundSpec*>(IwGetResManager()->GetResNamed("terminal_selected", "CIwSoundSpec"));
	terminalInst = NULL;
	doorSound = static_cast<CIwSoundSpec*>(IwGetResManager()->GetResNamed("locked_door", "CIwSoundSpec"));
	doorSoundInst = NULL;
	printf("GameplayState initialized\n");
}

void GameplayState::Destroy()
{
	for (int i = 0; i < 3; i++)
	{
		delete characters[i];
		delete m_Portraits[i];
		//delete m_PortraitSounds[i];
	}

	for (int i = 0; i < 2; i++)
		delete n_guiButtons[i];

	if (m_Font != NULL)
		delete m_Font;

	delete m_Cam;
	delete m_Level;

	//IwGetResManager()->DestroyGroup("Sprites");
	printf("GameplayState Destroyed\n");

}

void GameplayState::Pause()
{

}

void GameplayState::Resume()
{

}

void GameplayState::SpawnCharacters()
{
	// Dave (big), Nigel (small), Mandy (girl)
	characters[0] = new Sprite("dave_anim", true, CIwFVec2(4,1));
	characters[0]->SetCenter(CIwSVec2((int16)characters[0]->GetWidth() /2 , (int16)characters[0]->GetHeight() /2));
	characters[0]->SetPosition(m_Level->GetSpawnPositions().at(DAVE));
	characters[0]->SetMovSpeed(CIwFVec2(2,0)); // Moves 2 units fast in the x axis (slow)
	m_PortraitSounds[0] = static_cast<CIwSoundSpec*>(IwGetResManager()->GetResNamed("dave_selected", "CIwSoundSpec"));

	characters[1] = new Sprite("nigel_anim", true, CIwFVec2(4,1));
	characters[1]->SetCenter(CIwSVec2((int16)characters[1]->GetWidth() /2, (int16)characters[1]->GetHeight() /2));
	characters[1]->SetPosition(m_Level->GetSpawnPositions().at(NIGEL));
	characters[1]->SetMovSpeed(CIwFVec2(5,0)); // Moves 5 units fast in the x axis (fastest)
	m_PortraitSounds[1] = static_cast<CIwSoundSpec*>(IwGetResManager()->GetResNamed("nigel_selected", "CIwSoundSpec"));

	characters[2] = new Sprite("mandy_anim", true, CIwFVec2(4,1));
	characters[2]->SetCenter(CIwSVec2((int16)characters[2]->GetWidth() /2, (int16)characters[2]->GetHeight() /2));
	characters[2]->SetPosition(m_Level->GetSpawnPositions().at(MANDY));
	characters[2]->SetMovSpeed(CIwFVec2(3,0)); // Moves 3 units fast in the x axis (faster than dave, slower than nigel)
	m_PortraitSounds[2] = static_cast<CIwSoundSpec*>(IwGetResManager()->GetResNamed("mandy_selected", "CIwSoundSpec"));

	for (int i = 0; i < 3; i++)
	{
		characters[i]->ShowColliderPos = true;
	}

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
					m_PortraitSounds[i]->Play();
				}
			}
		}

		if (n_guiButtons[0]->isColliding((CIwFVec2((float)s3ePointerGetX(), (float)s3ePointerGetY()))) && (!characters[m_CharacterIndex]->CollisionLocation.Left) && (characters[m_CharacterIndex]->GetPosition().x > 32)) // Left Arrow Button
		{
			if (characters[m_CharacterIndex]->GetDirection() == FACING_RIGHT)
				characters[m_CharacterIndex]->SetDirection(FACING_LEFT);

			CIwFVec2 val = CIwFVec2((-5 * state->m_deltaTime) - characters[m_CharacterIndex]->GetMovSpeed().x, 0);
			characters[m_CharacterIndex]->MoveBy(val, state->m_deltaTime);
			//ScrollBackground(val);
		}

		if (n_guiButtons[1]->isColliding((CIwFVec2((float)s3ePointerGetX(), (float)s3ePointerGetY()))) && (!characters[m_CharacterIndex]->CollisionLocation.Right))
		{
			if (characters[m_CharacterIndex]->GetDirection() == FACING_LEFT)
				characters[m_CharacterIndex]->SetDirection(FACING_RIGHT);

			CIwFVec2 val = CIwFVec2((5 * state->m_deltaTime) + characters[m_CharacterIndex]->GetMovSpeed().x, 0);
			characters[m_CharacterIndex]->MoveBy(val, state->m_deltaTime);
			//ScrollBackground(val);
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
					// Set the target to reach to be where the center of the pointer is.
					m_throwingTarget->SetPosition((CIwFVec2((s3ePointerGetX() - (float)m_Cam->GetPosition().x)  - ( m_throwingTarget->GetWidth() /2) , (s3ePointerGetY() - (float)m_Cam->GetPosition().y ) - (m_throwingTarget->GetHeight() /2))));
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

	if (m_gameOver)
		state->ChangeState(GameoverState::Instance());
}

void GameplayState::Update(StateEngine* state, double dt)
{
	CheckInterations(state);

	characters[DAVE]->Update(dt);
	characters[MANDY]->Update(dt);
	if (!m_isThrowing)
		characters[NIGEL]->Update(dt);

	// Set the camera's position to the currently controlled player, at the bottom and (roughly) towards the center of the screen.
	m_Cam->SetPosition(
		CIwSVec2(static_cast<int16>(-characters[m_CharacterIndex]->GetPosition().x + (screenWidth /2)),
		static_cast<int16>(-characters[m_CharacterIndex]->GetPosition().y + (screenHeight - characters[m_CharacterIndex]->GetHeight() - 32))));

	if (s3eKeyboardGetState(s3eKeyUp) == 4)
		m_UpPressed = false;
	if (s3eKeyboardGetState(s3eKeySpace) == 4)
		m_SpacePressed = false;
}

void GameplayState::CheckInterations(StateEngine* state)
{
	int count = 0;
	int exitCount = 0;

	int debug_ele = 0;
	// Check if the characters are colliding with any floor tiles, if they are set their collision value to true and break, if not continue to poll the remaining tiles.
	for (int c = 0; c < 3; c++)
	{
		for (size_t m = 0; m < m_Level->GetMap().size(); m++)
		{
			GameObject *t = m_Level->GetMap().at(m);
			bool v = t->isColliding(characters[c]);

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

		// Check to see if any of the characters are colliding with 
		for (size_t o = 0; o < m_Level->GetObjects().size(); o++)
		{
			GameObject *t = m_Level->GetObjects().at(o);
			// Check to see if someone is colliding with an active door, if so, deny access
			if (characters[c]->isColliding(t))
			{
				if (t->GetType() == Door)
				{
					if (t->IsActive)
					{
						if (characters[c]->CollisionLocation.Right == false && characters[c]->CollisionLocation.Left == false){
						if (doorSoundInst == NULL)
						{
							doorSoundInst = doorSound->Play();
						}
						else
						{
							if (!doorSoundInst->IsPlaying())
								doorSound->Play();
						}}

						if (characters[c]->GetPosition().x < t->GetPosition().x)
							characters[c]->CollisionLocation.Right = true; //characters[c]->SetPosition(CIwFVec2(characters[c]->GetPosition().x - (5.2 * state->m_deltaTime), characters[c]->GetPosition().y));
						else
							characters[c]->CollisionLocation.Left = true;
					}
				}
				c++;
			}
			else
			{
				characters[c]->CollisionLocation.Right = false;
				characters[c]->CollisionLocation.Left = false;
			}
		}
	}

	for (size_t s = 0; s < m_Level->GetObjects().size(); s++)
	{
		debug_ele++;

		GameObject *t = m_Level->GetObjects().at(s);
		t->UpdateCollider();
		// Test if any of the characters hit this object
		for (int i = 0; i < 3; i++)
		{
			// First of all, test to see if a character hits something.
			if (t->GetType() == Button)
			{
				if (characters[i]->isColliding(t))
				{
					if (t->Child()->GetType() == Door)
					{
						t->Child()->IsActive = false;
						count++;
						//if (TEMP_HASPLAYED == false)
						//{
						buttonSound->Play();
						//EMP_HASPLAYED = true;
						//}
					}

					if ((i == NIGEL) && (t->Child()->GetType() == Elevator))
					{
						if (characters[DAVE]->isColliding(t->Child()))
							characters[DAVE]->SetPosition(CIwFVec2(t->Child()->GetPosition().x, characters[NIGEL]->GetPosition().y - 182));

						if (characters[MANDY]->isColliding(t->Child()))
							characters[MANDY]->SetPosition(CIwFVec2(t->Child()->GetPosition().x, characters[NIGEL]->GetPosition().y - 142));

						t->Child()->SetPosition(CIwFVec2(t->Child()->GetPosition().x, characters[NIGEL]->GetPosition().y + 54));
					}
				}
				else
				{
					if (count == 0)
					{
						t->Child()->IsActive = true;
					}
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
				if (characters[i]->isColliding(t) && exitCount !=3)
					exitCount++;
				if (exitCount == 3)
				{
					m_gameOver = true;
					//break;
				}
			}
		}
	}

	if (m_CharacterIndex == MANDY)
	{
		if (m_isTermActive)
		{
			if (m_UpPressed == true)
			{

				if (m_activeTerminal->Child()->GetType() == Elevator)
				{
					terminalSound->Play();

					if (characters[DAVE]->isColliding(m_activeTerminal->Child()))
						characters[DAVE]->SetPosition(CIwFVec2(m_activeTerminal->Child()->GetPosition().x, characters[DAVE]->GetPosition().y - 150));

					if (characters[NIGEL]->isColliding(m_activeTerminal->Child()))
						characters[NIGEL]->SetPosition(CIwFVec2(m_activeTerminal->Child()->GetPosition().x, characters[NIGEL]->GetPosition().y - 110));

					m_activeTerminal->Child()->SetPosition(CIwFVec2(m_activeTerminal->Child()->GetPosition().x, characters[MANDY]->GetPosition().y + 54));
				}

				if (m_activeTerminal->Child()->GetType() == Door)
					m_activeTerminal->Child()->IsActive = false;
			}
		}
	}
}

void GameplayState::Draw(StateEngine* state)
{

	for (int i = 0; i < 4; i++)
	{
		m_Layers.at(i)->Draw(m_Cam);
	}

	m_Level->Draw(m_Cam, state->m_deltaTime); // TODO: Don't draw elements in the level that are off screen

	for (int i = 0; i <3; i++)
	{
		characters[i]->Draw();
		m_Portraits[i]->Draw(m_Cam);		
	}

	if (m_canThrow)
		m_throwingTarget->Draw();

	for (int i = 0; i < 2; i++)
		n_guiButtons[i]->Draw(m_Cam);
}

void GameplayState::ScrollBackground(CIwFVec2& scrollBy)
{
	for (int i = 0; i < m_Layers.size(); i++)
	{
		m_Layers.at(i)->MoveBy(
			CIwFVec2(scrollBy.x - (i * 1.5), (0 * i) - scrollBy.y), 0);
	}
}