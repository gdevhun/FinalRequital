// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AN_GASMeleeAttackHitCheck.generated.h"

/**
 * UFRGA_MeleeAttackHitCheck
   └▶ UFRAT_Trace (AbilityTask)
          └▶ AFRTA_Trace (TargetActor)
                └▶ TargetData 생성 + Broadcast
          └◀ OnTargetDataReadyCallback → OnComplete.Broadcast
   ◀── OnTraceResultCallback(DataHandle)
 */
UCLASS()
class FINALREQUITAL_API UAN_GASMeleeAttackHitCheck : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAN_GASMeleeAttackHitCheck();

protected:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere)
	FGameplayTag TriggerGameplayTag;

	UPROPERTY(EditAnywhere)
	float ComboAttackLevel;
};
