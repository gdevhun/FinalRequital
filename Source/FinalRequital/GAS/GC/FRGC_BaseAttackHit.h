// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "FRGC_BaseAttackHit.generated.h"

/**
 *  GameplayCueNotify_Static 기반 - 일회성 이펙트 처리용
 */
UCLASS()
class FINALREQUITAL_API UFRGC_BaseAttackHit : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	UFRGC_BaseAttackHit();

	// 단발성 GameplayCue가 호출될 때 실행됨.
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayCue)
	TObjectPtr<UParticleSystem> ParticleEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayCue)
	TObjectPtr<USoundBase> HitSound;
};
