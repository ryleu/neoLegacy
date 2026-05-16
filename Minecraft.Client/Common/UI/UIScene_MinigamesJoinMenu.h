#pragma once

#include "IUIScene_StartGame.h"

class UIScene_MinigamesJoinMenu : public IUIScene_StartGame
{
private:
	static constexpr EUIScene kMiniGamesJoinScene = eUIScene_MinigamesJoinMenu;

	enum EControls
	{
		eControl_GameMode,
		eControl_MoreOptions,
		eControl_Join,
	};

	UIControl m_controlMainPanel;
	UIControl_Label m_labelGameName;
	UIControl_Button m_buttonMoreOptions;
	UIControl_Button m_buttonJoin;
	UIControl_Slider m_sliderGamemode;
	UIControl_BitmapIcon m_bitmapIcon;

	UI_BEGIN_MAP_ELEMENTS_AND_NAMES(IUIScene_StartGame)
		UI_MAP_ELEMENT(m_controlMainPanel, "MainPanel")
		UI_BEGIN_MAP_CHILD_ELEMENTS(m_controlMainPanel)
			UI_MAP_ELEMENT(m_labelGameName, "GameName")
			UI_MAP_ELEMENT(m_sliderGamemode, "GameModeToggle")
			UI_MAP_ELEMENT(m_buttonMoreOptions, "MoreOptions")
			UI_MAP_ELEMENT(m_buttonJoin, "LoadSettings")
			UI_MAP_ELEMENT(m_bitmapIcon, "LevelIcon")
		UI_END_MAP_CHILD_ELEMENTS()
	UI_END_MAP_ELEMENTS_AND_NAMES()

	int m_iMiniGameType;
	int m_iJoinModeSelection;

	static int s_pendingMiniGameType;

public:
	UIScene_MinigamesJoinMenu(int iPad, void *initData, UILayer *parentLayer);
	static void SetPendingMiniGameType(int miniGameType);

	virtual void updateTooltips();
	virtual void updateComponents();
	virtual EUIScene getSceneType() { return kMiniGamesJoinScene; }
	virtual void tick();
	virtual UIControl* GetMainPanel();

protected:
	virtual wstring getMoviePath();
	virtual void handleGainFocus(bool navBack);
	virtual bool canMoveSlider(F64 sliderId);
	virtual void checkStateAndStartGame();

public:
	virtual void handleInput(int iPad, int key, bool repeat, bool pressed, bool released, bool &handled);
	virtual void handleTimerComplete(int id);
	virtual void handleFocusChange(F64 controlId, F64 childId);
	virtual void handleSelectionChanged(F64 selectedId);
	virtual void handleTouchBoxRebuild();

private:
	void handlePress(F64 controlId, F64 childId);
	void handleSliderMove(F64 sliderId, F64 currentValue);
	void ApplyMiniGamePresentation();
	void ApplyMiniGameOptionDescription(int controlId);
	void LaunchGame();
};
