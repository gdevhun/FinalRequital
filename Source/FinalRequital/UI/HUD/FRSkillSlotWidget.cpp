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
		IMG_MaskSkill->SetBrushFromTexture(*FoundTexture);
		IMG_MaskSkill->SetVisibility(ESlateVisibility::Visible);
		IMG_MaskSkillDeactivated->SetBrushFromTexture(ActivatedSkillFrame);
		
	}

}

