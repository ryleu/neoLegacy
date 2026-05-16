#pragma once

#include "UIEnums.h"
#include "UIStructs.h"
#include "IUIScene_StartGame.h"

class UIScene_MinigamesCreateMenu : public IUIScene_StartGame
{
private:
	static constexpr EUIScene kMiniGamesCreateScene = eUIScene_MinigamesCreateMenu;

	enum EControls
	{
		eControl_GameMode,
		eControl_Difficulty,
		eControl_MoreOptions,
		eControl_CreateGame,
		eControl_OnlineGame,
		eControl_PublicGame,
		eControl_FriendsOfFriends,
		eControl_InviteOnly,
		eControl_SelectMaps,
	};

	UIControl m_controlMainPanel;
	UIControl_Label m_labelGameName;
	UIControl_Label m_labelSeed;
	UIControl_Label m_labelCreatedMode;
	UIControl_Button m_buttonSelectMaps;
	UIControl_Button m_buttonMoreOptions;
	UIControl_Button m_buttonCreateGame;
	UIControl_Slider m_sliderDifficulty;
	UIControl_Slider m_sliderGamemode;
	UIControl_BitmapIcon m_bitmapIcon;
	UIControl_CheckBox m_checkboxOnline;
	UIControl_CheckBox m_checkboxPublic;
	UIControl_CheckBox m_checkboxFriendsOfFriends;
	UIControl_CheckBox m_checkboxInviteOnly;

	UI_BEGIN_MAP_ELEMENTS_AND_NAMES(IUIScene_StartGame)
		UI_MAP_ELEMENT(m_controlMainPanel, "MainPanel")
		UI_BEGIN_MAP_CHILD_ELEMENTS(m_controlMainPanel)
			UI_MAP_ELEMENT(m_labelGameName, "GameName")
			UI_MAP_ELEMENT(m_labelCreatedMode, "CreatedMode")
			UI_MAP_ELEMENT(m_labelSeed, "Seed")
			UI_MAP_ELEMENT(m_sliderGamemode, "GameModeToggle")
			UI_MAP_ELEMENT(m_checkboxOnline, "CheckboxOnline")
			UI_MAP_ELEMENT(m_checkboxPublic, "CheckboxPublic")
			UI_MAP_ELEMENT(m_checkboxFriendsOfFriends, "CheckboxFriendsOfFriends")
			UI_MAP_ELEMENT(m_checkboxInviteOnly, "CheckboxInviteOnly")
			UI_MAP_ELEMENT(m_buttonSelectMaps, "SelectMaps")
			UI_MAP_ELEMENT(m_buttonMoreOptions, "MoreOptions")
			UI_MAP_ELEMENT(m_buttonCreateGame, "LoadSettings")
			UI_MAP_ELEMENT(m_sliderDifficulty, "Difficulty")
			UI_MAP_ELEMENT(m_bitmapIcon, "LevelIcon")
		UI_END_MAP_CHILD_ELEMENTS()
	UI_END_MAP_ELEMENTS_AND_NAMES()

	int m_iMiniGameType;
	int m_iBattleModeSelection;
	bool m_bMultiplayerAllowed;
	bool m_bPublicGame;

	static int s_pendingMiniGameType;

public:
	UIScene_MinigamesCreateMenu(int iPad, void *initData, UILayer *parentLayer);
	static void SetPendingMiniGameType(int miniGameType);

	virtual void updateTooltips();
	virtual void updateComponents();
	virtual EUIScene getSceneType() { return kMiniGamesCreateScene; }
	virtual void tick();
	virtual UIControl* GetMainPanel();
	virtual void handleTouchBoxRebuild();

protected:
	virtual wstring getMoviePath();

public:
	virtual void handleInput(int iPad, int key, bool repeat, bool pressed, bool released, bool &handled);
	virtual void handleTimerComplete(int id);
	virtual void handleFocusChange(F64 controlId, F64 childId);
	virtual void handleSelectionChanged(F64 selectedId);

protected:
	void handlePress(F64 controlId, F64 childId);
	void handleSliderMove(F64 sliderId, F64 currentValue);
	virtual void handleGainFocus(bool navBack);
	virtual bool canMoveSlider(F64 sliderId);

private:
	void ApplyMiniGamePresentation();
	void ApplyMiniGameOptionDescription(int controlId);
	void SyncBattleCheckboxStates();
	virtual void checkStateAndStartGame();
	void LaunchGame();
};
