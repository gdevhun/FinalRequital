// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FRBossAsura.generated.h"

UCLASS()
class FINALREQUITAL_API AFRBossAsura : public ACharacter
{
	GENERATED_BODY()

public:

	AFRBossAsura();

protected:

	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<class AFRCharacterBase> TargetPlayer;

	UFUNCTION()
	void LookAtTargetPlayer();

private:
	FTimerHandle LookAtTimerHandle;
};
