// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FRGA_BossMeleeAttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRGA_BossMeleeAttackHitCheck : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UFRGA_BossMeleeAttackHitCheck();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> AttackDamageEffect;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class AFRTA_BossTrace> TargetActorClass;
};
