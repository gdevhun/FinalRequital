// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FRMonsterSpawner.generated.h"

UCLASS()
class FINALREQUITAL_API AFRMonsterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AFRMonsterSpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Monster")
	TSubclassOf<class AFRMonsterBase> BaseMonsterClass;
	UPROPERTY(EditAnywhere, Category = "Monster")
	TSubclassOf<class AFRMonsterBase> DamageUpMonsterClass;
	UPROPERTY(EditAnywhere, Category = "Monster")
	TSubclassOf<class AFRMonsterBase> HpUpMonsterClass;
	UPROPERTY(EditAnywhere, Category = "Monster")
	TSubclassOf<class AFRMonsterBase> SpeedUpMonsterClass;

	UPROPERTY(EditAnywhere, Category = "Monster")
	float SpawnInterval;
	UPROPERTY(EditAnywhere, Category = "Monster")
	float SpecialSpawnPercentage;
	UPROPERTY(EditAnywhere, Category = "Monster")
	float RandomDeviation = 0.0f;;

	UFUNCTION(BlueprintCallable)
	void SpawnMonster();

public:
	UFUNCTION(BlueprintCallable)
	void StartSpawn();
	UFUNCTION(BlueprintCallable)
	void StopSpawn();

private:
	FTimerHandle SpawnTimerHandle;

};
