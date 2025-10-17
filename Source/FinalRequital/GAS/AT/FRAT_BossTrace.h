// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "FRAT_BossTrace.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBossTraceResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);
/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRAT_BossTrace : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFRAT_BossTrace();

	static UFRAT_BossTrace* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AFRTA_BossTrace> TargetActorClass);
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

protected:
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

public:
	FBossTraceResultDelegate OnComplete;

protected:
	UPROPERTY()
	TSubclassOf<class AFRTA_BossTrace> TargetActorClass;

	UPROPERTY()
	TObjectPtr<class AFRTA_BossTrace> SpawnedTargetActor;
};
