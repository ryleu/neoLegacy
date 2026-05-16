#include "stdafx.h"
#include "UI.h"
#include "UIScene_MinigamesCreateMenu.h"
#include "../../Minecraft.h"
// mini gaimes menus By aRockefeler o aRockefort
// string localization by Fireblade
namespace
{
	const wchar_t* GetBattleModeLabel(int mode)
	{
		switch (mode)
		{
		case 1: return app.GetString(IDS_BATTLE_MODE_COMPETITIVE);
		case 2: return app.GetString(IDS_BATTLE_MODE_CUSTOM);
		case 0:
		default:
			return app.GetString(IDS_BATTLE_MODE_CASUAL);
		}
	}
	// there's a bug with the slider that i haven't been able to fix, whe you move the slider
	// the descripcion changes but it's not visible 
	// it will only appear if you change the button
	// sorry if i didn't manage to fix it
	const wchar_t* GetBattleModeDescription(int mode)
	{
		switch (mode)
		{
		case 1:
			return app.GetString(IDS_BATTLE_MODE_COMPETITIVE_DESC);
		case 2:
			return app.GetString(IDS_BATTLE_MODE_CUSTOM_DESC);
		case 0:
		default:
			return app.GetString(IDS_BATTLE_MODE_CASUAL_DESC);
		}
	}
}

int UIScene_MinigamesCreateMenu::s_pendingMiniGameType = eMiniGame_Battle;

void UIScene_MinigamesCreateMenu::SetPendingMiniGameType(int miniGameType)
{
	s_pendingMiniGameType = miniGameType;
}

UIScene_MinigamesCreateMenu::UIScene_MinigamesCreateMenu(int iPad, void *initData, UILayer *parentLayer)
	: IUIScene_StartGame(iPad, parentLayer)
{
	initialiseMovie();

	MiniGamesCreateMenuInitData defaultMiniGamesParams = {};
	MiniGamesCreateMenuInitData *miniGamesParams = initData ? static_cast<MiniGamesCreateMenuInitData *>(initData) : &defaultMiniGamesParams;

	m_iMiniGameType = miniGamesParams->miniGameType;
	m_iBattleModeSelection = 0;
	m_bIgnoreInput = false;
	m_bShowTexturePackDescription = false;
	m_texturePackDescDisplayed = false;
	m_iSetTexturePackDescription = -1;
	m_bMultiplayerAllowed = ProfileManager.IsSignedInLive(m_iPad) && ProfileManager.AllowedToPlayMultiplayer(m_iPad);
	m_bPublicGame = false;

	// although the names say "texturepack" the swf reuses those labels as
    // part of the right panel of the mini game description 
	// that helps me avoid creating new things and reuse what already exist
	m_labelGameName.init(IDS_BATTLE);
	m_labelCreatedMode.init(L"");
	m_labelSeed.init(L"");
	m_sliderGamemode.init(GetBattleModeLabel(m_iBattleModeSelection), eControl_GameMode, 0, 2, m_iBattleModeSelection);
	m_labelTexturePackName.init(L"");
	m_labelTexturePackDescription.init(L"");
	m_checkboxOnline.init(app.GetString(IDS_ONLINE_GAME), eControl_OnlineGame, m_bMultiplayerAllowed);
	m_checkboxPublic.init(app.GetString(IDS_CHECKBOX_PUBLIC_GAME), eControl_PublicGame, false);
	m_checkboxFriendsOfFriends.init(app.GetString(IDS_GAMEOPTION_FRIENDSOFRIENDS), eControl_FriendsOfFriends, false);
	m_checkboxInviteOnly.init(app.GetString(IDS_GAMEOPTION_INVITEONLY), eControl_InviteOnly, false);
	m_buttonSelectMaps.init(app.GetString(IDS_GAMEOPTION_SELECTMAPS), eControl_SelectMaps);
	m_buttonMoreOptions.init(app.GetString(IDS_GAMEOPTION_MOREOPTIONS), eControl_MoreOptions);
	m_buttonCreateGame.init(app.GetString(IDS_GAMEOPTION_CREATEGAME), eControl_CreateGame);

	m_MoreOptionsParams = LaunchMoreOptionsMenuInitData();
	m_MoreOptionsParams.iPad = iPad;
	m_MoreOptionsParams.bGenerateOptions = FALSE;
	m_MoreOptionsParams.bPVP = TRUE;
	m_MoreOptionsParams.bTrust = TRUE;
	m_MoreOptionsParams.bFireSpreads = TRUE;
	m_MoreOptionsParams.bTNT = TRUE;
	m_MoreOptionsParams.bHostPrivileges = FALSE;
	m_MoreOptionsParams.bOnlineGame = m_bMultiplayerAllowed ? TRUE : FALSE;
	m_MoreOptionsParams.bInviteOnly = FALSE;
	m_MoreOptionsParams.bAllowFriendsOfFriends = FALSE;

	ApplyMiniGamePresentation();
	updateTooltips();
}

// this is where the main visual and functional part of minigamescreate
// is built
// labels, controls, icons, the image on the right panel etc 
void UIScene_MinigamesCreateMenu::ApplyMiniGamePresentation()
{
	const wchar_t* miniGameName = L"Battle";
	const wchar_t* miniGameIconTexture = L"mg_battle_create";
	const wchar_t* miniGameIconArchive = L"Graphics\\BattleModeIcon.png";
	const wchar_t* miniGameScreenTexture = L"mg_battle_screen";
	const wchar_t* miniGameScreenArchive = L"Graphics\\GameModeGraphics\\BattleModeScreen.png";

	m_labelGameName.setLabel(miniGameName, true, true);
	m_labelCreatedMode.setLabel(L"", true, true);
	m_labelSeed.setLabel(L"", true, true);
	m_sliderGamemode.setLabel(GetBattleModeLabel(m_iBattleModeSelection), true, true);
	m_buttonSelectMaps.setLabel(app.GetString(IDS_GAMEOPTION_SELECTMAPS), true, true);
	m_buttonMoreOptions.setLabel(app.GetString(IDS_GAMEOPTION_MOREOPTIONS), true, true);
	m_buttonCreateGame.setLabel(app.GetString(IDS_GAMEOPTION_CREATEGAME), true, true);

	m_checkboxOnline.SetEnable(m_bMultiplayerAllowed);
	m_checkboxOnline.setChecked(m_MoreOptionsParams.bOnlineGame == TRUE);
	if (m_checkboxPublic.getIggyValuePath())
	{
		m_checkboxPublic.setChecked(m_bPublicGame);
	}
	if (m_checkboxFriendsOfFriends.getIggyValuePath())
	{
		m_checkboxFriendsOfFriends.setChecked(m_MoreOptionsParams.bAllowFriendsOfFriends == TRUE);
	}
	if (m_checkboxInviteOnly.getIggyValuePath())
	{
		m_checkboxInviteOnly.setChecked(m_MoreOptionsParams.bInviteOnly == TRUE);
	}

	m_sliderDifficulty.setVisible(false);
	m_sliderDifficulty.setHidden(true);
	m_labelTexturePackName.setLabel(L"", true, true);
	m_labelTexturePackName.setVisible(false);
	m_labelTexturePackName.setHidden(true);
	m_labelTexturePackName.UpdateControl();
	m_bitmapTexturePackIcon.setVisible(false);
	m_bitmapTexturePackIcon.setHidden(true);

	byteArray miniGameIcon = app.getArchiveFile(miniGameIconArchive);
	if (miniGameIcon.data && miniGameIcon.length > 0)
	{
		registerSubstitutionTexture(miniGameIconTexture, miniGameIcon.data, miniGameIcon.length, true);
		m_bitmapIcon.setTextureName(miniGameIconTexture);
	}

	byteArray miniGameScreen = app.getArchiveFile(miniGameScreenArchive);
	if (miniGameScreen.data && miniGameScreen.length > 0)
	{
		registerSubstitutionTexture(miniGameScreenTexture, miniGameScreen.data, miniGameScreen.length, true);
		m_bitmapComparison.setTextureName(miniGameScreenTexture);
	}

	ApplyMiniGameOptionDescription(eControl_GameMode);
	m_labelTexturePackDescription.UpdateControl();
}

void UIScene_MinigamesCreateMenu::ApplyMiniGameOptionDescription(int controlId)
{
	// the description on the right depens on the focused control in battle
	// more options and create game inherit the text from the slider's current preset :v

	// fallback
	const wchar_t* description = L"";

	switch (controlId)
	{
	case eControl_OnlineGame:
		description = app.GetString(IDS_GAMEOPTION_ONLINE);
		break;
	case eControl_PublicGame:
		description = app.GetString(IDS_GAMEOPTION_PUBLIC);
		break;
	case eControl_FriendsOfFriends:
		description = app.GetString(IDS_GAMEOPTION_ALLOWFOF);
		break;
	case eControl_InviteOnly:
		description = app.GetString(IDS_GAMEOPTION_INVITEONLY);
		break;
	case eControl_SelectMaps:
		description = app.GetString(IDS_GAMEOPTION_SELECTMAPS);
		break;
	case eControl_GameMode:
		description = GetBattleModeDescription(m_iBattleModeSelection);
		break;
	case eControl_MoreOptions:
	case eControl_CreateGame:
		description = GetBattleModeDescription(m_iBattleModeSelection);
		break;
	default:
		break;
	}

	m_labelTexturePackName.setLabel(L"", true, true);
	m_labelTexturePackName.setVisible(false);
	m_labelTexturePackName.setHidden(true);
	m_labelTexturePackName.UpdateControl();
	m_labelTexturePackDescription.setLabel(description, true, true);
	m_labelTexturePackDescription.setVisible(true);
	m_labelTexturePackDescription.setHidden(false);
	m_labelTexturePackDescription.UpdateControl();
}

void UIScene_MinigamesCreateMenu::SyncBattleCheckboxStates()
{
	// MiniGamesCreate/Join checkboxes are mirrored to moreoptionsparams
	// in short if a checkbox is visible for example "Public Game" then when you go to more options you'll see
	// Public games there as well
	bool onlineGame = m_checkboxOnline.IsChecked();
	m_MoreOptionsParams.bOnlineGame = onlineGame;

	if (m_checkboxPublic.getIggyValuePath())
	{
		m_bPublicGame = onlineGame ? m_checkboxPublic.IsChecked() : false;
	}

	if (onlineGame)
	{
		m_MoreOptionsParams.bAllowFriendsOfFriends = m_checkboxFriendsOfFriends.getIggyValuePath() ? m_checkboxFriendsOfFriends.IsChecked() : false;
		m_MoreOptionsParams.bInviteOnly = m_checkboxInviteOnly.getIggyValuePath() ? m_checkboxInviteOnly.IsChecked() : false;
	}
	else
	{
		m_MoreOptionsParams.bAllowFriendsOfFriends = false;
		m_MoreOptionsParams.bInviteOnly = false;

		if (m_checkboxPublic.getIggyValuePath())
			m_checkboxPublic.setChecked(false);
		if (m_checkboxFriendsOfFriends.getIggyValuePath())
			m_checkboxFriendsOfFriends.setChecked(false);
		if (m_checkboxInviteOnly.getIggyValuePath())
			m_checkboxInviteOnly.setChecked(false);

		m_bPublicGame = false;
	}
}

void UIScene_MinigamesCreateMenu::updateTooltips()
{
	ui.SetTooltips(DEFAULT_XUI_MENU_USER, IDS_TOOLTIPS_SELECT, IDS_TOOLTIPS_BACK, -1, -1);
}

void UIScene_MinigamesCreateMenu::updateComponents()
{
	m_parentLayer->showComponent(m_iPad, eUIComponent_Panorama, true);
	m_parentLayer->showComponent(m_iPad, eUIComponent_Logo, false);

	if (m_iSetTexturePackDescription >= 0)
	{
		ApplyMiniGameOptionDescription(m_iSetTexturePackDescription);
		m_iSetTexturePackDescription = -1;
	}
}

void UIScene_MinigamesCreateMenu::tick()
{
	UIScene::tick();
}

UIControl* UIScene_MinigamesCreateMenu::GetMainPanel()
{
	return &m_controlMainPanel;
}

void UIScene_MinigamesCreateMenu::handleTouchBoxRebuild()
{
}

wstring UIScene_MinigamesCreateMenu::getMoviePath()
{
	return L"MiniGamesCreateMenu";
}

void UIScene_MinigamesCreateMenu::handleInput(int iPad, int key, bool repeat, bool pressed, bool released, bool &handled)
{
	if (m_bIgnoreInput)
		return;

	ui.AnimateKeyPress(m_iPad, key, repeat, pressed, released);

	switch (key)
	{
	case ACTION_MENU_CANCEL:
		if (pressed)
		{
			navigateBack();
			handled = true;
		}
		break;
	case ACTION_MENU_OK:
#ifdef __ORBIS__
	case ACTION_MENU_TOUCHPAD_PRESS:
#endif
	case ACTION_MENU_UP:
	case ACTION_MENU_DOWN:
	case ACTION_MENU_LEFT:
	case ACTION_MENU_RIGHT:
	case ACTION_MENU_OTHER_STICK_UP:
	case ACTION_MENU_OTHER_STICK_DOWN:
		sendInputToMovie(key, repeat, pressed, released);
		SyncBattleCheckboxStates();
		handled = true;
		break;
	}
}

void UIScene_MinigamesCreateMenu::handlePress(F64 controlId, F64 childId)
{
	(void)childId;

	if (m_bIgnoreInput)
		return;

	switch (static_cast<int>(controlId))
	{
	case eControl_MoreOptions:
		ui.NavigateToScene(m_iPad, eUIScene_LaunchMoreOptionsMenu, &m_MoreOptionsParams);
		break;
	case eControl_SelectMaps:
		// this opens the mapselect scene
		ui.NavigateToScene(m_iPad, eUIScene_MinigamesMapSelectMenu, nullptr);
		break;
	case eControl_CreateGame:
		ui.PlayUISFX(eSFX_Press);
		checkStateAndStartGame();
		break;
	case eControl_OnlineGame:
	case eControl_PublicGame:
	case eControl_FriendsOfFriends:
	case eControl_InviteOnly:
		SyncBattleCheckboxStates();
		break;
	}
}

void UIScene_MinigamesCreateMenu::handleSliderMove(F64 sliderId, F64 currentValue)
{
	if (static_cast<int>(sliderId) != eControl_GameMode)
		return;

	int value = static_cast<int>(currentValue);
	m_iBattleModeSelection = value < 0 ? 0 : (value > 2 ? 2 : value);
	m_sliderGamemode.handleSliderMove(m_iBattleModeSelection);
	m_sliderGamemode.setLabel(GetBattleModeLabel(m_iBattleModeSelection));
	ApplyMiniGameOptionDescription(eControl_GameMode);
}

void UIScene_MinigamesCreateMenu::handleGainFocus(bool navBack)
{
	m_iMiniGameType = s_pendingMiniGameType;
	if (navBack)
	{
		SyncBattleCheckboxStates();
	}
	ApplyMiniGamePresentation();
	SetFocusToElement(eControl_GameMode);
}

bool UIScene_MinigamesCreateMenu::canMoveSlider(F64 sliderId)
{
	return static_cast<int>(sliderId) == eControl_GameMode;
}

void UIScene_MinigamesCreateMenu::handleTimerComplete(int id)
{
	(void)id;
}

void UIScene_MinigamesCreateMenu::handleFocusChange(F64 controlId, F64 childId)
{
	(void)childId;
	m_iSetTexturePackDescription = static_cast<int>(controlId);
	ApplyMiniGameOptionDescription(static_cast<int>(controlId));
}

void UIScene_MinigamesCreateMenu::handleSelectionChanged(F64 selectedId)
{
	m_iSetTexturePackDescription = static_cast<int>(selectedId);
	ApplyMiniGameOptionDescription(static_cast<int>(selectedId));
}

void UIScene_MinigamesCreateMenu::checkStateAndStartGame()
{
	LaunchGame();
}

void UIScene_MinigamesCreateMenu::LaunchGame()
{
	// this is for debugging 
	// if the menu is already complete you can remove this
	app.DebugPrintf("[MGDBG] MiniGamesCreateMenu Create pressed for type=%d mode=%d online=%d public=%d friends=%d invite=%d\n",
		m_iMiniGameType,
		m_iBattleModeSelection,
		m_MoreOptionsParams.bOnlineGame ? 1 : 0,
		m_bPublicGame ? 1 : 0,
		m_MoreOptionsParams.bAllowFriendsOfFriends ? 1 : 0,
		m_MoreOptionsParams.bInviteOnly ? 1 : 0);
}
