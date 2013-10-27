#include "UI.h"

void UI::Draw()
{
	for (auto it = m_UIElements.begin(); it != m_UIElements.end(); ++it)
	{
		(*it)->Draw();
	}
}