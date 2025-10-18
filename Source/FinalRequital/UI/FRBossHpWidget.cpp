// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FRBossHpWidget.h"

#include "Components/ProgressBar.h"
#include "GAS/Attribute/FRBossAttributeSet.h"

void UFRBossHpWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (ASC)
	{
		if (const UFRBossAttributeSet* BossAttribute = ASC->GetSet<UFRBossAttributeSet>())
		{
			// Init
			PB_Hp->SetPercent(BossAttribute->GetHealth() / BossAttribute->GetMaxHealth());

			// Hp damaged delegate bind
			BossAttribute->OnBossTakeDamage.AddDynamic(this, &UFRBossHpWidget::UpdateHpBar);

			// Hp 0 delegate bind
			BossAttribute->OnBossOutOfHealth.AddDynamic(this, &UFRBossHpWidget::HandleOutOfHealth);
		}
	}
}

void UFRBossHpWidget::UpdateHpBar()
{
	if (!ASC) return;

	if (const UFRBossAttributeSet* BossAttribute = ASC->GetSet<UFRBossAttributeSet>())
	{
		PB_Hp->SetPercent(BossAttribute->GetHealth() / BossAttribute->GetMaxHealth());
	}
}

void UFRBossHpWidget::HandleOutOfHealth_Implementation()
{
}

