// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/TA/FRTA_Trace.h"
#include "FRTA_FindSoulTrace.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API AFRTA_FindSoulTrace : public AFRTA_Trace
{
	GENERATED_BODY()

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const override;

};
