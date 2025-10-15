// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FRUserWidget.h"
#include "GameplayEffectTypes.h"
#include "FRHintBoxHpWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRHintBoxHpWidget : public UFRUserWidget
{
	GENERATED_BODY()

protected:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

	virtual void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData);

	void UpdateHpBar();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UProgressBar> PB_HintBoxHp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_HintBoxHpStat;

	float CurrentHealth = 0.0f;
	float CurrentMaxHealth = 0.1f;
};
