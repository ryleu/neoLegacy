#pragma once

#include "UIScene.h"
#include "UIStructs.h"

class UIScene_MinigamesMenu : public UIScene
{
private:

	enum EControls
	{
		eControl_CreateList,
		eControl_JoinList,
		eControl_FriendsList,
		eControl_TabCreate,
		eControl_TabJoin,
		eControl_TabFriends,
	};

	enum EMiniGamesTabIndex
	{
		eTab_Create = 0,
		eTab_Join,
		eTab_Friends,
	};

	UIControl m_controlCreateRoot;
	UIControl m_controlCreatePanel;
	UIControl m_controlCreateFooter;
	UIControl m_controlJoinRoot;
	UIControl m_controlJoinPanel;
	UIControl m_controlFriendsRoot;
	UIControl m_controlFriendsPanel;
	UIControl_SaveList m_buttonListCreate;
	UIControl_SaveList m_buttonListJoin;
	UIControl_SaveList m_buttonListFriends;
	UIControl_Button m_tabCreate;
	UIControl_Button m_tabJoin;
	UIControl_Button m_tabFriends;
	UIControl_Label m_labelCreateTitle;
	UIControl_Label m_labelJoinTitle;
	UIControl_Label m_labelFriendsTitle;
	UIControl_HTMLLabel m_labelNoGames;
	UIControl m_controlCreateTimer;
	UIControl m_controlJoinTimer;
	UIControl m_controlFriendsTimer;
	UIControl m_controlMainPanel;

	IggyName m_funcSetActiveTab;
	IggyName m_funcShowSaveSizeBar;
	bool m_isApplyingMiniGamesTab;
	bool m_removedFooterLabel;
	int m_activeTab;
	int m_selectedCreateIndex;
	int m_selectedJoinIndex;
	int m_selectedFriendsIndex;

	UI_BEGIN_MAP_ELEMENTS_AND_NAMES(UIScene)
		UI_MAP_ELEMENT( m_controlMainPanel, "MainPanel")
		UI_MAP_ELEMENT( m_controlCreateRoot, "LoadGame")
		UI_BEGIN_MAP_CHILD_ELEMENTS( m_controlCreateRoot )
			UI_MAP_ELEMENT( m_controlCreatePanel, "LoadGameListRecessPanel")
			UI_MAP_ELEMENT( m_buttonListCreate, "LoadGameList")
			UI_MAP_ELEMENT( m_controlCreateTimer, "LoadGameTimer")
			UI_MAP_ELEMENT( m_controlCreateFooter, "SaveSizeBar")
			UI_MAP_ELEMENT( m_tabCreate, "TouchTabLoad")
		UI_END_MAP_CHILD_ELEMENTS()

		UI_MAP_ELEMENT( m_controlJoinRoot, "NewGame")
		UI_BEGIN_MAP_CHILD_ELEMENTS( m_controlJoinRoot )
			UI_MAP_ELEMENT( m_controlJoinPanel, "NewGameListRecessPanel")
			UI_MAP_ELEMENT( m_buttonListJoin, "NewGameList")
			UI_MAP_ELEMENT( m_controlJoinTimer, "NewGameTimer")
			UI_MAP_ELEMENT( m_tabJoin, "TouchTabCreate")
		UI_END_MAP_CHILD_ELEMENTS()

		UI_MAP_ELEMENT( m_controlFriendsRoot, "JoinGame")
		UI_BEGIN_MAP_CHILD_ELEMENTS( m_controlFriendsRoot )
			UI_MAP_ELEMENT( m_controlFriendsPanel, "JoinListPanel")
			UI_MAP_ELEMENT( m_buttonListFriends, "JoinGameGamesList")
			UI_MAP_ELEMENT( m_controlFriendsTimer, "JoinGamesTimer")
			UI_MAP_ELEMENT( m_tabFriends, "TouchTabJoin")
		UI_END_MAP_CHILD_ELEMENTS()

		UI_MAP_ELEMENT( m_labelCreateTitle, "LoadGameTabTitle")
		UI_MAP_ELEMENT( m_labelJoinTitle, "NewGameTabTitle")
		UI_MAP_ELEMENT( m_labelFriendsTitle, "JoinGameTabTitle")
		UI_MAP_ELEMENT( m_labelNoGames, "NoGames")
		UI_MAP_NAME( m_funcSetActiveTab, L"SetActiveTab")
		UI_MAP_NAME( m_funcShowSaveSizeBar, L"ShowSaveSizeBar")
	UI_END_MAP_ELEMENTS_AND_NAMES()

	void RefreshMiniGamesContent();
	void ApplyMiniGamesLabels();
	void ApplyMiniGamesCurrentTab(bool setFocus);
	int GetMovieTabIndex() const;
	void SetMiniGamesTab(int tab, bool setFocus);
	void RemoveFooterLabel();
	int GetSelectedMiniGameType();
	void NavigateToSelectedMiniGame();
	int ClampSelectionIndex(int index, int itemCount) const;

public:
	UIScene_MinigamesMenu(int iPad, void *initData, UILayer *parentLayer);
	virtual EUIScene getSceneType() { return eUIScene_MinigamesMenu; }
	virtual std::wstring getMoviePath();
	virtual void updateTooltips();
	virtual void handleGainFocus(bool navBack);
	virtual void handleLoseFocus();
	virtual void handleInitFocus(F64 controlId, F64 childId);
	virtual void handleFocusChange(F64 controlId, F64 childId);
	virtual void handleInput(int iPad, int key, bool repeat, bool pressed, bool released, bool &handled);
	virtual UIControl* GetMainPanel() { return &m_controlMainPanel; }

protected:
	void handlePress(F64 controlId, F64 childId);
};



