// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Trap/FRCCTVTrap.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SpotLightComponent.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystemComponent.h"
#include "Player/FRGASCharacterPlayer.h"
#include "FRGameplayTag.h"

AFRCCTVTrap::AFRCCTVTrap()
{
	PrimaryActorTick.bCanEverTick = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;

	CCTVMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CCTVMesh"));
	CCTVMesh->SetupAttachment(RootComponent);

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(RootComponent);

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

void AFRCCTVTrap::BeginPlay()
{
	Super::BeginPlay();
	if (ASC)
	{
		ASC->InitAbilityActorInfo(this, this);
	}
	
	GetWorld()->GetTimerManager().SetTimer(
		TraceTimerHandle,
		this,
		&AFRCCTVTrap::DetectByLineTrace,
		0.2f, 
		true  
	);
}

class UAbilitySystemComponent* AFRCCTVTrap::GetAbilitySystemComponent() const
{
	return ASC;
}

void AFRCCTVTrap::DetectByLineTrace()
{
	const FVector Start = SpotLight->GetComponentLocation();
	const FVector Forward = SpotLight->GetForwardVector();
	const float MaxDistance = SpotLight->AttenuationRadius;
	const float ConeAngle = FMath::DegreesToRadians(SpotLight->OuterConeAngle);
	const int NumRays = 15;

	for (int i = 0; i < NumRays; ++i)
	{
		const FVector Direction = FMath::VRandCone(Forward, ConeAngle);
		const FVector End = Start + Direction * MaxDistance;

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
		{
			AActor* HitActor = Hit.GetActor();
			if (!HitActor) continue;

			AFRGASCharacterPlayer* Player = Cast<AFRGASCharacterPlayer>(HitActor);
			if (!Player || !StunEffectClass) continue;

			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
			if (!TargetASC || TargetASC->HasMatchingGameplayTag(FRTAG_CHARACTER_STUNNED)
				|| TargetASC->HasMatchingGameplayTag(FRTAG_CHARACTER_INVISIBLE))
			{
				continue;
			}

			FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(StunEffectClass, 1.0f, EffectContext);
			if (SpecHandle.IsValid())
			{
				ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
			}

#if WITH_EDITOR
			DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.15f);
#endif
		}
	}
}
