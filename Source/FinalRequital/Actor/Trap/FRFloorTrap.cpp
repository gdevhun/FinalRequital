// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Trap/FRFloorTrap.h"
#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Player/FRGASCharacterPlayer.h"
#include "FRDebugHelper.h"
#include "Character/FRMonsterBase.h"

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
    CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
    CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
    CollisionBox->SetGenerateOverlapEvents(true);
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AFRFloorTrap::OnOverlapBegin);

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

    TrapActiveInterval = UFRTrapIntervalHelper::GetIntervalSeconds(TrapIntervalType);

    GetWorld()->GetTimerManager().SetTimer(
        ToggleActiveTimer,
        this,
        &AFRFloorTrap::ToggleTrapActive,
        TrapActiveInterval,
        true,
        StartDelay
    );
}

void AFRFloorTrap::ToggleTrapActive()
{
    bIsTrapActive = !bIsTrapActive;

    if (bIsTrapActive)
    {
        CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        // D(TEXT("Trap Activated"));
    }
    else
    {
        CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
       // D(TEXT("Trap Deactivated"));
    }
}

void AFRFloorTrap::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || !bIsTrapActive) return;

    for (ACharacter* Character : OverlappingCharacters)
    {
        if (!Character) continue;

        UAbilitySystemComponent* TargetASC = nullptr;
        TSubclassOf<UGameplayEffect> EffectClass = nullptr;
        TSubclassOf<UGameplayAbility> AbilityClass = nullptr;

        // Player Check
        if (AFRGASCharacterPlayer* Player = Cast<AFRGASCharacterPlayer>(Character))
        {
            TargetASC = Player->GetAbilitySystemComponent();
            EffectClass = PlayerDamageEffectClass;
            AbilityClass = PlayerHitAbilityClass;
        }
        // Monster
        else if (AFRMonsterBase* Monster = Cast<AFRMonsterBase>(Character))
        {
            TargetASC = Monster->GetAbilitySystemComponent();
            EffectClass = MonsterDamageEffectClass;
            AbilityClass = MonsterHitAbilityClass;
        }

        if (TargetASC)
        {
            //  Apply Damage Effect
            if (EffectClass)
            {
                FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
                EffectContext.AddSourceObject(this);

                FGameplayEffectSpecHandle EffectSpec = ASC->MakeOutgoingSpec(EffectClass, 1.0f, EffectContext);
                if (EffectSpec.IsValid())
                {
                    ASC->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data.Get(), TargetASC);
                }
            }

            // Trigger Ability
            if (AbilityClass)
            {
                TargetASC->TryActivateAbilityByClass(AbilityClass);
            }
        }
    }
}