// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FRArrowProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemGlobals.h"
#include "FRDebugHelper.h"
#include "FRGameplayTag.h"
#include "Character/FRBossAsura.h"
#include "Character/FRMonsterBase.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"

AFRArrowProjectile::AFRArrowProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	CollisionComponent->SetGenerateOverlapEvents(false);
	CollisionComponent->OnComponentHit.AddDynamic(this, &AFRArrowProjectile::OnHit);
	RootComponent = CollisionComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);
	ArrowComponent->SetRelativeLocation(FVector::ZeroVector);
	ArrowComponent->SetRelativeRotation(FRotator::ZeroRotator);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.5f;
	ProjectileMovement->bInitialVelocityInLocalSpace = false;

}

void AFRArrowProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AFRArrowProjectile::InitVelocity(const FVector& Direction, float Speed, float ProjectileGravity)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = Direction * Speed;
		ProjectileMovement->ProjectileGravityScale = ProjectileGravity;
	}
}

void AFRArrowProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (!OtherActor || OtherActor == GetOwner())
	{
		Destroy();
		return;
	}

	// 타격 위치, 방향
	const FVector HitLocation = Hit.ImpactPoint;
	const FRotator HitRotation = Hit.Normal.Rotation();

	// ASC 가져오기
	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);

	AFRMonsterBase* Monster = Cast<AFRMonsterBase>(OtherActor);
	AFRBossAsura* Boss = Cast<AFRBossAsura>(OtherActor);
	if ((Monster || Boss) && SourceASC && TargetASC && DamageEffectClass)
	{
		// 몬스터 전용 처리 (GE + GC + 즉시 파괴)
		FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
		EffectContext.AddHitResult(Hit);
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.f, EffectContext);
		if (SpecHandle.IsValid())
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

			FGameplayCueParameters CueParameters;
			CueParameters.EffectContext = SpecHandle.Data->GetEffectContext();
			CueParameters.Location = Hit.ImpactPoint;
			CueParameters.Normal = Hit.Normal;

			TargetASC->ExecuteGameplayCue(GAMEPLAYCUE_CHARACTER_ARROWATTACKHIT, CueParameters);
			
		}

		Destroy();
		return;
	}

	// ASC 없는 대상: 화살을 박아두고 일정 시간 후 제거

	// 충돌 및 이동 중지
	ProjectileMovement->StopMovementImmediately();
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 메시를 충돌 대상에 붙여서 박히는 효과
	Mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	Mesh->AttachToComponent(HitComponent, FAttachmentTransformRules::KeepWorldTransform);
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 3초 후 제거
	SetLifeSpan(3.0f);
}
