/*
 * This file is part of the Marmalade SDK Code Samples.
 *
 * (C) 2001-2012 Marmalade. All Rights Reserved.
 *
 * This source code is intended only as a supplement to the Marmalade SDK.
 *
 * THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 */
#include "s3e.h"
#include "StateEngine.h"
#include "IntroState.h"
#include "GameplayState.h"
#include "GameoverState.h"

// updates per second
#define UPS 60

// throttle frame time at 10 fps (i.e. the game will slow down rather
// than having very low frame rate)
#define MAX_UPDATES 6

StateEngine engine;

int GetUpdateFrame()
{
    return (int)(s3eTimerGetMs() / (1000/UPS));
}

// "main" is the S3E entry point
int main()
{
    // create game object
	engine.Init("Game");
	engine.ChangeState(GameplayState::Instance());

    int currentUpdate = GetUpdateFrame();
    int nextUpdate = currentUpdate;
    
	while(engine.Running()) // Fairly certain that this will be a problem
	{
		engine.HandleEvent(); // Possibly add s3eDeviceCheckQuitRequest into here
		engine.Update();
		engine.Draw();
		s3ePointerUpdate();
		s3eKeyboardUpdate();

		// S3E applications should yield frequently
		s3eDeviceYield();
	}
    // clear up game object
    engine.Destroy();
    return 0;
}
