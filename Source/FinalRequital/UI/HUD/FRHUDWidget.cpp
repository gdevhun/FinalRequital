// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/FRHUDWidget.h"
#include "Components/Image.h"

UFRHUDWidget::UFRHUDWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFRHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IMG_CrossHair)
	{
		IMG_CrossHair->SetColorAndOpacity(FLinearColor::White);
		ShowCrossHair(false);
	}
	
}

void UFRHUDWidget::ShowCrossHair(bool bShow)
{
	if (IMG_CrossHair)
	{
		IMG_CrossHair->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UFRHUDWidget::ChangeCrossHairColor()
{
	if (IMG_CrossHair)
	{
		IMG_CrossHair->SetColorAndOpacity(FLinearColor::Green);
	}
}

void UFRHUDWidget::ResetCrossHairColor()
{
	if (IMG_CrossHair)
	{
		IMG_CrossHair->SetColorAndOpacity(FLinearColor::White);
	}
}
