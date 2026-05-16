#pragma once

#include "UIEnums.h"
#include "UIScene.h"

class UIScene_MinigamesMapSelectMenu : public UIScene
{
private:
	UIControl m_controlMainPanel;

	UI_BEGIN_MAP_ELEMENTS_AND_NAMES(UIScene)
		UI_MAP_ELEMENT(m_controlMainPanel, "MainPanel")
	UI_END_MAP_ELEMENTS_AND_NAMES()

public:
	UIScene_MinigamesMapSelectMenu(int iPad, void *initData, UILayer *parentLayer);

	virtual EUIScene getSceneType() { return ::eUIScene_MinigamesMapSelectMenu; }
	virtual bool hidesLowerScenes() { return true; }

	virtual void updateTooltips();
	virtual void updateComponents();
	virtual UIControl* GetMainPanel();
	virtual void handleInput(int iPad, int key, bool repeat, bool pressed, bool released, bool &handled);

protected:
	virtual wstring getMoviePath();
};
