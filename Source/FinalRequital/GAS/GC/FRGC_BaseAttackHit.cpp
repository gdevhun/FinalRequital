// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GC/FRGC_BaseAttackHit.h"
#include "Particles/ParticleSystem.h"
#include "kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

UFRGC_BaseAttackHit::UFRGC_BaseAttackHit()
{
}

bool UFRGC_BaseAttackHit::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();

	if (HitResult)
	{
		FVector SpawnLocation = HitResult->ImpactPoint;

		if (ParticleEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleEffect, SpawnLocation, FRotator::ZeroRotator, true);
		}

		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(Target, HitSound, SpawnLocation);
		}
	}
	else
	{
		TArray<TWeakObjectPtr<AActor>> TargetActors = Parameters.EffectContext.Get()->GetActors();

		for (const auto& TargetActor : TargetActors)
		{
			if (TargetActor.IsValid())
			{
				FVector SpawnLocation = TargetActor->GetActorLocation();

				if (ParticleEffect)
				{
					UGameplayStatics::SpawnEmitterAtLocation(TargetActor.Get(), ParticleEffect, SpawnLocation, FRotator::ZeroRotator, true);
				}

				if (HitSound)
				{
					UGameplayStatics::PlaySoundAtLocation(TargetActor.Get(), HitSound, SpawnLocation);
				}
			}
		}
	}

	return Super::OnExecute_Implementation(Target, Parameters);
}