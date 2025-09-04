// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/FRMainMenuWidget.h"
#include "Components/Button.h"
#include "Player/FRPlayerController.h"
#include "UI/MainMenu/FRProfileWidget.h"
#include "UI/MainMenu/FRGameSettingWidget.h"
#include "UI/MainMenu/FRTitleExitWidget.h"

UFRMainMenuWidget::UFRMainMenuWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFRMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Button Bind Callback function
    if (BTN_Profile) BTN_Profile->OnClicked.AddDynamic(this, &UFRMainMenuWidget::OnProfileBtnCallback);
    if (BTN_GameSettingMenu) BTN_GameSettingMenu->OnClicked.AddDynamic(this, &UFRMainMenuWidget::OnGameSettingMenuBtnCallback);
    if (BTN_TitleMenu) BTN_TitleMenu->OnClicked.AddDynamic(this, &UFRMainMenuWidget::OnTitleMenuBtnCallback);
    if (BTN_Back) BTN_Back->OnClicked.AddDynamic(this, &UFRMainMenuWidget::OnBackBtnCallback);

}

void UFRMainMenuWidget::OnProfileBtnCallback()
{
	if (FRProfileWidgetClass)
	{
		ProfileWidget = CreateWidget<UFRProfileWidget>(this, FRProfileWidgetClass);
		if (ProfileWidget)
		{
			AddUIWithPlayerController(ProfileWidget);
		}
	}
}

void UFRMainMenuWidget::OnGameSettingMenuBtnCallback()
{
	if (FRGameSettingWidgetClass)
	{
		GameSettingWidget = CreateWidget<UFRGameSettingWidget>(this, FRGameSettingWidgetClass);
		if (GameSettingWidget)
		{
			AddUIWithPlayerController(GameSettingWidget);
		}
	}
}

void UFRMainMenuWidget::OnTitleMenuBtnCallback()
{
	if (FRTitleExitWidgetClass)
	{
		TitleExitWidget = CreateWidget<UFRTitleExitWidget>(this, FRTitleExitWidgetClass);
		if (TitleExitWidget)
		{
			AddUIWithPlayerController(TitleExitWidget);
		}
	}
}

void UFRMainMenuWidget::OnBackBtnCallback()
{
	RemoveUIWithPlayerController();
}
