// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GE/FRGE_ActionCooldownCalc.h"
#include "GAS/Attribute/FRCharacterAttributeSet.h"

UFRGE_ActionCooldownCalc::UFRGE_ActionCooldownCalc()
{
	// Mana 속성을 캡처하도록 설정
	Mana = FGameplayEffectAttributeCaptureDefinition(
		UFRCharacterAttributeSet::GetManaAttribute(),
		EGameplayEffectAttributeCaptureSource::Source,
		false
	);

	RelevantAttributesToCapture.Add(Mana);
}

float UFRGE_ActionCooldownCalc::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float ManaValue = 0.0f;
	GetCapturedAttributeMagnitude(Mana, Spec, EvaluationParameters, ManaValue);

	// Coefficient 값을 기본 쿨다운으로 사용 (블루프린트에서 설정)

	// Spec에서 ModifierMagnitude 정보를 가져와서 Coefficient 값 사용
	// 이 부분은 실제로는 블루프린트의 Coefficient에서 자동으로 곱해짐
	// 따라서 여기서는 계산만 하고 최종 결과에 Coefficient가 곱함

	// 기본 마나는 0이고, P 스탯이 1씩 증가할 때마다 마나가 1씩 증가
	const float PStat = FMath::Max(0.0f, ManaValue);

	// P 스탯 1당 10%씩 감소 (최대 30% 감소로 제한)
	const float ReductionPercent = FMath::Min(0.4f, PStat * 0.1f);

	// 여기서는 1.0에서 감소율을 뺀 값을 반환 (Coefficient와 곱해짐)
	const float ReductionMultiplier = 1.0f - ReductionPercent;

	return ReductionMultiplier; // 블루프린트의 Coefficient(기본 쿨다운)와 곱해짐
}
