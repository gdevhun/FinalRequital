// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "FRPlayerState.generated.h"

enum class EMaskSkillType : uint8;
enum class EWeaponType : uint8;

UENUM(BlueprintType)
enum class EFRCharacterStatType : uint8
{
	H UMETA(DisplayName = "기력"),
	D UMETA(DisplayName = "결심"),
	P UMETA(DisplayName = "의념")
};

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API AFRPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AFRPlayerState();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// 무기 관리
	UFUNCTION(BlueprintCallable)
	void AcquireWeapon(EWeaponType WeaponType);
	bool HasWeapon(EWeaponType WeaponType) const;

	// 마스크 스킬 관리
	UFUNCTION(BlueprintCallable)
	void AcquireMaskSkill(EMaskSkillType SkillType);
	bool HasMaskSkill() const;

	EMaskSkillType GetSelectedMaskSkill() const;

	// 스탯 관리
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void IncreaseStat(EFRCharacterStatType StatType, int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ApplyStatsToAttributes();

protected:
	UPROPERTY(EditAnywhere, Category="GAS")
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UFRCharacterAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Replicated, Category = "GAS")
	TArray<EWeaponType> AcquiredWeapons;

	UPROPERTY(EditAnywhere, Replicated, Category = "GAS")
	EMaskSkillType SelectedMaskSkill;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	int32 Stat_H;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	int32 Stat_D;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	int32 Stat_P;
};
