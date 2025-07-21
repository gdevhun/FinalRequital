// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/FRPlayerHpBarWidget.h"

#include "Components/ProgressBar.h"
#include "GAS/Attribute/FRCharacterAttributeSet.h"

UFRPlayerHpBarWidget::UFRPlayerHpBarWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFRPlayerHpBarWidget::InitWithAbilitySystem(UAbilitySystemComponent* InASC)
{
	// Hp 감지 변화 UI 어트리뷰트셋으로부터 가져온 후 델리게이트 바인딩
	if (!InASC) return;

	ASC = InASC;

	ASC->GetGameplayAttributeValueChangeDelegate(UFRCharacterAttributeSet::GetHealthAttribute())
		.AddUObject(this, &UFRPlayerHpBarWidget::OnHealthChanged);

	ASC->GetGameplayAttributeValueChangeDelegate(UFRCharacterAttributeSet::GetMaxHealthAttribute())
		.AddUObject(this, &UFRPlayerHpBarWidget::OnMaxHealthChanged);

	if (const UFRCharacterAttributeSet* CurrentAttributeSet = ASC->GetSet<UFRCharacterAttributeSet>())
	{
		CurrentHealth = CurrentAttributeSet->GetHealth();
		CurrentMaxHealth = CurrentAttributeSet->GetMaxHealth();
		UpdatePlayerHpBar();
	}
}

void UFRPlayerHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFRPlayerHpBarWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdatePlayerHpBar();
}

void UFRPlayerHpBarWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdatePlayerHpBar();
}

void UFRPlayerHpBarWidget::UpdatePlayerHpBar()
{
	if (PB_PlayerOwnHp)
	{
		PB_PlayerOwnHp->SetPercent(CurrentHealth / CurrentMaxHealth);
	}
}
