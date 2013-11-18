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
#include "GameoverState.h"


StateEngine engine;

int main()
{
    // create game object
	engine.Init("Game");
	engine.ChangeState(IntroState::Instance());
    
	while(engine.Running())
	{
		engine.HandleEvent();
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
