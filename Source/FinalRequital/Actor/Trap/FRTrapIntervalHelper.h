// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FRTrapIntervalHelper.generated.h"

UENUM(BlueprintType)
enum class ETrapIntervalType : uint8
{
	Interval_1_25 UMETA(DisplayName = "1.25 Seconds"),
	Interval_1_5  UMETA(DisplayName = "1.5 Seconds"),
	Interval_2  UMETA(DisplayName = "2 Seconds"),
	Interval_2_5  UMETA(DisplayName = "2.5 Seconds"),
	Interval_3    UMETA(DisplayName = "3 Seconds")
};

UCLASS()
class FINALREQUITAL_API UFRTrapIntervalHelper : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Trap")
	static float GetIntervalSeconds(ETrapIntervalType IntervalType)
	{
		switch (IntervalType)
		{
		case ETrapIntervalType::Interval_1_25: return 1.25f;
		case ETrapIntervalType::Interval_1_5:  return 1.5f;
		case ETrapIntervalType::Interval_2:  return 2.0f;
		case ETrapIntervalType::Interval_2_5:  return 2.5f;
		case ETrapIntervalType::Interval_3: return 3.0f;

		default: return 3.0f;
		}
	}
};
