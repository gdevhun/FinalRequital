// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/Monster/FRGA_MonsterRangeAttackTrigger.h"
#include "FRDebugHelper.h"
#include "Actor/FRMonsterProjectile.h"
#include "Character/FRMonsterBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UFRGA_MonsterRangeAttackTrigger::UFRGA_MonsterRangeAttackTrigger()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFRGA_MonsterRangeAttackTrigger::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AFRMonsterBase* Monster = Cast<AFRMonsterBase>(ActorInfo->AvatarActor.Get());
	if (!Monster) return;

	//const FName SocketName = TEXT("SpawnPos");
	const FName SocketName = TEXT("Head");
	const FVector SpawnLocation = Monster->GetMesh()->GetSocketLocation(SocketName);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(Monster->GetWorld(), 0);
	if (!PlayerPawn) return;

	FVector Direction = (PlayerPawn->GetActorLocation() - SpawnLocation).GetSafeNormal();
	FRotator SpawnRotation = Direction.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Monster;
	SpawnParams.Instigator = Monster;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AFRMonsterProjectile* MonsterProjectile = GetWorld()->SpawnActor<AFRMonsterProjectile>(
		MonsterProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (MonsterProjectile && Monster)
	{
		MonsterProjectile->CollisionComponent->IgnoreActorWhenMoving(Monster, true);
		MonsterProjectile->ProjectileMovement->Velocity = Direction * MonsterProjectile->ProjectileMovement->InitialSpeed;
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
