// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "FRTA_BossTrace.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API AFRTA_BossTrace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
public:
	AFRTA_BossTrace();

	//즉각적인 검출만 수행하는 타겟 엑터

	//타게팅 시작함수
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	//타게팅 확정하고 남은 프로세스 진행 함수
	virtual void ConfirmTargetingAndContinue() override;
	void SetShowDebug(bool InShowDebug) { bShowDebug = InShowDebug; }

protected:
	// 데이터들을 가지고 있는 핸들
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;

	bool bShowDebug = false;
};
