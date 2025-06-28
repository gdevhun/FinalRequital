// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA_Skill/FRGA_GyeokSkillTrigger.h"

#include "Actor/FRAmuletProjectile.h"
#include "Camera/CameraComponent.h"
#include "Character/FRCharacterBase.h"
#include "Components/SphereComponent.h"

UFRGA_GyeokSkillTrigger::UFRGA_GyeokSkillTrigger()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFRGA_GyeokSkillTrigger::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	SpawnAmulet(FireSpeedPower);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UFRGA_GyeokSkillTrigger::SpawnAmulet(float InAmuletSpeed)
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor || !AmuletClass) return;

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
	FVector MuzzleLocation = Mesh->GetSocketLocation("bow_socket");
	FVector FireDirection = (TargetPoint - MuzzleLocation).GetSafeNormal();
	FRotator FireRotation = FireDirection.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = AvatarActor;
	SpawnParams.Instigator = Character;

	AFRAmuletProjectile* Arrow = GetWorld()->SpawnActor<AFRAmuletProjectile>(
		AmuletClass, MuzzleLocation, FireRotation, SpawnParams);

	if (Arrow)
	{
		Arrow->InitVelocity(FireDirection, FireSpeedPower);
		Arrow->GetCollisionComponent()->IgnoreActorWhenMoving(AvatarActor, true);
	}

	DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, FColor::Red, false, 2.0f);
	DrawDebugLine(GetWorld(), MuzzleLocation, TargetPoint, FColor::Green, false, 2.0f);
}
