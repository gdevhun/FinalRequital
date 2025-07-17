// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/FRWeaponSlotWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"

UFRWeaponSlotWidget::UFRWeaponSlotWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFRWeaponSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (PB_WeaponSkillCoolTime && IMG_Weapon && IMG_WeaponSelected)
	{
		PB_WeaponSkillCoolTime->SetVisibility(ESlateVisibility::Hidden);
		IMG_Weapon->SetVisibility(ESlateVisibility::Hidden);
		IMG_WeaponSelected->SetVisibility(ESlateVisibility::Hidden);
	}

}

void UFRWeaponSlotWidget::UpdateSlotVisibility(EWeaponType NewlyAcquiredWeapon)
{
	if (NewlyAcquiredWeapon == ThisSlotWeaponType)
	{
		PB_WeaponSkillCoolTime->SetVisibility(ESlateVisibility::Visible);
		IMG_Weapon->SetVisibility(ESlateVisibility::Visible);
		IMG_WeaponSelected->SetVisibility(ESlateVisibility::Visible);
		IMG_WeaponDeactivated->SetVisibility(ESlateVisibility::Hidden);
	}
}
