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

	m_CharacterIndex = 0;
	m_isThrowing = false;
	m_isTermActive = false;
	m_canThrow = false;
	m_SpacePressed = false;
	m_gameOver = false;
	m_MouseClicked = false;
	TEMP_HASPLAYED = false;
	m_ClickLocation = CIwFVec2(0,0);
	m_Level = new TileMap("levels\\levelproto1.txt", "levels\\levelrelationships1.txt");
	m_Cam = new Camera(screenWidth, screenHeight);
	m_Cam->SetPosition(CIwSVec2(0, 0));
	m_Cam->Position = CIwSVec2(0, 0);
	m_Cam->SetLevelBounds(m_Level->GetLevelBounds());
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

	buttonSoundCount = 0;
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
		if (m_CharacterIndex == MANDY)
		{
			characters[MANDY]->Jump();
		}
		m_SpacePressed = true;
	}

	if( (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN))
    {
		m_ClickLocation = (CIwFVec2((s3ePointerGetX() - (float)m_Cam->GetPosition().x) , (s3ePointerGetY() - (float)m_Cam->GetPosition().y )));
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

		if (n_guiButtons[0]->isColliding((CIwFVec2((float)s3ePointerGetX(), (float)s3ePointerGetY()))))
		{
			if (characters[m_CharacterIndex]->GetDirection() == FACING_RIGHT)
				characters[m_CharacterIndex]->SetDirection(FACING_LEFT);

			characters[m_CharacterIndex]->MoveBy(CIwFVec2((-5 * state->m_deltaTime) - characters[m_CharacterIndex]->GetMovSpeed().x, 0),state->m_deltaTime);
			CheckCollisions(m_CharacterIndex);
		}

		if (n_guiButtons[1]->isColliding((CIwFVec2((float)s3ePointerGetX(), (float)s3ePointerGetY()))))
		{
			if (characters[m_CharacterIndex]->GetDirection() == FACING_LEFT)
				characters[m_CharacterIndex]->SetDirection(FACING_RIGHT);

			characters[m_CharacterIndex]->MoveBy(CIwFVec2((5 * state->m_deltaTime) + characters[m_CharacterIndex]->GetMovSpeed().x, 0),state->m_deltaTime);
			CheckCollisions(m_CharacterIndex);
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

	if (m_isThrowing)

	{
		characters[NIGEL]->LerpTo(CIwFVec2(m_throwingTarget->GetPosition().x, m_throwingTarget->GetPosition().y), 0.05f);
		characters[NIGEL]->UpdateCollider();
		//characters[NIGEL]->MoveBy(CIwFVec2(0,0),0);
		//hack
		if (characters[NIGEL]->isColliding(m_throwingTarget))
		{
			m_canThrow = false;
			m_isThrowing = false;
		}
	}

	if (m_gameOver)
		state->ChangeState(GameoverState::Instance());
}

void GameplayState::Update(StateEngine* state, double dt)
{
	characters[DAVE]->Update(dt);
	characters[MANDY]->Update(dt);
	if (!m_isThrowing)
		characters[NIGEL]->Update(dt);

	CheckInterations(state);

	for (int i = 0; i <3; i++)
	{
		CheckObjects(i);
		CheckCollisions(i);
		
	}

	m_Cam->SetPosition(
		CIwSVec2(static_cast<int16>(-characters[m_CharacterIndex]->GetPosition().x + (screenWidth /2)),
		static_cast<int16>(-characters[m_CharacterIndex]->GetPosition().y + (screenHeight - characters[m_CharacterIndex]->GetHeight() - 32))));

	if (s3eKeyboardGetState(s3eKeySpace) == 4)
		m_SpacePressed = false;

	if (s3ePointerGetState(S3E_POINTER_BUTTON_LEFTMOUSE) == 4)
	{
		m_MouseClicked = false;
		m_ClickLocation = CIwFVec2(0,0);
	}
}

void GameplayState::CheckCollisions(const int &pCharacter)
{
	characters[pCharacter]->UpdateCollider();
	for (size_t m = 0; m < m_Level->GetMap().size(); m++)
	{
		GameObject *t = m_Level->GetMap().at(m);
		if (characters[pCharacter]->isColliding(t))
		{
			characters[pCharacter]->SetPosition(characters[pCharacter]->GetLastPosition());
			if (pCharacter == NIGEL)
			{
				if (m_isThrowing)
				{
					m_canThrow = false;
					m_isThrowing = false;
				}
			}
		}
	}
}

void GameplayState::CheckObjects(const int &pCharacter)
{
	for (size_t m = 0; m < m_Level->GetObjects().size(); m++)
	{
		GameObject *t = m_Level->GetObjects().at(m);
		if (characters[pCharacter]->isColliding(t))
		{
			if (characters[pCharacter]->isColliding(t))
			{
				if (t->GetType() == Door)
				{
					if (t->IsActive)
					{
						if (doorSoundInst == NULL)
						{
							doorSoundInst = doorSound->Play();
						}
						else
						{
							if (!doorSoundInst->IsPlaying())
								doorSound->Play();
						}

						characters[pCharacter]->MoveBy(-characters[pCharacter]->GetLastMovement(),0);
					}
				}
			}
		}
	}
}

void GameplayState::CheckInterations(StateEngine* state)
{
	int count = 0;
	int eCount = 0;
	int exitCount = 0;

	int debug_ele = 0;

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
						buttonSound->Play();
					}

					if (t->Child()->GetType() == Elevator)
					{
						for (int s = 0; s < 3; s++)
						{
							if (s != i)
							{
								if (characters[s]->isColliding(t->Child()))
									characters[s]->MoveBy(CIwFVec2(0, -4),0);
								eCount++;
							}
						}

						std::cout << "Position was "; t->Child()->Debug_PrintPos();
						t->Child()->DoAbility(CIwFVec2(0,characters[i]->GetBottom()),state->m_deltaTime);
						std::cout << "Position is "; t->Child()->Debug_PrintPos();
					}
				}
				else
				{
					if (count == 0)
					{
						t->Child()->IsActive = true;
					}

					if (eCount == 0)
						if (t->Child()->GetType() == Elevator)
							t->Child()->DoAbility(CIwFVec2(0,0),state->m_deltaTime);
				}
			}
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

		// move the elevator effected by a terminal
		if (t->GetType() == Terminal)
		{
			if (m_CharacterIndex == MANDY)
			{
				if (characters[MANDY]->isColliding(t))
				{
					//CIwFVec2 fag = CIwFVec2((s3ePointerGetX() - (float)m_Cam->GetPosition().x)  - ( characters[MANDY]->GetWidth() /2) , (s3ePointerGetY() - (float)m_Cam->GetPosition().y ) - (characters[MANDY]->GetHeight() /2))
					if (characters[MANDY]->isColliding(m_ClickLocation))
					{
						terminalSound->Play();

						if (t->Child()->GetType() == Elevator)
						{
							std::cout << "Elevator was " << t->IsActive << std::endl;
							if (t->IsActive == false)
							{

								t->Child()->SetTarget(CIwFVec2(characters[MANDY]->GetPosition().x, characters[MANDY]->GetBottom()));
								t->IsActive = true;
							}
							else
							{
								t->Child()->SetTarget(CIwFVec2(0,0));
								t->IsActive = false;
							}
							std::cout << "Elevator is now " << t->IsActive << std::endl;
						}
					}
				}
			}

			if (t->Child()->GetType() == Elevator)
			{
				for (int s = 0; s < 3; s++)
				{
					if (characters[s]->isColliding(t->Child()))
						characters[s]->MoveBy(CIwFVec2(0, -4),0);
				}
				t->Child()->DoAbility(t->Child()->GetTarget(), state->m_deltaTime);
			}

			m_MouseClicked = false;
			m_ClickLocation = CIwFVec2(0,0);
		}
	}
}


void GameplayState::Draw(StateEngine* state)
{

	for (int i = 0; i < 4; i++)
	{
		m_Layers.at(i)->Draw(m_Cam->GetPosition());
	}
	m_Level->Draw(state->m_deltaTime);

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

void GameplayState::ScrollBackground(CIwFVec2& scrollBy)
{
	for (int i = 0; i < m_Layers.size(); i++)
	{
		m_Layers.at(i)->MoveBy(
			CIwFVec2(scrollBy.x - (i * 1.5), (0 * i) - scrollBy.y), 0);
	}
}