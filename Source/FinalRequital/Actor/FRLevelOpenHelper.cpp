// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FRLevelOpenHelper.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/FRGASCharacterPlayer.h"
#include "Player/FRPlayerState.h"
#include "System/FRGameInstance.h"

AFRLevelOpenHelper::AFRLevelOpenHelper()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	CollisionBox->SetGenerateOverlapEvents(true);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AFRLevelOpenHelper::OnOverlapBegin);
}

void AFRLevelOpenHelper::BeginPlay()
{
	Super::BeginPlay();
}

void AFRLevelOpenHelper::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFRGASCharacterPlayer* Player = Cast<AFRGASCharacterPlayer>(OtherActor);
	if (!Player) return;


	AFRPlayerState* PS = Player->GetPlayerState<AFRPlayerState>();
	if (PS)
	{
		if (UFRGameInstance* GI = GetGameInstance<UFRGameInstance>())
		{
			GI->PersistentPlayerData.AcquiredWeapons = PS->AcquiredWeapons;
			GI->PersistentPlayerData.SelectedMaskSkill = PS->GetSelectedMaskSkill();
			GI->PersistentPlayerData.Stat_H = PS->Stat_H;
			GI->PersistentPlayerData.Stat_D = PS->Stat_D;
			GI->PersistentPlayerData.Stat_P = PS->Stat_P;
		}
	}

	OpenNextLevel();
}

void AFRLevelOpenHelper::OpenNextLevel()
{
	if (NextLevelName != NAME_None)
	{
		UGameplayStatics::OpenLevel(this, NextLevelName);
	}
}
