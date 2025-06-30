// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Trap/FRFireHydrantTrap.h"
#include "AbilitySystemComponent.h"
#include "Character/FRMonsterBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystemComponent.h"
#include "Player/FRGASCharacterPlayer.h"

AFRFireHydrantTrap::AFRFireHydrantTrap()
{
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
    ParticleSystem->SetAutoActivate(false);

    // ASC 
    ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AFRFireHydrantTrap::OnOverlapBegin);
    CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AFRFireHydrantTrap::OnOverlapEnd);
}

void AFRFireHydrantTrap::BeginPlay()
{
	Super::BeginPlay();

    if (ASC)
    {
        ASC->InitAbilityActorInfo(this, this);
    }

    GetWorld()->GetTimerManager().SetTimer(
        ToggleActiveTimer,
        this, 
        &AFRFireHydrantTrap::ToggleTrapActive, 
        TrapCooldownTime,
        true, 
        0.0f);
}

void AFRFireHydrantTrap::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || !bIsTrapActive) return;

    if (ACharacter* Character = Cast<ACharacter>(OtherActor))
    {
        if (!OverlappingCharacters.Contains(Character))
        {
            OverlappingCharacters.AddUnique(Character);
        }
    }
}

void AFRFireHydrantTrap::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!OtherActor || !bIsTrapActive) return;

    if (ACharacter* Character = Cast<ACharacter>(OtherActor))
    {
        OverlappingCharacters.Remove(Character);
    }
}

void AFRFireHydrantTrap::ToggleTrapActive()
{
	bIsTrapActive = !bIsTrapActive;

    if (bIsTrapActive)
    {
        ParticleSystem->Activate(true);

        GetWorld()->GetTimerManager().SetTimer(
            DamageTickTimer,
            this,
            &AFRFireHydrantTrap::ApplyDamageEffect,
            0.5f,
            true
        );
    }
    else
    {
        ParticleSystem->Deactivate();
        GetWorld()->GetTimerManager().ClearTimer(DamageTickTimer);
    }
}

void AFRFireHydrantTrap::ApplyDamageEffect()
{
    for (ACharacter* Character : OverlappingCharacters)
    {
        if (!Character) continue;

        UAbilitySystemComponent* TargetASC = nullptr;
        TSubclassOf<UGameplayEffect> EffectClass = nullptr;

        // Player Check
        if (AFRGASCharacterPlayer* Player = Cast<AFRGASCharacterPlayer>(Character))
        {
            TargetASC = Player->GetAbilitySystemComponent();
            EffectClass = PlayerDamageEffectClass;
        }
        // Monster
        else if (AFRMonsterBase* Monster = Cast<AFRMonsterBase>(Character))
        {
            TargetASC = Monster->GetAbilitySystemComponent();
            EffectClass = MonsterDamageEffectClass;
        }

        // Apply GameplayEffect
        if (TargetASC && EffectClass)
        {
            FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
            EffectContext.AddSourceObject(this);

            FGameplayEffectSpecHandle EffectSpec = ASC->MakeOutgoingSpec(EffectClass, 1.0f, EffectContext);
            if (EffectSpec.IsValid())
            {
                ASC->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data.Get(), TargetASC);
            }
        }
    }
}

class UAbilitySystemComponent* AFRFireHydrantTrap::GetAbilitySystemComponent() const
{
    return ASC;
}


