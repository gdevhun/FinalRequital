// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FRLevelOpenHelper.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/FRGASCharacterPlayer.h"

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

	// 서버에서만 실행
	if (bOnlyServerCanTrigger && !HasAuthority()) return;

	// Optional: 중복 트리거 방지 (한 번만 호출하고 싶을 때)
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// SeamlessTravel로 레벨 이동
	UWorld* World = GetWorld();
	if (World)
	{
		FString CurrentURL = NextLevelName.ToString();
		UGameplayStatics::OpenLevel(World, FName(*CurrentURL), true); // true = seamless
	}
}