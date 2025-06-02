// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "FRGC_BaseAttackHit.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRGC_BaseAttackHit : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	UFRGC_BaseAttackHit();

	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

protected:

	// 할당할 파티클 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayCue)
	TObjectPtr<UParticleSystem> ParticleEffect;

	// 타격 사운드
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayCue)
	TObjectPtr<USoundBase> HitSound;
};
