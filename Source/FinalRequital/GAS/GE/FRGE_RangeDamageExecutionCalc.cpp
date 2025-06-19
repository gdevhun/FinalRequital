// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GE/FRGE_RangeDamageExecutionCalc.h"
#include "GAS/Attribute/FRCharacterAttributeSet.h"
#include "GAS/Attribute/FRMonsterAttributeSet.h"


void UFRGE_RangeDamageExecutionCalc::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	
	if (SourceASC && TargetASC)
	{
		AActor* SourceActor = SourceASC->GetAvatarActor();
		AActor* TargetActor = TargetASC->GetAvatarActor();
		if (SourceActor && TargetActor)
		{
			const float MaxDamageRange = 350.0f;
			const float BaseDamage = SourceASC->GetNumericAttributeBase(UFRCharacterAttributeSet::GetAttackRateAttribute());
			const float Distance = FMath::Clamp(SourceActor->GetDistanceTo(TargetActor), 0.0f, MaxDamageRange);

			// 거리 비율 계산 (0.0 = 가장 가까움, 1.0 = 최대 거리)
			const float DistanceRatio = Distance / MaxDamageRange;

			// 데미지 배율 계산: 가까울 때 3.5배, 멀 때 1.0배
			// 3.5 - (3.5 - 1.0) * DistanceRatio = 3.5 - 2.5 * DistanceRatio
			const float DamageMultiplier = 3.5f - 2.5f * DistanceRatio;

			float Damage = BaseDamage * DamageMultiplier;

			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
				UFRMonsterAttributeSet::GetReceivedMonsterDamageAttribute(),
				EGameplayModOp::Additive,
				Damage));
		}
	}

}
