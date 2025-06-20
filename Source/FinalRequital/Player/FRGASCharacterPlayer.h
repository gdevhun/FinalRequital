// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Character/FRCharacterBase.h"
#include "FRGASCharacterPlayer.generated.h"

enum class EWeaponType : uint8;
/**
 * 
 */
UCLASS()
class FINALREQUITAL_API AFRGASCharacterPlayer : public AFRCharacterBase, public IAbilitySystemInterface
{

	GENERATED_BODY()

public:
	AFRGASCharacterPlayer();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

protected:
	void SetupGASInputComponent();
	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);

public:
	// Weapon QuickSlot
	void HandleQuickSlot(EWeaponType WeaponType);
	void QuickSlot1();
	void QuickSlot2();
	void QuickSlot3();
	void QuickSlot4();

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = GAS)
	TMap<int32, TSubclassOf<class UGameplayAbility>> StartInputAbilities;

	UPROPERTY(EditAnywhere, Category = Component)
	TObjectPtr<class UFRWeaponComponent> WeaponComponent;

	UPROPERTY(EditAnywhere, Category = Component)
	TObjectPtr<class UFRMaskSkillComponent> MaskSkillComponent;


	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UFRWidgetComponent> HpBar;

public:
	// State Delegate
	void OnStunTagChanged(const FGameplayTag Tag, int32 NewCount);

	UPROPERTY(EditAnywhere, Category = Animation)
	TObjectPtr<UAnimMontage> StunMontage;

	// LayerAnim Section
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AdjustUnarmedLayerAnim();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AdjustSwordLayerAnim();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AdjustIronMaceLayerAnim();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AdjustBowLayerAnim();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AdjustBronzeBellLayerAnim();
};
