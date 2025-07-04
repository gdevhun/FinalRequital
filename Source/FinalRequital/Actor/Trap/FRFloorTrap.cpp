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

    GetWorld()->GetTimerManager().SetTimer(
        ToggleActiveTimer,
        this,
        &AFRFloorTrap::ToggleTrapActive,
        TrapCooldownTime,
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

    if (AFRGASCharacterPlayer* Player = Cast<AFRGASCharacterPlayer>(OtherActor))
    {
        if (UAbilitySystemComponent* TargetASC = Player->GetAbilitySystemComponent())
        {
            // 데미지 적용
            if (PlayerDamageEffectClass)
            {
                FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
                EffectContext.AddSourceObject(this);

                FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(PlayerDamageEffectClass, 1.f, EffectContext);
                if (SpecHandle.IsValid())
                {
                    ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
                }
            }

            // 어빌리티 발동
            if (PlayerHitAbilityClass)
            {
                TargetASC->TryActivateAbilityByClass(PlayerHitAbilityClass);
            }
        }
       // D(TEXT("Trap Triggered by Player"));
    }

}