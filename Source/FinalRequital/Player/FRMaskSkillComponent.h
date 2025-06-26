// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Components/ActorComponent.h"
#include "FRMaskSkillComponent.generated.h"

UENUM(BlueprintType)
enum class EMaskSkillType : uint8
{
	None		UMETA(DisplayName = "None"),
	Gyeok		UMETA(DisplayName = "Gyeok"),
	Yu			UMETA(DisplayName = "Yu"),
	Myeol		UMETA(DisplayName = "Myeol"),
	Jeong		UMETA(DisplayName = "Jeong")
};

UCLASS(ClassGroup = (Custom), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class FINALREQUITAL_API UFRMaskSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFRMaskSkillComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void ActivateSelectedMaskSkill() const;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<UGameplayAbility> GyeokAbility;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<UGameplayAbility> YuAbility;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<UGameplayAbility> MyeolAbility;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<UGameplayAbility> JeongAbility;
};
