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
	None UMETA(DisplayName = "NULL"),
	H UMETA(DisplayName = "기력"),
	D UMETA(DisplayName = "결심"),
	P UMETA(DisplayName = "의념")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponAcquired, EWeaponType, WeaponType);

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
	virtual void BeginPlay() override;
	// 무기 관리
	UFUNCTION(BlueprintCallable)
	void AcquireWeapon(EWeaponType WeaponType);
	bool HasWeapon(EWeaponType WeaponType) const;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponAcquired OnWeaponAcquired;

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

public:
	UPROPERTY(EditAnywhere, Category="GAS")
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UFRCharacterAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TArray<EWeaponType> AcquiredWeapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	EMaskSkillType SelectedMaskSkill;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	int32 Stat_H;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	int32 Stat_D;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	int32 Stat_P;

};
