#ifndef UI_H_
#define UI_H_

#include <vector>
#include "sprite.h"
#include "camera.h"

class UI
{
public:
	inline void AddElment(Sprite* s) {m_UIElements.push_back(s);}
	void Draw();
	bool isClicked(float, float);
protected:

private:
	std::vector<Sprite*> m_UIElements;
};

#endif