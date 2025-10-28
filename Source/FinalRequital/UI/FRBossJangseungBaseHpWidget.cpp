// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FRBossJangseungBaseHpWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GAS/Attribute/FRPhase1JangseungAttributeSet.h"
#include "GAS/Attribute/FRPhase2JangseungAttributeSet.h"
#include "GAS/Attribute/FRPhase3JangseungAttributeSet.h"

void UFRBossJangseungBaseHpWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (!ASC)
	{
		return;
	}

	// PhaseNum에 따라 적절한 AttributeSet 바인딩
	switch (PhaseNum)
	{
	case 1:
		BindAttributeSet<UFRPhase1JangseungAttributeSet>();
		break;
	case 2:
		BindAttributeSet<UFRPhase2JangseungAttributeSet>();
		break;
	case 3:
		BindAttributeSet<UFRPhase3JangseungAttributeSet>();
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid PhaseNum: %d"), PhaseNum);
		break;
	}
}

template<typename TAttributeSet>
void UFRBossJangseungBaseHpWidget::BindAttributeSet()
{
	// Delegate 바인딩
	ASC->GetGameplayAttributeValueChangeDelegate(TAttributeSet::GetHealthAttribute())
		.AddUObject(this, &UFRBossJangseungBaseHpWidget::OnHealthChanged);

	ASC->GetGameplayAttributeValueChangeDelegate(TAttributeSet::GetMaxHealthAttribute())
		.AddUObject(this, &UFRBossJangseungBaseHpWidget::OnMaxHealthChanged);

	// 현재 값 가져오기
	const TAttributeSet* CurrentAttributeSet = ASC->GetSet<TAttributeSet>();
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
		TXT_JangseungHpStat->SetText(
			FText::FromString(FString::Printf(TEXT("%.0f/%.0f"), CurrentHealth, CurrentMaxHealth))
		);
	}
}