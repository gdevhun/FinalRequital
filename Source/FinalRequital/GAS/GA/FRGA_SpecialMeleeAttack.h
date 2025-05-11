// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FRGA_SpecialMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRGA_SpecialMeleeAttack : public UGameplayAbility
{
	GENERATED_BODY()
public:

	UFRGA_SpecialMeleeAttack();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnInterruptedCallback();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> SpecialAttackActionMontage;
};
