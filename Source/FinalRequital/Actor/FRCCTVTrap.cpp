// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FRCCTVTrap.h"
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
	const int NumRays = 10;

	for (int i = 0; i < NumRays; ++i)
	{
		FVector Direction = FMath::VRandCone(Forward, ConeAngle);
		FVector End = Start + Direction * MaxDistance;

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
		{
			AActor* HitActor = Hit.GetActor();
			if (AFRGASCharacterPlayer* Player = Cast<AFRGASCharacterPlayer>(HitActor))
			{
				if (HitActor->Implements<UAbilitySystemInterface>())
				{
					IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(HitActor);
					if (AbilityInterface)
					{
						UAbilitySystemComponent* TargetASC = AbilityInterface->GetAbilitySystemComponent();
						if (TargetASC && DamageEffectClass)
						{
							// 이미 스턴 상태가 아닌 경우만 이펙트 적용
							if (!TargetASC->HasMatchingGameplayTag(FRTAG_CHARACTER_STUNNED))
							{
								FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
								EffectContext.AddSourceObject(this);

								FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, EffectContext);
								if (SpecHandle.IsValid())
								{
									ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
									UE_LOG(LogTemp, Warning, TEXT("스턴 이펙트 적용됨: %s"), *HitActor->GetName());
								}
							}
							else
							{
								UE_LOG(LogTemp, Warning, TEXT("이미 스턴 상태: %s"), *HitActor->GetName());
							}
						}
					}
				}
			}

#if WITH_EDITOR
			DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.15f);
#endif
		}
	}
}