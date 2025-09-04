// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/FRTitleExitWidget.h"

#include "Components/Button.h"

UFRTitleExitWidget::UFRTitleExitWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFRTitleExitWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BTN_Back) BTN_Back->OnClicked.AddDynamic(this, &UFRTitleExitWidget::OnBackBtnCallback);
	if (BTN_No) BTN_No->OnClicked.AddDynamic(this, &UFRTitleExitWidget::OnNoBtnCallback);
	if (BTN_Yes) BTN_Yes->OnClicked.AddDynamic(this, &UFRTitleExitWidget::OnYesBtnCallback);
}

void UFRTitleExitWidget::OnNoBtnCallback()
{
	RemoveUIWithPlayerController();
}

void UFRTitleExitWidget::OnYesBtnCallback()
{

}

void UFRTitleExitWidget::OnBackBtnCallback()
{
	RemoveUIWithPlayerController();
}
