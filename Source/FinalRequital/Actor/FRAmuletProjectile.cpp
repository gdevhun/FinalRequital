// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FRAmuletProjectile.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "FRDebugHelper.h"
#include "FRGameplayTag.h"
#include "Character/FRMonsterBase.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameplayCueManager.h" 
#include "Kismet/GameplayStatics.h"

AFRAmuletProjectile::AFRAmuletProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	CollisionComponent->SetGenerateOverlapEvents(false);
	CollisionComponent->OnComponentHit.AddDynamic(this, &AFRAmuletProjectile::OnHit);
	RootComponent = CollisionComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmuletMesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetRelativeLocation(FVector(15.f, 0.f, 0.f)); 
	Mesh->SetRelativeRotation(FRotator(-90.f, 90.f, 90.f));

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);
	ArrowComponent->SetRelativeLocation(FVector::ZeroVector);
	ArrowComponent->SetRelativeRotation(FRotator::ZeroRotator);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bInitialVelocityInLocalSpace = false;

}

void AFRAmuletProjectile::BeginPlay()
{
	Super::BeginPlay();
}


void AFRAmuletProjectile::InitVelocity(const FVector& Direction, float Speed)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = Direction * Speed;
	}
}

void AFRAmuletProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == GetOwner())
	{
		D(FString::Printf(TEXT("TRIGGER2!")));
		Destroy();
		return;
	}

	const FVector HitLocation = Hit.ImpactPoint;
	const FVector HitNormal = Hit.Normal;

	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);

	AFRMonsterBase* Monster = Cast<AFRMonsterBase>(OtherActor);
	if (Monster && SourceASC && TargetASC && DamageEffectClass)
	{
		FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
		EffectContext.AddHitResult(Hit);
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.f, EffectContext);
		if (SpecHandle.IsValid())
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

			FGameplayCueParameters CueParams;
			CueParams.EffectContext = SpecHandle.Data->GetEffectContext();
			CueParams.Location = HitLocation;
			CueParams.Normal = HitNormal;
			ActiveEffect();
			//TargetASC->ExecuteGameplayCue(GAMEPLAYCUE_CHARACTER_AMULETDOTHIT, CueParams);
			TargetASC->AddGameplayCue(GAMEPLAYCUE_CHARACTER_AMULETDOTHIT, CueParams);
		}

		Destroy();
		return;
	}

	// ASC가 없거나 몬스터가 아닌 경우
	ActiveEffect();
	Destroy();
}

void AFRAmuletProjectile::ActiveEffect() const
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
