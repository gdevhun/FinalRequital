// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FRMaskSkillComponent.h"
#include "FRPlayerState.h"
#include "AbilitySystemComponent.h"

UFRMaskSkillComponent::UFRMaskSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFRMaskSkillComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UFRMaskSkillComponent::ActivateSelectedMaskSkill() const
{
	const APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;

	AFRPlayerState* PS = OwnerPawn->GetPlayerState<AFRPlayerState>();
	if (!PS || !PS->HasMaskSkill())
		return;

	const EMaskSkillType SkillType = PS->GetSelectedMaskSkill();
	UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
	if (!ASC) return;

	TSubclassOf<UGameplayAbility> SkillAbility = nullptr;

	switch (SkillType)
	{
	case EMaskSkillType::Gyeok:
		SkillAbility = GyeokAbility;
		break;
	case EMaskSkillType::Yu:
		SkillAbility = YuAbility;
		break;
	case EMaskSkillType::Myeol:
		SkillAbility = MyeolAbility;
		break;
	case EMaskSkillType::Jeong:
		SkillAbility = JeongAbility;
		break;
	default:
		break;
	}

	if (SkillAbility)
	{
		ASC->GiveAbility(FGameplayAbilitySpec(SkillAbility, 1, 0));
		UE_LOG(LogTemp, Log, TEXT("Mask skill ability granted: %s"), *SkillAbility->GetName());
	}
}
