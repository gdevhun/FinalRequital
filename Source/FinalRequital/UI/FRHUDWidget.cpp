// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FRHUDWidget.h"
#include "Components/Image.h"

UFRHUDWidget::UFRHUDWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFRHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CrossHair)
	{
		CrossHair->SetColorAndOpacity(FLinearColor::White);
		ShowCrossHair(false);
	}
	
}

void UFRHUDWidget::ShowCrossHair(bool bShow)
{
	if (CrossHair)
	{
		CrossHair->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UFRHUDWidget::ChangeCrossHairColor()
{
	if (CrossHair)
	{
		CrossHair->SetColorAndOpacity(FLinearColor::Green);
	}
}

void UFRHUDWidget::ResetCrossHairColor()
{
	if (CrossHair)
	{
		CrossHair->SetColorAndOpacity(FLinearColor::White);
	}
}
