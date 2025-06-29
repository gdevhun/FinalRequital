// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GC/FRGC_BaseContinuousHit.h"

#include "FRDebugHelper.h"
#include "kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AFRGC_BaseContinuousHit::AFRGC_BaseContinuousHit()
{
	PrimaryActorTick.bCanEverTick = false;
	bAutoDestroyOnRemove = true;  
	SetActorTickEnabled(false);
}

bool AFRGC_BaseContinuousHit::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	D(FString::Printf(TEXT("TRIGGER!1")));
	if (!MyTarget) return false;

	D(FString::Printf(TEXT("TRIGGER!")));
	// 파티클 컴포넌트 생성 및 타겟에 부착
	if (ParticleTemplate)
	{
		ParticleComponent = UGameplayStatics::SpawnEmitterAttached(
			ParticleTemplate,
			MyTarget->GetRootComponent(),
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}

	// 루프 사운드도 타겟에 부착 
	if (LoopingSound)
	{
		UGameplayStatics::SpawnSoundAttached(
			LoopingSound,
			MyTarget->GetRootComponent()
		);
	}

	return true;
}

bool AFRGC_BaseContinuousHit::WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	return true;
}

bool AFRGC_BaseContinuousHit::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (ParticleComponent)
	{
		ParticleComponent->DeactivateSystem();  // 부드럽게 종료
	}
	return true;
}
