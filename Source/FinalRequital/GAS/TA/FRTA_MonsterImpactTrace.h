// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/TA/FRTA_MonsterTrace.h"
#include "FRTA_MonsterImpactTrace.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API AFRTA_MonsterImpactTrace : public AFRTA_MonsterTrace
{
	GENERATED_BODY()

public:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const override;
};
