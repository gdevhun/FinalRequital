// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FRHintBoxHpWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GAS/Attribute/FRHintBoxAttributeSet.h"

void UFRHintBoxHpWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UFRHintBoxAttributeSet::GetHealthAttribute()).AddUObject(this, &UFRHintBoxHpWidget::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UFRHintBoxAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UFRHintBoxHpWidget::OnMaxHealthChanged);

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

void UFRHintBoxHpWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UFRHintBoxHpWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UFRHintBoxHpWidget::UpdateHpBar()
{
	if (PB_HintBoxHp)
	{
		PB_HintBoxHp->SetPercent(CurrentHealth / CurrentMaxHealth);
	}
	if (TXT_HintBoxHpStat)
	{
		TXT_HintBoxHpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%0.f"), CurrentHealth, CurrentMaxHealth)));
	}
}
