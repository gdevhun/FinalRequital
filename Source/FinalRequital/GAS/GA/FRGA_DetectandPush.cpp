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
#include "Actor/FRPushableActor.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Character/FRMonsterBase.h"
#include "Character/FRSoul.h"

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
    FVector End = Start + Direction * 700.0f;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(SourceCharacter);

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

    if (bHit && Hit.GetActor())
    {
        AActor* HitActor = Hit.GetActor();

        FVector PushDir = Direction.GetSafeNormal();
        PushDir.Z += 0.5f;
        PushDir.Normalize();

        // AFRMonsterBase
        if (AFRMonsterBase* Monster = Cast<AFRMonsterBase>(HitActor))
        {
            ACharacter* HitCharacter = Monster;
            UCharacterMovementComponent* MoveComp = HitCharacter->GetCharacterMovement();
            UAnimInstance* AnimInstance = HitCharacter->GetMesh()->GetAnimInstance();
            AAIController* AIController = Cast<AAIController>(HitCharacter->GetController());

            if (AIController)
            {
                AIController->StopMovement();
                if (UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(AIController->BrainComponent))
                {
                    BTComp->StopTree(EBTStopMode::Safe);
                }
            }

            if (AnimInstance)
            {
                AnimInstance->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
            }

            if (MoveComp)
            {
                MoveComp->SetMovementMode(MOVE_Walking);
            }

            HitCharacter->LaunchCharacter(PushDir * PushStrength, true, true);

            FTimerHandle TimerHandle;
            FTimerDelegate Delegate;

            Delegate.BindLambda([=]()
                {
                    if (MoveComp)
                    {
                        MoveComp->SetMovementMode(MOVE_Walking);
                    }

                    if (AnimInstance)
                    {
                        AnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
                    }

                    if (AIController)
                    {
                        if (UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(AIController->BrainComponent))
                        {
                            BTComp->RestartTree();
                        }
                    }
                });

            GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, 0.8f, false);
        }
        else if (AFRSoul* Soul = Cast<AFRSoul>(HitActor))
        {
            // Soul 단순 LaunchCharacter 처리만
            FVector SoulPushDir = Direction.GetSafeNormal();
            SoulPushDir.Z += 0.5f;
            SoulPushDir.Normalize();

            Soul->LaunchCharacter(SoulPushDir * PushStrength, true, true);
        }
        // AFRPushableActor
        else if (AFRPushableActor* Pushable = Cast<AFRPushableActor>(HitActor))
        {
            if (UStaticMeshComponent* Mesh = Pushable->StaticMesh)
            {
                // 물리 시뮬레이션 일시 활성화
                Mesh->SetSimulatePhysics(true);
                Mesh->AddImpulse(PushDir * PushStrength * 0.5f, NAME_None, true);
                
                // 일정 시간 후 물리 시뮬레이션 비활성화 (낙하 완료 후)
                FTimerHandle DisablePhysicsHandle;
                FTimerDelegate DisablePhysicsDelegate;

                DisablePhysicsDelegate.BindLambda([=]()
                    {
                        if (Mesh)
                        {
                            Mesh->SetSimulatePhysics(false);
                            Pushable->bIsTutorialObj = true;
                        }
                    });

                // 2.5초 뒤 비활성화
                Mesh->GetWorld()->GetTimerManager().SetTimer(DisablePhysicsHandle, DisablePhysicsDelegate, 2.5f, false);
            }
        }

        // GameplayCue 실행 
        if (UAbilitySystemComponent* TargetASC = HitActor->FindComponentByClass<UAbilitySystemComponent>())
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

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
