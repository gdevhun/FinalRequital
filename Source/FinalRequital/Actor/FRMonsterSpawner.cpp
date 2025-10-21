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
	RandomDeviation = 1.0f; // 예: ±1초 변동
}

void AFRMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();
}

void AFRMonsterSpawner::StartSpawn()
{
	if (GetWorld())
	{
		SpawnMonster();

		// 첫 스폰 이후 주기적으로 실행
		const float InitialInterval = UKismetMathLibrary::RandomFloatInRange(
			SpawnInterval - RandomDeviation,
			SpawnInterval + RandomDeviation
		);

		GetWorld()->GetTimerManager().SetTimer(
			SpawnTimerHandle,
			this,
			&AFRMonsterSpawner::HandleSpawnTimer,
			InitialInterval,
			false // 반복이 아니라 매번 새 간격으로 다시 설정
		);
	}
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
	SpawnMonster();

	// 다음 스폰 간격도 랜덤하게 다시 설정
	const float NextInterval = UKismetMathLibrary::RandomFloatInRange(
		SpawnInterval - RandomDeviation,
		SpawnInterval + RandomDeviation
	);

	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AFRMonsterSpawner::HandleSpawnTimer,
		FMath::Max(0.1f, NextInterval), // 최소값 제한 (0 이하 방지)
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