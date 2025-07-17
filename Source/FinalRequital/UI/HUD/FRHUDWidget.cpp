// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/FRHUDWidget.h"

#include "FRWeaponSlotWidget.h"
#include "Components/Image.h"
#include "Player/FRPlayerState.h"

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

	BindActivatedWeaponSlotsToPlayerState();
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

void UFRHUDWidget::BindActivatedWeaponSlotsToPlayerState()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AFRPlayerState* PS = Cast<AFRPlayerState>(PC->GetPlayerState<AFRPlayerState>()))
		{
			PS->OnWeaponAcquired.AddDynamic(this, &UFRHUDWidget::OnWeaponAcquiredFromState);
		}
	}
}

void UFRHUDWidget::OnWeaponAcquiredFromState(EWeaponType WeaponType)
{
	TArray<UFRWeaponSlotWidget*> Slots = { WBP_WeaponSlot_1, WBP_WeaponSlot_2, WBP_WeaponSlot_3, WBP_WeaponSlot_4 };

	for (UFRWeaponSlotWidget* WeaponSlot : Slots)
	{
		if (WeaponSlot)
		{
			WeaponSlot->UpdateSlotVisibility(WeaponType);
		}
	}
}
