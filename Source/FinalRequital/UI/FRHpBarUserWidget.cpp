// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/FRHpBarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "FRDebugHelper.h"
#include "GAS/Attribute/FRMonsterAttributeSet.h"

void UFRHpBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if(ASC)
	{
		// Hp 감지 변화 UI 어트리뷰트셋으로부터 가져온 후 델리게이트 바인딩
		ASC->GetGameplayAttributeValueChangeDelegate(UFRMonsterAttributeSet::GetHealthAttribute()).AddUObject(this, &UFRHpBarUserWidget::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UFRMonsterAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UFRHpBarUserWidget::OnMaxHealthChanged);

		const UFRMonsterAttributeSet* CurrentAttributeSet = ASC->GetSet<UFRMonsterAttributeSet>();
		if(CurrentAttributeSet)
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

void UFRHpBarUserWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UFRHpBarUserWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UFRHpBarUserWidget::UpdateHpBar()
{
	if(PbHpBar)
	{
		PbHpBar->SetPercent(CurrentHealth / CurrentMaxHealth);
	}
	if(TxtHpStat)
	{
		TxtHpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%0.f"), CurrentHealth, CurrentMaxHealth)));
	}
}
