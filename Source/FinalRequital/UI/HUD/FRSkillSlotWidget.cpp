// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/FRSkillSlotWidget.h"
#include "FRDebugHelper.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "TimerManager.h"
#include "Player/FRMaskSkillComponent.h"
#include "UI/HUD/FRHUDWidget.h"

UFRSkillSlotWidget::UFRSkillSlotWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}
void UFRSkillSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (PB_MaskSkillCoolTime && IMG_MaskSkill && IMG_MaskSkillDeactivated)
	{
		PB_MaskSkillCoolTime->SetVisibility(ESlateVisibility::Hidden);
		IMG_MaskSkill->SetVisibility(ESlateVisibility::Hidden);
	}
}
void UFRSkillSlotWidget::UpdateSkillImage(EMaskSkillType SelectedSkillType)
{
	if (!IMG_MaskSkill) return;

	if (UTexture2D** FoundTexture = SkillImages.Find(SelectedSkillType))
	{
		PB_MaskSkillCoolTime->SetVisibility(ESlateVisibility::Visible);
		IMG_MaskSkill->SetBrushFromTexture(*FoundTexture);
		IMG_MaskSkill->SetVisibility(ESlateVisibility::Visible);
		IMG_MaskSkillDeactivated->SetBrushFromTexture(ActivatedSkillFrame);

	}

}

void UFRSkillSlotWidget::UpdateCooldownProgress()
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	const float Elapsed = CurrentTime - CooldownStartTime;

	// 시간이 지날수록 퍼센트 증가
	float Ratio = Elapsed / MaxCooldownTime;
	Ratio = FMath::Clamp(Ratio, 0.f, 1.f);

	PB_MaskSkillCoolTime->SetPercent(Ratio);

	if (Elapsed >= MaxCooldownTime)
	{
		GetWorld()->GetTimerManager().ClearTimer(CooldownUpdateTimerHandle);
	}
}


void UFRSkillSlotWidget::StartSkillSlotCooldown(float Remaining, float TotalDuration)
{
	if (!PB_MaskSkillCoolTime || TotalDuration <= 0.f) return;

	MaxCooldownTime = TotalDuration;
	CooldownStartTime = GetWorld()->GetTimeSeconds();

	UpdateCooldownProgress(); // 첫 프레임 즉시 갱신

	GetWorld()->GetTimerManager().SetTimer(
		CooldownUpdateTimerHandle,
		this,
		&UFRSkillSlotWidget::UpdateCooldownProgress,
		0.025f,
		true
	);
}

