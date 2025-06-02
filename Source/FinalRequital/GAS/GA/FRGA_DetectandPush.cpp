// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/FRGA_DetectandPush.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Character/FRCharacterBase.h"
#include "DrawDebugHelpers.h"
#include "FRGameplayTag.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "FRDebugHelper.h"
#include "GameplayEffectTypes.h"

UFRGA_DetectandPush::UFRGA_DetectandPush()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFRGA_DetectandPush::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    AActor* AvatarActor = GetAvatarActorFromActorInfo();
    AFRCharacterBase* SourceCharacter = Cast<AFRCharacterBase>(AvatarActor);
    if (!SourceCharacter) return;

    UCameraComponent* CameraComp = SourceCharacter->FindComponentByClass<UCameraComponent>();
    if (!CameraComp) return;

    FVector Start = CameraComp->GetComponentLocation();
    FVector Direction = CameraComp->GetForwardVector();
    FVector End = Start + Direction * 3000.0f;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(SourceCharacter);

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

    if (bHit && Hit.GetActor())
    {

        ACharacter* HitCharacter = Cast<ACharacter>(Hit.GetActor());

        if (HitCharacter && HitCharacter != SourceCharacter)
        {
 
            UAbilitySystemComponent* TargetASC = HitCharacter->FindComponentByClass<UAbilitySystemComponent>();
            if (TargetASC)
            {
                // GameplayCue 실행용 컨텍스트 구성
                FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
                EffectContext.AddHitResult(Hit);
                EffectContext.AddSourceObject(this);

                FGameplayCueParameters CueParams;
                CueParams.EffectContext = EffectContext;
                CueParams.Location = Hit.ImpactPoint;
                CueParams.Instigator = SourceCharacter;
                CueParams.EffectCauser = SourceCharacter;
                CueParams.SourceObject = this;

                TargetASC->ExecuteGameplayCue(GAMEPLAYCUE_CHARACTER_BRONZEBELLHIT, CueParams);
            }

            // 넉백
            FVector PushDir = Direction;
            HitCharacter->LaunchCharacter(PushDir * PushStrength, true, true);
        }
    }


    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
