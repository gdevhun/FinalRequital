// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/FRWeaponSlotWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "FRDebugHelper.h"
#include "TimerManager.h"
#include "Player/FRPlayerState.h"
#include "UI/HUD/FRHUDWidget.h"

UFRWeaponSlotWidget::UFRWeaponSlotWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFRWeaponSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (PB_WeaponSkillCoolTime)
	{
		PB_WeaponSkillCoolTime->SetVisibility(ESlateVisibility::Hidden);
	}

	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AFRPlayerState* PS = PC->GetPlayerState<AFRPlayerState>())
		{
			bool bOwned = PS->HasWeapon(ThisSlotWeaponType);
			UpdateSlotVisibility(bOwned);
		}
	}

}
void UFRWeaponSlotWidget::UpdateSlotVisibility(bool bOwned) const
{
	if (bOwned)
	{
		IMG_Weapon->SetVisibility(ESlateVisibility::Visible);
		IMG_WeaponSelected->SetVisibility(ESlateVisibility::Hidden);
		PB_WeaponSkillCoolTime->SetVisibility(ESlateVisibility::Visible);
		IMG_WeaponDeactivated->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		IMG_Weapon->SetVisibility(ESlateVisibility::Hidden);
		IMG_WeaponSelected->SetVisibility(ESlateVisibility::Hidden);
		IMG_WeaponDeactivated->SetVisibility(ESlateVisibility::Visible);
	}
}
void UFRWeaponSlotWidget::UpdateCooldownProgress()
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	const float Elapsed = CurrentTime - CooldownStartTime;

	// 시간이 지날수록 퍼센트 증가
	float Ratio = Elapsed / MaxCooldownTime;
	Ratio = FMath::Clamp(Ratio, 0.f, 1.f);

	PB_WeaponSkillCoolTime->SetPercent(Ratio);

	if (Elapsed >= MaxCooldownTime)
	{
		GetWorld()->GetTimerManager().ClearTimer(CooldownUpdateTimerHandle);
	}
}

void UFRWeaponSlotWidget::UpdateSlotVisibilityByAcquisition(EWeaponType NewlyAcquiredWeapon)
{
	if (NewlyAcquiredWeapon == ThisSlotWeaponType)
	{
		PB_WeaponSkillCoolTime->SetVisibility(ESlateVisibility::Visible);
		IMG_Weapon->SetVisibility(ESlateVisibility::Visible);
		IMG_WeaponSelected->SetVisibility(ESlateVisibility::Hidden);
		IMG_WeaponDeactivated->SetVisibility(ESlateVisibility::Visible);
	}
}

void UFRWeaponSlotWidget::UpdateRestSlotVisibilityBySwap(EWeaponType NewlyAcquiredWeapon)
{
	if (NewlyAcquiredWeapon == ThisSlotWeaponType)
	{
		IMG_WeaponSelected->SetVisibility(ESlateVisibility::Hidden);
		IMG_WeaponDeactivated->SetVisibility(ESlateVisibility::Visible);
	}
}

void UFRWeaponSlotWidget::UpdateSlotVisibilityBySwap()
{
	IMG_WeaponSelected->SetVisibility(ESlateVisibility::Visible);
	IMG_WeaponDeactivated->SetVisibility(ESlateVisibility::Hidden);
}

void UFRWeaponSlotWidget::StartSlotCooldown(float Remaining, float TotalDuration)
{
	if (!PB_WeaponSkillCoolTime || TotalDuration <= 0.f) return;

	MaxCooldownTime = TotalDuration;
	CooldownStartTime = GetWorld()->GetTimeSeconds();

	UpdateCooldownProgress(); // 첫 프레임 즉시 갱신

	GetWorld()->GetTimerManager().SetTimer(
		CooldownUpdateTimerHandle,
		this,
		&UFRWeaponSlotWidget::UpdateCooldownProgress,
		0.025f,
		true
	);
}
