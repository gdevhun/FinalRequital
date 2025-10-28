// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FRBossJangseungBaseHpWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GAS/Attribute/FRHintBoxAttributeSet.h"

void UFRBossJangseungBaseHpWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UFRHintBoxAttributeSet::GetHealthAttribute()).AddUObject(this, &UFRBossJangseungBaseHpWidget::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UFRHintBoxAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UFRBossJangseungBaseHpWidget::OnMaxHealthChanged);

		const UFRHintBoxAttributeSet* CurrentAttributeSet = ASC->GetSet<UFRHintBoxAttributeSet>();
		if (CurrentAttributeSet)
		{
			CurrentHealth = CurrentAttributeSet->GetHealth();
			CurrentMaxHealth = CurrentAttributeSet->GetMaxHealth();
			if (CurrentMaxHealth > 0.0f)
			{
				UpdateHpBar();
			}
		}
	}
}

void UFRBossJangseungBaseHpWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UFRBossJangseungBaseHpWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UFRBossJangseungBaseHpWidget::UpdateHpBar()
{
	if (PB_JangseungHp)
	{
		PB_JangseungHp->SetPercent(CurrentHealth / CurrentMaxHealth);
	}
	if (TXT_JangseungHpStat)
	{
		TXT_JangseungHpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%0.f"), CurrentHealth, CurrentMaxHealth)));
	}
}
