// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FRGA_DetectandPush.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRGA_DetectandPush : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UFRGA_DetectandPush();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

public:
	UPROPERTY(EditAnywhere)
	float PushStrength = 1000.f;
};
