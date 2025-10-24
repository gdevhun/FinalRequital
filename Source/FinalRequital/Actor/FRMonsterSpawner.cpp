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
	if (!GetWorld())
	{
		return;
	}

	// 첫 스폰은 FirstSpawnDelay 후에 딱 한 번만 실행
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AFRMonsterSpawner::HandleFirstSpawn, // 첫 스폰 전용 함수
		FMath::Max(0.1f, FirstSpawnDelay),
		false
	);
}

void AFRMonsterSpawner::HandleFirstSpawn()
{
	// 첫 스폰 실행
	SpawnMonster();

	// 그 후 반복 타이머 등록
	const float NextInterval = UKismetMathLibrary::RandomFloatInRange(
		SpawnInterval - RandomDeviation,
		SpawnInterval + RandomDeviation
	);

	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AFRMonsterSpawner::HandleSpawnTimer,
		FMath::Max(0.1f, NextInterval),
		false
	);
}

void AFRMonsterSpawner::HandleSpawnTimer()
{
	SpawnMonster();

	const float NextInterval = UKismetMathLibrary::RandomFloatInRange(
		SpawnInterval - RandomDeviation,
		SpawnInterval + RandomDeviation
	);

	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AFRMonsterSpawner::HandleSpawnTimer,
		FMath::Max(0.1f, NextInterval),
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

void AFRMonsterSpawner::SpawnMonster()
{
	if (!GetWorld())
	{
		return;
	}

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

	if (!MonsterToSpawn)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FVector SpawnLoc = GetActorLocation() + FVector(0, 0, 80);
	const FRotator SpawnRot = GetActorRotation();

	AFRMonsterBase* SpawnedMonster = GetWorld()->SpawnActor<AFRMonsterBase>(
		MonsterToSpawn,
		SpawnLoc,
		SpawnRot,
		SpawnParams
	);

}