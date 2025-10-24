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

	// 기본 스폰 간격(초)
	UPROPERTY(EditAnywhere, Category = "Monster")
	float SpawnInterval;

	// 특수 몬스터가 스폰될 확률(%)
	UPROPERTY(EditAnywhere, Category = "Monster")
	float SpecialSpawnPercentage;

	// 스폰 간격의 랜덤 편차(±값)
	UPROPERTY(EditAnywhere, Category = "Monster")
	float RandomDeviation;

	// 첫 스폰 지연 시간(초)
	UPROPERTY(EditAnywhere, Category = "Monster")
	float FirstSpawnDelay;

	UFUNCTION(BlueprintCallable)
	void SpawnMonster();
	void HandleFirstSpawn();
	void HandleSpawnTimer();

public:
	UFUNCTION(BlueprintCallable)
	void StartSpawn();
	UFUNCTION(BlueprintCallable)
	void StopSpawn();

private:
	FTimerHandle SpawnTimerHandle;

};
