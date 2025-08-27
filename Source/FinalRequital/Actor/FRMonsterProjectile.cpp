// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FRMonsterProjectile.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "FRGameplayTag.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/FRGASCharacterPlayer.h"

AFRMonsterProjectile::AFRMonsterProjectile()
{

	PrimaryActorTick.bCanEverTick = false;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	CollisionComponent->SetGenerateOverlapEvents(false);
	CollisionComponent->OnComponentHit.AddDynamic(this, &AFRMonsterProjectile::OnHit);
	RootComponent = CollisionComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetRelativeLocation(FVector(15.f, 0.f, 0.f));
	Mesh->SetRelativeRotation(FRotator(-90.f, 90.f, 90.f));

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);
	ArrowComponent->SetRelativeLocation(FVector::ZeroVector);
	ArrowComponent->SetRelativeRotation(FRotator::ZeroRotator);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 750.f;
	ProjectileMovement->MaxSpeed = 750.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bInitialVelocityInLocalSpace = false;

}

void AFRMonsterProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(5.0f);
}

void AFRMonsterProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == GetOwner())
	{
		Destroy();
		return;
	}
	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);

	AFRGASCharacterPlayer* TargetPlayer = Cast<AFRGASCharacterPlayer>(OtherActor);
	if (TargetPlayer && SourceASC && TargetASC && DamageEffectClass)
	{
		FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
		EffectContext.AddHitResult(Hit);
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.f, EffectContext);
		if (SpecHandle.IsValid())
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

			ActiveEffect();
		}

		Destroy();
		return;
	}

	// ASC가 없거나 플레이어가 아닌 경우
	ActiveEffect();
	Destroy();
}

void AFRMonsterProjectile::ActiveEffect() const
{
	if (ImpactParticleEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticleEffect, GetActorLocation(), GetActorRotation());
	}
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
	}
}



