// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/FRAT_BossTrace.h"
#include "AbilitySystemComponent.h"
#include "GAS/TA/FRTA_BossTrace.h"


UFRAT_BossTrace::UFRAT_BossTrace()
{
}

UFRAT_BossTrace* UFRAT_BossTrace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<AFRTA_BossTrace> TargetActorClass)
{
	UFRAT_BossTrace* NewTask = NewAbilityTask<UFRAT_BossTrace>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

void UFRAT_BossTrace::Activate()
{
	Super::Activate();

	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UFRAT_BossTrace::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);

	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}
}

void UFRAT_BossTrace::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<AFRTA_BossTrace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>
		(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetShowDebug(true);

		// 스폰액터에 데이타가 레디되면 호출할 콜백 델리게이트 등록
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UFRAT_BossTrace::OnTargetDataReadyCallback);
	}
}

void UFRAT_BossTrace::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();

	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);
		SpawnedTargetActor->ConfirmTargetingAndContinue();
	}
}

void UFRAT_BossTrace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		// OnComplete 델리게이트는 다시 밀리어택히트체크 게임 어빌리티에 데이터핸들 정보 전달
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
