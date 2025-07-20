// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "FRAT_MonsterTrace.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMonsterTraceResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);
/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRAT_MonsterTrace : public UAbilityTask
{
	GENERATED_BODY()


public:
	UFRAT_MonsterTrace();

	static UFRAT_MonsterTrace* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AFRTA_MonsterTrace> TargetActorClass);
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

protected:
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

public:
	FMonsterTraceResultDelegate OnComplete;

protected:
	UPROPERTY()
	TSubclassOf<class AFRTA_MonsterTrace> TargetActorClass;

	UPROPERTY()
	TObjectPtr<class AFRTA_MonsterTrace> SpawnedTargetActor;
};
