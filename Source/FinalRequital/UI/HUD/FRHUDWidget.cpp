// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/FRHUDWidget.h"
#include "FRPlayerHpBarWidget.h"
#include "FRSkillSlotWidget.h"
#include "FRWeaponSlotWidget.h"
#include "Components/Image.h"
#include "Player/FRPlayerState.h"
#include "Player/FRWeaponComponent.h"

UFRHUDWidget::UFRHUDWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFRHUDWidget::InitHUDWidget(class UAbilitySystemComponent* InASC)
{
	if (!InASC) return;
	ASC = InASC;

	if (WBP_PlayerHpBar)
	{
		WBP_PlayerHpBar->InitWithAbilitySystem(ASC);
	}
}


void UFRHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IMG_CrossHair)
	{
		IMG_CrossHair->SetColorAndOpacity(FLinearColor::White);
		ShowCrossHair(false);
	}

	// BIND DELEGATE
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

void UFRHUDWidget::SetHUDVisibility(bool bVisible)
{
	const ESlateVisibility NewVisibility = bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;

	if (WBP_PlayerHpBar) WBP_PlayerHpBar->SetVisibility(NewVisibility);
	if (IMG_CrossHair) IMG_CrossHair->SetVisibility(NewVisibility);
	if (WBP_SkillSlot) WBP_SkillSlot->SetVisibility(NewVisibility);
	if (WBP_WeaponSlot_1) WBP_WeaponSlot_1->SetVisibility(NewVisibility);
	if (WBP_WeaponSlot_2) WBP_WeaponSlot_2->SetVisibility(NewVisibility);
	if (WBP_WeaponSlot_3) WBP_WeaponSlot_3->SetVisibility(NewVisibility);
	if (WBP_WeaponSlot_4) WBP_WeaponSlot_4->SetVisibility(NewVisibility);
}

void UFRHUDWidget::OnWeaponAcquiredFromState(EWeaponType WeaponType)
{
	TArray<UFRWeaponSlotWidget*> Slots = { WBP_WeaponSlot_1, WBP_WeaponSlot_2, WBP_WeaponSlot_3, WBP_WeaponSlot_4 };

	for (UFRWeaponSlotWidget* WeaponSlot : Slots)
	{
		if (WeaponSlot)
		{
			WeaponSlot->UpdateSlotVisibilityByAcquisition(WeaponType);
		}
	}
}

void UFRHUDWidget::SetStagePlayStatus()
{
	IMG_MemoryCellAcquireStatus->SetVisibility(ESlateVisibility::Visible);
}

void UFRHUDWidget::AcquireMemoryCell()
{
	if (!bIsGetFirstMemoryCell)
	{

		if (IMG_MemoryCellAcquireStatus && MemoryCell_AcquireTexture_1)
		{
			IMG_MemoryCellAcquireStatus->SetBrushFromTexture(MemoryCell_AcquireTexture_1);
			bIsGetFirstMemoryCell = true;
		}
	}
	else
	{
		if (IMG_MemoryCellAcquireStatus && MemoryCell_AcquireTexture_2)
		{
			IMG_MemoryCellAcquireStatus->SetBrushFromTexture(MemoryCell_AcquireTexture_2);
		}
	}
}

void UFRHUDWidget::UpdateWeaponSlotBySwap(EWeaponType CurrentEquippedWeaponType)
{
	auto UpdateSlot = [&](UFRWeaponSlotWidget* SlotWidget, EWeaponType SlotType)
		{
			if (!SlotWidget) return;

			if (CurrentEquippedWeaponType == SlotType)
			{
				SlotWidget->UpdateSlotVisibilityBySwap();
			}
			else
			{
				SlotWidget->UpdateRestSlotVisibilityBySwap(SlotType); //Deactivation
			}
		};

	UpdateSlot(WBP_WeaponSlot_1, EWeaponType::Sword);
	UpdateSlot(WBP_WeaponSlot_2, EWeaponType::Bow);
	UpdateSlot(WBP_WeaponSlot_3, EWeaponType::IronMace);
	UpdateSlot(WBP_WeaponSlot_4, EWeaponType::BronzeBell);
}
