// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FRUserWidget.h"
#include "GameplayEffectTypes.h"
#include "FRHpBarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRHpBarUserWidget : public UFRUserWidget
{
	GENERATED_BODY()

protected:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

	virtual void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData);

	void UpdateHpBar();

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UProgressBar> PbHpBar;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> TxtHpStat;

	float CurrentHealth = 0.0f;
	float CurrentMaxHealth = 0.1f;
};
