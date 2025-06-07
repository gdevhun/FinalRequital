// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/FRGA_SpawnArrow.h"
#include "Actor/FRArrowProjectile.h"
#include "Camera/CameraComponent.h"
#include "Character/FRCharacterBase.h"
#include "FRDebugHelper.h"
#include "Components/SphereComponent.h"

UFRGA_SpawnArrow::UFRGA_SpawnArrow()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFRGA_SpawnArrow::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	SpawnAndFireArrow(BasicFirePower, BasicProjectileGravityPower);

	// Ability 즉시 종료
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UFRGA_SpawnArrow::SpawnAndFireArrow(float InFirePower, float InProjectileGravityPower)
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor || !ArrowClass) return;

	AFRCharacterBase* Character = Cast<AFRCharacterBase>(AvatarActor);
	if (!Character) return;

	UCameraComponent* CameraComp = Character->FindComponentByClass<UCameraComponent>();
	USkeletalMeshComponent* Mesh = Character->GetMesh();
	if (!CameraComp || !Mesh) return;

	FVector CameraLocation = CameraComp->GetComponentLocation();
	FVector CameraDirection = CameraComp->GetForwardVector();
	FVector TraceEnd = CameraLocation + CameraDirection * 3000.0f;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);
	GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECC_Visibility, Params);

	FVector TargetPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	FVector MuzzleLocation = Mesh->GetSocketLocation("Bow_socket");
	FVector FireDirection = (TargetPoint - MuzzleLocation).GetSafeNormal();
	FRotator FireRotation = FireDirection.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = AvatarActor;
	SpawnParams.Instigator = Character;

	AFRArrowProjectile* Arrow = GetWorld()->SpawnActor<AFRArrowProjectile>(
		ArrowClass, MuzzleLocation, FireRotation, SpawnParams);

	if (Arrow)
	{
		Arrow->InitVelocity(FireDirection, InFirePower, InProjectileGravityPower);
		Arrow->GetCollisionComponent()->IgnoreActorWhenMoving(AvatarActor, true);
	}
	DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, FColor::Red, false, 2.0f);
	DrawDebugLine(GetWorld(), MuzzleLocation, TargetPoint, FColor::Green, false, 2.0f);
}
