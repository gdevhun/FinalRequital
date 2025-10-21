// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FRMonsterSpawner.h"
#include "Character/FRMonsterBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"

AFRMonsterSpawner::AFRMonsterSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	SpawnInterval = 5.0f;
	SpecialSpawnPercentage = 10.0f;
	RandomDeviation = 1.0f;
	FirstSpawnDelay = 0.0f;
}
void AFRMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();
}

void AFRMonsterSpawner::StartSpawn()
{
	if (!GetWorld()) return;

	// 첫 스폰을 FirstSpawnDelay 후에 실행
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AFRMonsterSpawner::HandleSpawnTimer,
		FMath::Max(0.0f, FirstSpawnDelay),
		false
	);
}

void AFRMonsterSpawner::StopSpawn()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}
}

void AFRMonsterSpawner::HandleSpawnTimer()
{
	// 몬스터 스폰 실행
	SpawnMonster();

	// 다음 스폰 간격 랜덤 적용
	const float NextInterval = UKismetMathLibrary::RandomFloatInRange(
		SpawnInterval - RandomDeviation,
		SpawnInterval + RandomDeviation
	);

	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AFRMonsterSpawner::HandleSpawnTimer,
		FMath::Max(0.1f, NextInterval), // 최소 간격 보장
		false
	);
}

void AFRMonsterSpawner::SpawnMonster()
{
	if (!GetWorld()) return;

	TSubclassOf<AFRMonsterBase> MonsterToSpawn = nullptr;
	const float RandomValue = UKismetMathLibrary::RandomFloatInRange(0.f, 100.f);

	if (RandomValue <= SpecialSpawnPercentage)
	{
		const int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, 2);
		switch (RandomIndex)
		{
		case 0: MonsterToSpawn = DamageUpMonsterClass; break;
		case 1: MonsterToSpawn = HpUpMonsterClass; break;
		case 2: MonsterToSpawn = SpeedUpMonsterClass; break;
		}
	}
	else
	{
		MonsterToSpawn = BaseMonsterClass;
	}

	if (MonsterToSpawn)
	{
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<AFRMonsterBase>(
			MonsterToSpawn,
			GetActorLocation(),
			GetActorRotation(),
			SpawnParams
		);
	}
}