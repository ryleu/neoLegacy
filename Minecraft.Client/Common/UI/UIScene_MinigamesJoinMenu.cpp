#include "stdafx.h"
#include "UI.h"
#include "UIScene_MinigamesJoinMenu.h"
#include "../../Minecraft.h"
// mini games menus By aRockefeler o aRockefort
namespace
{
	// Join filtra partidas por preset, en vez de configurar reglas completas como Create.
	const wchar_t* GetJoinModeLabel(int mode)
	{
		switch (mode)
		{
		case 1: return app.GetString(IDS_BATTLE_MODE_CASUAL);
		case 2: return app.GetString(IDS_BATTLE_MODE_COMPETITIVE);
		case 3: return app.GetString(IDS_BATTLE_MODE_CUSTOM);
		case 0:
		default:
			return app.GetString(IDS_BATTLE_MODE_ANY);
		}
	}

	const wchar_t* GetJoinModeDescription(int mode)
	{
		switch (mode)
		{
		case 1:
			return app.GetString(IDS_BATTLE_JOIN_CASUAL_DESC);
		case 2:
			return app.GetString(IDS_BATTLE_JOIN_COMPETITIVE_DESC);
		case 3:
			return app.GetString(IDS_BATTLE_JOIN_CUSTOM_DESC);
		case 0:
		default:
			return app.GetString(IDS_BATTLE_JOIN_ANY_DESC);
		}
	}
}

int UIScene_MinigamesJoinMenu::s_pendingMiniGameType = eMiniGame_Battle;

void UIScene_MinigamesJoinMenu::SetPendingMiniGameType(int miniGameType)
{
	s_pendingMiniGameType = miniGameType;
}

UIScene_MinigamesJoinMenu::UIScene_MinigamesJoinMenu(int iPad, void *initData, UILayer *parentLayer)
	: IUIScene_StartGame(iPad, parentLayer)
{
	initialiseMovie();

	m_iMiniGameType = s_pendingMiniGameType;
	m_iJoinModeSelection = 0;
	m_bIgnoreInput = false;
	m_bShowTexturePackDescription = false;
	m_texturePackDescDisplayed = false;
	m_iSetTexturePackDescription = -1;

	// Igual que en Create, el panel derecho del SWF sigue usando labels heredados
	// con nombres de TexturePack aunque hoy funcionen como descripcion del modo.
	m_labelGameName.init(IDS_BATTLE);
	m_buttonMoreOptions.init(IDS_MORE_OPTIONS, eControl_MoreOptions);
	m_buttonJoin.init(IDS_JOIN_GAME, eControl_Join);
	m_sliderGamemode.init(GetJoinModeLabel(m_iJoinModeSelection), eControl_GameMode, 0, 3, m_iJoinModeSelection);
	m_labelTexturePackName.init(L"");
	m_labelTexturePackDescription.init(L"");

	ApplyMiniGamePresentation();
	updateTooltips();
}

void UIScene_MinigamesJoinMenu::ApplyMiniGamePresentation()
{
	const wchar_t* miniGameName = L"Battle";
	const wchar_t* miniGameIconTexture = L"mg_battle_join";
	const wchar_t* miniGameIconArchive = L"Graphics\\BattleModeIcon.png";
	const wchar_t* miniGameScreenTexture = L"mg_battle_join_screen";
	const wchar_t* miniGameScreenArchive = L"Graphics\\GameModeGraphics\\BattleModeScreen.png";

	// Join Battle es un menu reducido: solo Game Type, More Options, Join e
	// imagen/presentacion del minijuego.
	m_labelGameName.setLabel(miniGameName, true, true);
	m_buttonMoreOptions.setLabel(IDS_MORE_OPTIONS, true, true);
	m_buttonJoin.setLabel(IDS_JOIN_GAME, true, true);
	m_sliderGamemode.setLabel(GetJoinModeLabel(m_iJoinModeSelection), true, true);

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
}
void UIScene_MinigamesJoinMenu::ApplyMiniGameOptionDescription(int controlId)
{
	// En Join, More Options y Join muestran la misma descripcion del preset
	// actual del slider para mantener el panel derecho consistente.
	const wchar_t* description = GetJoinModeDescription(m_iJoinModeSelection);

	switch (controlId)
	{
	case eControl_GameMode:
	case eControl_MoreOptions:
	case eControl_Join:
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

void UIScene_MinigamesJoinMenu::updateTooltips()
{
	ui.SetTooltips(DEFAULT_XUI_MENU_USER, IDS_TOOLTIPS_SELECT, IDS_TOOLTIPS_BACK, -1, -1);
}

void UIScene_MinigamesJoinMenu::updateComponents()
{
	m_parentLayer->showComponent(m_iPad, eUIComponent_Panorama, true);
	m_parentLayer->showComponent(m_iPad, eUIComponent_Logo, false);

	if (m_iSetTexturePackDescription >= 0)
	{
		ApplyMiniGameOptionDescription(m_iSetTexturePackDescription);
		m_iSetTexturePackDescription = -1;
	}
}

void UIScene_MinigamesJoinMenu::tick()
{
	UIScene::tick();
}

UIControl* UIScene_MinigamesJoinMenu::GetMainPanel()
{
	return &m_controlMainPanel;
}
// here the code ask for the swf
wstring UIScene_MinigamesJoinMenu::getMoviePath()
{
	return L"MiniGamesJoinMenu"; 
}

void UIScene_MinigamesJoinMenu::handleInput(int iPad, int key, bool repeat, bool pressed, bool released, bool &handled)
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
		handled = true;
		break;
	}
}

void UIScene_MinigamesJoinMenu::handlePress(F64 controlId, F64 childId)
{
	(void)childId;

	if (m_bIgnoreInput)
		return;

	switch (static_cast<int>(controlId))
	{
	case eControl_MoreOptions:
		ui.NavigateToScene(m_iPad, eUIScene_LaunchMoreOptionsMenu, &m_MoreOptionsParams);
		break;
	case eControl_Join:
		ui.PlayUISFX(eSFX_Press);
		LaunchGame();
		break;
	}
}

void UIScene_MinigamesJoinMenu::handleSliderMove(F64 sliderId, F64 currentValue)
{
	if (static_cast<int>(sliderId) != eControl_GameMode)
		return;

	// there's a bug with the slider that i haven't been able to fix, whe you move the slider
	// the descripcion changes but it's not visible 
	// it will only appear if you change the button
	// sorry if i didn't manage to fix it
	int value = static_cast<int>(currentValue);
	m_iJoinModeSelection = value < 0 ? 0 : (value > 3 ? 3 : value);
	m_sliderGamemode.handleSliderMove(m_iJoinModeSelection);
	m_sliderGamemode.setLabel(GetJoinModeLabel(m_iJoinModeSelection));
	m_iSetTexturePackDescription = eControl_GameMode;
	ApplyMiniGameOptionDescription(eControl_GameMode);
}

void UIScene_MinigamesJoinMenu::handleGainFocus(bool navBack)
{
	(void)navBack;
	m_iMiniGameType = s_pendingMiniGameType;
	ApplyMiniGamePresentation();
	SetFocusToElement(eControl_GameMode);
}

bool UIScene_MinigamesJoinMenu::canMoveSlider(F64 sliderId)
{
	return static_cast<int>(sliderId) == eControl_GameMode;
}

void UIScene_MinigamesJoinMenu::handleTimerComplete(int id)
{
	(void)id;
}

void UIScene_MinigamesJoinMenu::handleFocusChange(F64 controlId, F64 childId)
{
	(void)childId;
	m_iSetTexturePackDescription = static_cast<int>(controlId);
	ApplyMiniGameOptionDescription(static_cast<int>(controlId));
}

void UIScene_MinigamesJoinMenu::handleSelectionChanged(F64 selectedId)
{
	m_iSetTexturePackDescription = static_cast<int>(selectedId);
	ApplyMiniGameOptionDescription(static_cast<int>(selectedId));
}

void UIScene_MinigamesJoinMenu::handleTouchBoxRebuild()
{
}

void UIScene_MinigamesJoinMenu::checkStateAndStartGame()
{
	LaunchGame();
}

void UIScene_MinigamesJoinMenu::LaunchGame()
{
	
	// this is the same as Minigamescreate
	// it's used for debugging
	// if you want to remove it when everthing is ready, remove it
	app.DebugPrintf("[MGDBG] MiniGamesJoinMenu Join pressed for type=%d mode=%d\n", m_iMiniGameType, m_iJoinModeSelection);
}
