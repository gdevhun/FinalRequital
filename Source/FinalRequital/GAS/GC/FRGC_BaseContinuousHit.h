// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "FRGC_BaseContinuousHit.generated.h"

/**
 *  GameplayCueNotify_Actor 기반 - 인스턴스화 되어 라이프사이클식 이펙트
 */
UCLASS()
class FINALREQUITAL_API AFRGC_BaseContinuousHit : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	AFRGC_BaseContinuousHit();

	// GameplayCue가 활성화될 때 호출됨 (처음 시작 시 한 번)
	virtual bool OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

	// GameplayCue가 활성 상태로 유지되는 동안 주기적으로 호출됨 (Tick 느낌)
	virtual bool WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

	// GameplayCue가 제거될 때 호출됨 (종료 시 한 번)
	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UParticleSystemComponent> ParticleComponent;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UParticleSystem> ParticleTemplate;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<USoundBase> LoopingSound;
};
