#include "stdafx.h"
#include "UI.h"
#include "UIScene_MinigamesMapSelectMenu.h"
// Mini Games Map select are incomplete
// maybe you can finish it
// sorry i didn't finish it, if you want me to finish it let me know on discord
UIScene_MinigamesMapSelectMenu::UIScene_MinigamesMapSelectMenu(int iPad, void *initData, UILayer *parentLayer)
	: UIScene(iPad, parentLayer)
{

	initialiseMovie();
	updateTooltips();
}

void UIScene_MinigamesMapSelectMenu::updateTooltips()
{
	ui.SetTooltips(DEFAULT_XUI_MENU_USER, IDS_TOOLTIPS_SELECT, IDS_TOOLTIPS_BACK, -1, -1);
}

void UIScene_MinigamesMapSelectMenu::updateComponents()
{
	
	m_parentLayer->showComponent(m_iPad, eUIComponent_Panorama, true);
	m_parentLayer->showComponent(m_iPad, eUIComponent_Logo, false);
}

UIControl* UIScene_MinigamesMapSelectMenu::GetMainPanel()
{
	return &m_controlMainPanel;
}

wstring UIScene_MinigamesMapSelectMenu::getMoviePath()
{
	return L"MapSelectMenu";
}

void UIScene_MinigamesMapSelectMenu::handleInput(int iPad, int key, bool repeat, bool pressed, bool released, bool &handled)
{
	
	if (key == ACTION_MENU_CANCEL && pressed)
	{
		navigateBack();
		handled = true;
		return;
	}

	sendInputToMovie(key, repeat, pressed, released);
	handled = true;
}
