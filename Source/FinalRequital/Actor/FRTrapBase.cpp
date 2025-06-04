// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FRTrapBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AFRTrapBase::AFRTrapBase()
{
    TrapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrapMesh"));
    RootComponent = TrapMesh;

    ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(TrapMesh);
    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
    CollisionBox->SetGenerateOverlapEvents(true);

    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AFRTrapBase::OnTrapTriggered);
    CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AFRTrapBase::OnTrapEndOverlap);
}

void AFRTrapBase::BeginPlay()
{
    Super::BeginPlay();

    if (ASC)
    {
        ASC->InitAbilityActorInfo(this, this);
    }
    GetWorldTimerManager().SetTimer(DamageTickTimer, this, &AFRTrapBase::DealDamageToOverlappingActors, DamageInterval, true);
}

class UAbilitySystemComponent* AFRTrapBase::GetAbilitySystemComponent() const
{
    return ASC;
}

void AFRTrapBase::OnTrapTriggered(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;
    OverlappingActors.Add(OtherActor);
}

void AFRTrapBase::OnTrapEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    OverlappingActors.Remove(OtherActor);
}

void AFRTrapBase::DealDamageToOverlappingActors()
{
    if (!bIsTrapActive || !ASC || !DamageEffectClass) return;

    for (AActor* Target : OverlappingActors)
    {
        if (IsValid(Target))
        {
            ApplyTrapEffectToActor(Target);
        }
    }
}

void AFRTrapBase::ApplyTrapEffectToActor(AActor* TargetActor)
{
    if (!DamageEffectClass || !TargetActor || !ASC) return;

    ActivateTrap();

    UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
    if (!TargetASC) return;

    FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
    EffectContext.AddSourceObject(this);

    FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DamageEffectClass, 1.f, EffectContext);
    if (SpecHandle.IsValid())
    {
        ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
    }
}

void AFRTrapBase::ActivateTrap_Implementation()
{
    bIsTrapActive = true;
    // 여기서 애니메이션, VFX, SFX 등을 활성화할 수 있음
}

void AFRTrapBase::DeactivateTrap_Implementation()
{
    bIsTrapActive = false;
    // 여기서 애니메이션, VFX, SFX 등을 비활성화할 수 있음
}
