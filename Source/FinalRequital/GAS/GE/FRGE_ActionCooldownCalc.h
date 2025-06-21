// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "FRGE_ActionCooldownCalc.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRGE_ActionCooldownCalc : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UFRGE_ActionCooldownCalc();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition Mana;
};
