// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Trap/FRFloorTrap.h"
#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Player/FRGASCharacterPlayer.h"
#include "FRDebugHelper.h"

AFRFloorTrap::AFRFloorTrap()
{
    PrimaryActorTick.bCanEverTick = false;

    // Scene Component Root
    USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = RootSceneComponent;

    // TrapMesh
    TrapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrapMesh"));
    TrapMesh->SetupAttachment(RootComponent);

    // CollisionBox
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(RootComponent);
    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
    CollisionBox->SetGenerateOverlapEvents(true);

    // ParticleSystem
    ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
    ParticleSystem->SetupAttachment(RootComponent);

    // ASC 
    ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

class UAbilitySystemComponent* AFRFloorTrap::GetAbilitySystemComponent() const
{
    return ASC;
}

void AFRFloorTrap::BeginPlay()
{
	Super::BeginPlay();
    if (ASC)
    {
        ASC->InitAbilityActorInfo(this, this);
    }

    GetWorld()->GetTimerManager().SetTimer(
        ToggleActiveTimer,
        this,
        &AFRFloorTrap::ToggleTrapActive,
        TrapCooldownTime,
        true,
        0.0f);
}

void AFRFloorTrap::ToggleTrapActive()
{
    bIsTrapActive = !bIsTrapActive;

    if (bIsTrapActive)
    {
        for (ACharacter* Character : OverlappingCharacters)
        {
            if (!Character) continue;

            // Player Check
            if (AFRGASCharacterPlayer* Player = Cast<AFRGASCharacterPlayer>(Character))
            {
                UAbilitySystemComponent* TargetASC = Player->GetAbilitySystemComponent();
                if (TargetASC)
                {
                    //  Apply Damage Effect
                    if (PlayerDamageEffectClass)
                    {
                        FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
                        EffectContext.AddSourceObject(this);

                        FGameplayEffectSpecHandle EffectSpec = ASC->MakeOutgoingSpec(PlayerDamageEffectClass, 1.0f, EffectContext);
                        if (EffectSpec.IsValid())
                        {
                            ASC->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data.Get(), TargetASC);
                        }
                    }

                    // Trigger Ability
                    if (PlayerHitAbilityClass)
                    {
                        TargetASC->TryActivateAbilityByClass(PlayerHitAbilityClass);
                    }
                }
            }

        }
    }
}

void AFRFloorTrap::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor) return;

    if (AFRGASCharacterPlayer* Player = Cast<AFRGASCharacterPlayer>(OtherActor))
    {
        if (!OverlappingCharacters.Contains(Player))
        {
            OverlappingCharacters.AddUnique(Player);
            D(FString::Printf(TEXT("TRIGGER!")));
        }
    }
}

void AFRFloorTrap::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    if (!OtherActor) return;

    if (AFRGASCharacterPlayer* Player = Cast<AFRGASCharacterPlayer>(OtherActor))
    {
        OverlappingCharacters.Remove(Player);
        D(FString::Printf(TEXT("TRIGGER2!")));
    }
}