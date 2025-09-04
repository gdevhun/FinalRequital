// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FRUserWidget.h"
#include "GameplayEffectTypes.h"
#include "FRPlayerHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRPlayerHpBarWidget : public UFRUserWidget
{
	GENERATED_BODY()
public:
	UFRPlayerHpBarWidget(const FObjectInitializer& ObjectInitializer);

	void InitWithAbilitySystem(UAbilitySystemComponent* InASC);

protected:
	virtual void NativeConstruct() override;

	virtual void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData);

	void UpdatePlayerHpBar();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UProgressBar> PB_PlayerOwnHp;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	//TObjectPtr<class UProgressBar> PB_PlayerRemoteHp;

	float CurrentHealth = 0.0f;
	float CurrentMaxHealth = 0.1f;
};
