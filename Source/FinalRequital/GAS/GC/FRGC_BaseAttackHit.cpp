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
	// HitResult 정보가 있는 경우 해당 ImpactPoint에 출력
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
	UE_LOG(LogTemp, Warning, TEXT(">>> GameplayCue Executed, Target: %s"), *GetNameSafe(Target));
	if (HitResult)
	{
		FVector SpawnLocation = HitResult->ImpactPoint;

		if (ParticleEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleEffect, SpawnLocation, FRotator::ZeroRotator, true);
		}

		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				Target,
				HitSound,
				SpawnLocation,
				FRotator::ZeroRotator,   // Rotation 
				1.0f,                    // Volume
				1.0f,                    // Pitch
				0.0f,                    // Start Time
				AttenuationSetting.Get() // AttenuationSettings
			);
		}
	}
	// 그렇지 않으면 EffectContext에서 가져온 액터 위치에 출력
	else if (Parameters.EffectContext.IsValid() && Parameters.EffectContext.Get())
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
					UGameplayStatics::PlaySoundAtLocation(
						Target,
						HitSound,
						SpawnLocation,
						FRotator::ZeroRotator,   // Rotation 
						1.0f,                    // Volume
						1.0f,                    // Pitch
						0.0f,                    // Start Time
						AttenuationSetting.Get() // AttenuationSettings
					);
				}
			}
		}
	}
	
	return Super::OnExecute_Implementation(Target, Parameters);
}
