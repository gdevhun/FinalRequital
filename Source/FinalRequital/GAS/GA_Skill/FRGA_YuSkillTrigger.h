// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FRGA_YuSkillTrigger.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRGA_YuSkillTrigger : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UFRGA_YuSkillTrigger();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
};
