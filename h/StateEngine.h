#ifndef _STATEENGINE_H
#define _STATEENGINE_H

#include <vector>

class GameState;

class StateEngine
{
public:
	void Init(const char* title);
	void Destroy();

	void ChangeState(GameState* state);
	void PushState(GameState* state);
	void PopState();

	void HandleEvent();
	void Update();
	void Draw();

	int Running(){return m_isRunning;}
	void SetActive(int pRunning){m_isRunning = pRunning;}
	void Quit() {m_isRunning = false;}

protected:

private:

	// The states on the stack
	std::vector<GameState*> m_States;

	int m_isRunning;
	double m_CurrTime, m_LastTime;
};
#endif