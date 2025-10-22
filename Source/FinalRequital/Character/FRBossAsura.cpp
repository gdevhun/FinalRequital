// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FRBossAsura.h"
#include "AbilitySystemComponent.h"
#include "FRCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GAS/Attribute/FRBossAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Physics/FRCollision.h"

AFRBossAsura::AFRBossAsura()
{
	PrimaryActorTick.bCanEverTick = true;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(28.f, 60.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_FRMONSTER);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(CCHANNEL_FRACTION, ECR_Block);

	// ASC
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UFRBossAttributeSet>(TEXT("MonsterAttributeSet"));

}

class UAbilitySystemComponent* AFRBossAsura::GetAbilitySystemComponent() const
{
	return ASC;
}

void AFRBossAsura::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);

	for (int32 i = 0; i < StartAbilities.Num(); ++i)
	{
		const TSubclassOf<UGameplayAbility>& StartAbility = StartAbilities[i];
		FGameplayAbilitySpec StartSpec(StartAbility);

		FGameplayAbilitySpecHandle GivenHandle = ASC->GiveAbility(StartSpec);

	}

	// 체력 관련 DELEGATE 연결 처리
	// if (AttributeSet)
	// {
	// 	AttributeSet->OnMonsterOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);
	// 	AttributeSet->OnMonsterTakeDamage.AddDynamic(this, &ThisClass::HitReact);
	// }

}

void AFRBossAsura::BeginPlay()
{
	Super::BeginPlay();

	TargetPlayer = Cast<AFRCharacterBase>(UGameplayStatics::GetPlayerCharacter(this, 0));

	GetWorldTimerManager().SetTimer(
		LookAtTimerHandle,
		this,
		&AFRBossAsura::UpdateTargetRotation,
		1.5f,
		true
	);

}
void AFRBossAsura::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 자연스럽게 회전 (보간)
	if (!bShouldRotate || !TargetPlayer) return;

	const FRotator CurrentRot = GetActorRotation();
	const FRotator InterpRot = FMath::RInterpTo(CurrentRot, TargetRotation, DeltaSeconds, 3.5f); 
	SetActorRotation(InterpRot);
}
void AFRBossAsura::UpdateTargetRotation()
{
	if (!TargetPlayer) return;

	const FVector MyLoc = GetActorLocation();
	const FVector TargetLoc = TargetPlayer->GetActorLocation();
	const FVector FlatTargetLoc(TargetLoc.X, TargetLoc.Y, MyLoc.Z);

	TargetRotation = (FlatTargetLoc - MyLoc).Rotation();
	bShouldRotate = true;
}

void AFRBossAsura::OnOutOfHealth()
{
}

/*void AFRBossAsura::LookAtTargetPlayer()
{
	if (!TargetPlayer) return;

	const FVector MyLocation = GetActorLocation();
	const FVector TargetLocation = TargetPlayer->GetActorLocation();

	const FVector FlatTargetLocation(TargetLocation.X, TargetLocation.Y, MyLocation.Z);

	const FRotator LookAtRot = (FlatTargetLocation - MyLocation).Rotation();
	SetActorRotation(LookAtRot);
}*/