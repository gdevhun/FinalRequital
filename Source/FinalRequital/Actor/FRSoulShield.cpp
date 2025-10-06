// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FRSoulShield.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "FRDebugHelper.h"
#include "FRGameplayTag.h"
#include "Character/FRMonsterBase.h"
#include "Components/SphereComponent.h"
#include "GAS/Attribute/FRSoulShieldAttributeSet.h"
#include "UI/FRSoulShieldHpWidget.h"

class UAbilitySystemComponent* AFRSoulShield::GetAbilitySystemComponent() const
{
	return ASC;
}

AFRSoulShield::AFRSoulShield()
{
    PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	DamageDetectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DamageDetectSphere"));
	DamageDetectSphere->SetupAttachment(RootComponent);

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

	AttributeSet = CreateDefaultSubobject<UFRSoulShieldAttributeSet>(TEXT("AttributeSet"));
   
}

void AFRSoulShield::BeginPlay()
{
	Super::BeginPlay();
	ASC->InitAbilityActorInfo(this, this);

	// overlap event
	if (DamageDetectSphere)
	{
		DamageDetectSphere->OnComponentBeginOverlap.AddDynamic(this, &AFRSoulShield::OnSphereBeginOverlap);
	}

	AttributeSet->OnOutOfShield.AddDynamic(this, &AFRSoulShield::DestroyShield);

}

void AFRSoulShield::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || !ASC) return;

	AFRMonsterBase* Monster = Cast<AFRMonsterBase>(OtherActor);
	if (!Monster) return;

	//  GameplayCue 발생 
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Monster);
	if (TargetASC)
	{
		FGameplayCueParameters CueParams;
		CueParams.Location = Monster->GetActorLocation();
		TargetASC->ExecuteGameplayCue(GAMEPLAYCUE_CHARACTER_SOULATTACKHIT, CueParams);
		D("Trigger!")
	}

	//  몬스터 제거
	Monster->Destroy();

	//  Shield / Health Effect 적용
	TSubclassOf<UGameplayEffect> EffectToApply = IsShieldDestroyed ? HpDamageEffectClass : ShieldDamageEffectClass;
	if (!EffectToApply) return;

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectToApply, 1.f, EffectContext);
	if (SpecHandle.IsValid())
	{
		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	}
}

