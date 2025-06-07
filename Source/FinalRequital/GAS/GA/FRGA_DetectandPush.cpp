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
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "FRDebugHelper.h"
#include "GameplayEffectTypes.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

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
    FVector End = Start + Direction * 1000.0f;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(SourceCharacter);

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

    if (bHit && Hit.GetActor())
    {
        ACharacter* HitCharacter = Cast<ACharacter>(Hit.GetActor());

        if (HitCharacter && HitCharacter != SourceCharacter)
        {
            // 넉백 방향 계산
            FVector PushDir = Direction.GetSafeNormal();
            PushDir.Z += 0.5f;
            PushDir.Normalize();

            // 컴포넌트 참조
            UCharacterMovementComponent* MoveComp = HitCharacter->GetCharacterMovement();
            UAnimInstance* AnimInstance = HitCharacter->GetMesh()->GetAnimInstance();
            AAIController* AIController = Cast<AAIController>(HitCharacter->GetController());

            // AI 멈춤
            if (AIController)
            {
                AIController->StopMovement();
                UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(AIController->BrainComponent);
                if (BTComp)
                {
                    BTComp->StopTree(EBTStopMode::Safe);
                }
            }

            // RootMotion 무시 (선택적)
            if (AnimInstance)
            {
                AnimInstance->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
            }

            // MovementMode Walking 보장
            if (MoveComp)
            {
                MoveComp->SetMovementMode(MOVE_Walking);
            }

            // LaunchCharacter 호출
            HitCharacter->LaunchCharacter(PushDir * PushStrength, true, true);

            // 일정 시간 뒤 AI 재시작
            float RecoveryDelay = 0.8f;
            FTimerHandle RecoveryTimerHandle;
            FTimerDelegate RecoveryDelegate;

            RecoveryDelegate.BindLambda([=]()
                {
                    // MovementMode 복구
                    if (MoveComp)
                    {
                        MoveComp->SetMovementMode(MOVE_Walking);
                    }

                    // RootMotion 복구
                    if (AnimInstance)
                    {
                        AnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
                    }

                    // AI 다시 시작
                    if (AIController)
                    {
                        UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(AIController->BrainComponent);
                        if (BTComp)
                        {
                            BTComp->RestartTree();
                        }
                    }

                });

            HitCharacter->GetWorldTimerManager().SetTimer(RecoveryTimerHandle, RecoveryDelegate, RecoveryDelay, false);

            // GameplayCue (이펙트) 실행
            UAbilitySystemComponent* TargetASC = HitCharacter->FindComponentByClass<UAbilitySystemComponent>();
            if (TargetASC)
            {
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
        }
    }



    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
