#include "stdafx.h"
#include "UI.h"
#include "UIScene_MinigamesMenu.h"
#include "UIStructs.h"
#include "UIScene_MinigamesCreateMenu.h"
#include "UIScene_MinigamesJoinMenu.h"
// MiniGamesMenu By aRockefeler o Rockefort 
namespace
{
	// these scenes are the two main destinations for the minigames menus
	constexpr EUIScene kMiniGamesCreateScene = eUIScene_MinigamesCreateMenu;
	constexpr EUIScene kMiniGamesJoinScene = eUIScene_MinigamesJoinMenu;
}

UIScene_MinigamesMenu::UIScene_MinigamesMenu(int iPad, void *initData, UILayer *parentLayer)
	: UIScene(iPad, parentLayer)
	, m_isApplyingMiniGamesTab(false)
	, m_removedFooterLabel(false)
	, m_activeTab(eTab_Create)
	, m_selectedCreateIndex(0)
	, m_selectedJoinIndex(0)
	, m_selectedFriendsIndex(0)
{
	app.DebugPrintf("[MGDBG] UIScene_MinigamesMenu ctor pad=%d\n", iPad); // for debugging
	initialiseMovie();
	ApplyMiniGamesLabels();
}
// here the code ask for the swf
std::wstring UIScene_MinigamesMenu::getMoviePath()
{
	app.DebugPrintf("[MGDBG] UIScene_MinigamesMenu getMoviePath -> MiniGamesMenu\n");// and this is for debugging too
	return L"MiniGamesMenu";
}

void UIScene_MinigamesMenu::ApplyMiniGamesLabels()
{
	m_labelCreateTitle.init(IDS_CREATE);
	m_labelJoinTitle.init(IDS_JOIN);
	m_labelFriendsTitle.init(IDS_FRIENDS);
}

int UIScene_MinigamesMenu::GetMovieTabIndex() const
{
	switch (m_activeTab)
	{
	case eTab_Create:
		return 0;
	case eTab_Join:
		return 1;
	case eTab_Friends:
	default:
		return 2;
	}
}

int UIScene_MinigamesMenu::ClampSelectionIndex(int index, int itemCount) const
{
	if (itemCount <= 0)
		return 0;
	if (index < 0)
		return 0;
	if (index >= itemCount)
		return itemCount - 1;
	return index;
}

void UIScene_MinigamesMenu::RefreshMiniGamesContent()
{
	ApplyMiniGamesLabels();

	// reloads the visible menu content 
	auto applyMenuIcon = [this](UIControl_SaveList& list, int itemIndex, const wchar_t* textureName, const wchar_t* archivePath)
	{
		byteArray iconBytes = app.getArchiveFile(archivePath);
		if (iconBytes.data && iconBytes.length > 0)
		{
			registerSubstitutionTexture(textureName, iconBytes.data, iconBytes.length, true);
			list.setTextureName(itemIndex, textureName);
		}
	};

	auto populateMiniGamesList = [this, &applyMenuIcon](UIControl_SaveList& list, int& selectedIndex)
	{
		if (list.getIggyValuePath() == nullptr)
			return;

		// when entering battle this code ask for the icon
		// then gives it a name like mg_battle exacttly as it says there
		// and then applies it to the menu
		list.clearList();
		list.addItem(L"Battle");
		applyMenuIcon(list, list.getItemCount() - 1, L"mg_battle", L"Graphics\\BattleModeIcon.png");
		selectedIndex = ClampSelectionIndex(selectedIndex, list.getItemCount());
		list.setCurrentSelection(selectedIndex);
	};

	populateMiniGamesList(m_buttonListCreate, m_selectedCreateIndex);
	populateMiniGamesList(m_buttonListJoin, m_selectedJoinIndex);

	if (m_buttonListFriends.getIggyValuePath() != nullptr)
	{
		m_buttonListFriends.clearList();
		m_buttonListFriends.addItem(L"Work in progress");
		m_selectedFriendsIndex = ClampSelectionIndex(m_selectedFriendsIndex, m_buttonListFriends.getItemCount());
		m_buttonListFriends.setCurrentSelection(m_selectedFriendsIndex);
	}
}

int UIScene_MinigamesMenu::GetSelectedMiniGameType()
{
	switch (m_activeTab)
	{
	case eTab_Create:
		return ClampSelectionIndex(m_selectedCreateIndex, m_buttonListCreate.getItemCount());
	case eTab_Join:
		return ClampSelectionIndex(m_selectedJoinIndex, m_buttonListJoin.getItemCount());
	default:
		return -1;
	}
}
// before opening battle this checks which tab is active
// create opens minigamescreatemenu
// while join opens minigamesjoinmenu
void UIScene_MinigamesMenu::NavigateToSelectedMiniGame()
{
	if (m_activeTab != eTab_Create && m_activeTab != eTab_Join)
		return;
 
	const int miniGameType = GetSelectedMiniGameType();
	// for now the menu only has battle
	// i removed tumble and glide
	// perhaps you can add them back
	if (miniGameType != eMiniGame_Battle)
		return;

	MiniGamesCreateMenuInitData *params = new MiniGamesCreateMenuInitData();
	params->iPad = m_iPad;
	params->miniGameType = miniGameType;
	if (m_activeTab == eTab_Create)
	{
		UIScene_MinigamesCreateMenu::SetPendingMiniGameType(miniGameType);
		ui.NavigateToScene(m_iPad, kMiniGamesCreateScene, params);
	}
	else
	{
		UIScene_MinigamesJoinMenu::SetPendingMiniGameType(miniGameType);
		ui.NavigateToScene(m_iPad, kMiniGamesJoinScene, params);
	}
}

void UIScene_MinigamesMenu::RemoveFooterLabel()
{
	if (!m_removedFooterLabel)
	{

		if (m_funcShowSaveSizeBar != 0)//this is used to disable the save size bar
		{
			IggyDataValue result;
			IggyDataValue value[1];
			value[0].type = IGGY_DATATYPE_boolean;
			value[0].boolval = false;
			IggyPlayerCallMethodRS(getMovie(), &result, IggyPlayerRootPath(getMovie()), m_funcShowSaveSizeBar, 1, value);
		}

		if (m_labelNoGames.getIggyValuePath() != nullptr)
		{
			m_labelNoGames.setLabel(L"");
			m_labelNoGames.setVisible(false);
			removeControl(&m_labelNoGames, true);
		}

		if (m_controlCreateTimer.getIggyValuePath() != nullptr)
		{
			m_controlCreateTimer.setVisible(false);
			m_controlCreateTimer.setHidden(true);
		}

		if (m_controlJoinTimer.getIggyValuePath() != nullptr)
		{
			m_controlJoinTimer.setVisible(false);
			m_controlJoinTimer.setHidden(true);
		}

		if (m_controlFriendsTimer.getIggyValuePath() != nullptr)
		{
			m_controlFriendsTimer.setVisible(false);
			m_controlFriendsTimer.setHidden(true);
		}

		if (m_controlCreateFooter.getIggyValuePath() != nullptr)
		{
			m_controlCreateFooter.setVisible(false);
			m_controlCreateFooter.setHidden(true);
		}

		m_removedFooterLabel = true;
	}
}

void UIScene_MinigamesMenu::ApplyMiniGamesCurrentTab(bool setFocus)
{
	if (m_isApplyingMiniGamesTab)
		return;

	m_isApplyingMiniGamesTab = true;

	// this is used to align the swf and the code
	ApplyMiniGamesLabels();
	RefreshMiniGamesContent();

	if (m_funcSetActiveTab != 0)
	{
		IggyDataValue result;
		IggyDataValue value[1];
		value[0].type = IGGY_DATATYPE_number;
		value[0].number = static_cast<F64>(GetMovieTabIndex());
		IggyPlayerCallMethodRS(getMovie(), &result, IggyPlayerRootPath(getMovie()), m_funcSetActiveTab, 1, value);
	}

	if (m_controlCreateTimer.getIggyValuePath() != nullptr) { m_controlCreateTimer.setVisible(false); m_controlCreateTimer.setHidden(true); }
	if (m_controlJoinTimer.getIggyValuePath() != nullptr) { m_controlJoinTimer.setVisible(false); m_controlJoinTimer.setHidden(true); }
	if (m_controlFriendsTimer.getIggyValuePath() != nullptr) { m_controlFriendsTimer.setVisible(false); m_controlFriendsTimer.setHidden(true); }
	if (m_controlCreateFooter.getIggyValuePath() != nullptr) { m_controlCreateFooter.setVisible(false); m_controlCreateFooter.setHidden(true); }
	if (m_labelNoGames.getIggyValuePath() != nullptr) { m_labelNoGames.setLabel(L""); m_labelNoGames.setVisible(false); }

	if (setFocus)
	{
		switch (m_activeTab)
		{
		case eTab_Create:
			if (m_buttonListCreate.getIggyValuePath() != nullptr)
				SetFocusToElement(eControl_CreateList);
			break;
		case eTab_Join:
			if (m_buttonListJoin.getIggyValuePath() != nullptr)
				SetFocusToElement(eControl_JoinList);
			break;
		case eTab_Friends:
			if (m_buttonListFriends.getIggyValuePath() != nullptr)
				SetFocusToElement(eControl_FriendsList);
			break;
		}
	}

	RemoveFooterLabel();
	m_isApplyingMiniGamesTab = false;
}

void UIScene_MinigamesMenu::SetMiniGamesTab(int tab, bool setFocus)
{
	if (tab < eTab_Create)
		tab = eTab_Friends;
	else if (tab > eTab_Friends)
		tab = eTab_Create;

	m_activeTab = tab;
	ApplyMiniGamesCurrentTab(setFocus);
	updateTooltips();
}

void UIScene_MinigamesMenu::updateTooltips()
{
	ui.SetTooltips(m_iPad, IDS_TOOLTIPS_SELECT, IDS_TOOLTIPS_BACK, -1, -1, -1, -1, -1, -1, IDS_TOOLTIPS_NAVIGATE);
}

void UIScene_MinigamesMenu::handleGainFocus(bool navBack)
{
	app.DebugPrintf("[MGDBG] UIScene_MinigamesMenu handleGainFocus navBack=%d\n", navBack ? 1 : 0); // holy debugging!
	UIScene::handleGainFocus(navBack);
	m_parentLayer->showComponent(m_iPad, eUIComponent_Logo, false);
	// if we return from minigamesjoin we keep the active tab
	// that didn't happen before and it would go back to create
	// that happend because i wasn't keeping the tab active
	// now is fixed
	SetMiniGamesTab(navBack ? m_activeTab : eTab_Create, true);
	RemoveFooterLabel();
}

void UIScene_MinigamesMenu::handleLoseFocus()
{
	m_parentLayer->showComponent(m_iPad, eUIComponent_Logo, true);
}

void UIScene_MinigamesMenu::handleInitFocus(F64 controlId, F64 childId)
{
	SetMiniGamesTab(eTab_Create, true);
	RemoveFooterLabel();
}

void UIScene_MinigamesMenu::handleFocusChange(F64 controlId, F64 childId)
{
	switch (static_cast<int>(controlId))
	{
	case eControl_TabCreate:
		SetMiniGamesTab(eTab_Create, false);
		break;
	case eControl_TabJoin:
		SetMiniGamesTab(eTab_Join, false);
		break;
	case eControl_TabFriends:
		SetMiniGamesTab(eTab_Friends, false);
		break;
	case eControl_CreateList:
		m_selectedCreateIndex = ClampSelectionIndex(static_cast<int>(childId), m_buttonListCreate.getItemCount());
		m_buttonListCreate.updateChildFocus(m_selectedCreateIndex);
		m_buttonListCreate.setCurrentSelection(m_selectedCreateIndex);
		break;
	case eControl_JoinList:
		m_selectedJoinIndex = ClampSelectionIndex(static_cast<int>(childId), m_buttonListJoin.getItemCount());
		m_buttonListJoin.updateChildFocus(m_selectedJoinIndex);
		m_buttonListJoin.setCurrentSelection(m_selectedJoinIndex);
		break;
	case eControl_FriendsList:
		m_selectedFriendsIndex = ClampSelectionIndex(static_cast<int>(childId), m_buttonListFriends.getItemCount());
		m_buttonListFriends.updateChildFocus(m_selectedFriendsIndex);
		m_buttonListFriends.setCurrentSelection(m_selectedFriendsIndex);
		break;
	default:
		break;
	}
}

void UIScene_MinigamesMenu::handlePress(F64 controlId, F64 childId)
{
	switch (static_cast<int>(controlId))
	{
	case eControl_TabCreate:
		SetMiniGamesTab(eTab_Create, true);
		ui.PlayUISFX(eSFX_Focus);
		break;
	case eControl_TabJoin:
		SetMiniGamesTab(eTab_Join, true);
		ui.PlayUISFX(eSFX_Focus);
		break;
	case eControl_TabFriends:
		SetMiniGamesTab(eTab_Friends, true);
		ui.PlayUISFX(eSFX_Focus);
		break;
	case eControl_CreateList:
		m_selectedCreateIndex = ClampSelectionIndex(static_cast<int>(childId), m_buttonListCreate.getItemCount());
		m_buttonListCreate.updateChildFocus(m_selectedCreateIndex);
		m_buttonListCreate.setCurrentSelection(m_selectedCreateIndex);
		ui.PlayUISFX(eSFX_Press);
		NavigateToSelectedMiniGame();
		break;
	case eControl_JoinList:
		m_selectedJoinIndex = ClampSelectionIndex(static_cast<int>(childId), m_buttonListJoin.getItemCount());
		m_buttonListJoin.updateChildFocus(m_selectedJoinIndex);
		m_buttonListJoin.setCurrentSelection(m_selectedJoinIndex);
		ui.PlayUISFX(eSFX_Press);
		NavigateToSelectedMiniGame();
		break;
	case eControl_FriendsList:
		ui.PlayUISFX(eSFX_Press);
		break;
	default:
		break;
	}
}

void UIScene_MinigamesMenu::handleInput(int iPad, int key, bool repeat, bool pressed, bool released, bool &handled)
{
	auto moveMiniGameSelection = [this](int delta)
	{
		if (m_activeTab == eTab_Create && m_buttonListCreate.getItemCount() > 0)
		{
			m_selectedCreateIndex = ClampSelectionIndex(m_selectedCreateIndex + delta, m_buttonListCreate.getItemCount());
			m_buttonListCreate.updateChildFocus(m_selectedCreateIndex);
			m_buttonListCreate.setCurrentSelection(m_selectedCreateIndex);
			return true;
		}

		if (m_activeTab == eTab_Join && m_buttonListJoin.getItemCount() > 0)
		{
			m_selectedJoinIndex = ClampSelectionIndex(m_selectedJoinIndex + delta, m_buttonListJoin.getItemCount());
			m_buttonListJoin.updateChildFocus(m_selectedJoinIndex);
			m_buttonListJoin.setCurrentSelection(m_selectedJoinIndex);
			return true;
		}

		if (m_activeTab == eTab_Friends && m_buttonListFriends.getItemCount() > 0)
		{
			m_selectedFriendsIndex = ClampSelectionIndex(m_selectedFriendsIndex + delta, m_buttonListFriends.getItemCount());
			m_buttonListFriends.updateChildFocus(m_selectedFriendsIndex);
			m_buttonListFriends.setCurrentSelection(m_selectedFriendsIndex);
			return true;
		}

		return false;
	};

	if (pressed)
	{
		if (key == ACTION_MENU_RIGHT_SCROLL)
		{
			SetMiniGamesTab(m_activeTab + 1, true);
			ui.PlayUISFX(eSFX_Focus);
			handled = true;
			return;
		}

		if (key == ACTION_MENU_LEFT_SCROLL)
		{
			SetMiniGamesTab(m_activeTab - 1, true);
			ui.PlayUISFX(eSFX_Focus);
			handled = true;
			return;
		}
	}

	switch (key)
	{
	case ACTION_MENU_OK:
		if (pressed)
		{
			ui.PlayUISFX(eSFX_Press);
			if (m_activeTab == eTab_Create)
			{
				m_selectedCreateIndex = ClampSelectionIndex(m_selectedCreateIndex, m_buttonListCreate.getItemCount());
				m_buttonListCreate.updateChildFocus(m_selectedCreateIndex);
				m_buttonListCreate.setCurrentSelection(m_selectedCreateIndex);
				NavigateToSelectedMiniGame();
			}
			else if (m_activeTab == eTab_Join)
			{
				m_selectedJoinIndex = ClampSelectionIndex(m_selectedJoinIndex, m_buttonListJoin.getItemCount());
				m_buttonListJoin.updateChildFocus(m_selectedJoinIndex);
				m_buttonListJoin.setCurrentSelection(m_selectedJoinIndex);
				NavigateToSelectedMiniGame();
			}
			handled = true;
			return;
		}
		sendInputToMovie(key, repeat, pressed, released);
		handled = true;
		return;
	case ACTION_MENU_CANCEL:
		if (pressed)
		{
			navigateBack();
			handled = true;
			return;
		}
		break;
	case ACTION_MENU_UP:
		if (pressed && moveMiniGameSelection(-1))
		{
			ui.PlayUISFX(eSFX_Focus);
			handled = true;
			return;
		}
		sendInputToMovie(key, repeat, pressed, released);
		handled = true;
		return;
	case ACTION_MENU_DOWN:
		if (pressed && moveMiniGameSelection(1))
		{
			ui.PlayUISFX(eSFX_Focus);
			handled = true;
			return;
		}
		sendInputToMovie(key, repeat, pressed, released);
		handled = true;
		return;
	case ACTION_MENU_PAGEUP:
	case ACTION_MENU_PAGEDOWN:
		sendInputToMovie(key, repeat, pressed, released);
		handled = true;
		return;
	case ACTION_MENU_OTHER_STICK_UP:
		sendInputToMovie(ACTION_MENU_UP, repeat, pressed, released);
		handled = true;
		return;
	case ACTION_MENU_OTHER_STICK_DOWN:
		sendInputToMovie(ACTION_MENU_DOWN, repeat, pressed, released);
		handled = true;
		return;
	}

	UIScene::handleInput(iPad, key, repeat, pressed, released, handled);
}


