// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FRBossLotusPatternSpawner.generated.h"

UCLASS()
class FINALREQUITAL_API AFRBossLotusPatternSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AFRBossLotusPatternSpawner();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Boss|LotusPattern")
	void ActivateLotusPattern(AActor* TargetPlayer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|LotusPattern")
	TSubclassOf<AActor> HealingPatternActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|LotusPattern")
	TSubclassOf<AActor> ProjectilePatternActorClass;

	bool ShouldSpawnHealingPattern() const;

};
