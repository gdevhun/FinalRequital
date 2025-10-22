// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "FRBossAsura.generated.h"

UCLASS()
class FINALREQUITAL_API AFRBossAsura : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AFRBossAsura();

protected:

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UFRBossAttributeSet> AttributeSet;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> Phase1StatEffect;
	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> Phase2StatEffect;
	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> Phase3StatEffect;

	UPROPERTY()
	TObjectPtr<class AFRCharacterBase> TargetPlayer;

	//UFUNCTION()
	//void LookAtTargetPlayer();

	UFUNCTION()
	virtual void OnOutOfHealth();
private:
	FTimerHandle LookAtTimerHandle;
	FRotator TargetRotation;
	bool bShouldRotate = false;

	UFUNCTION()
	void UpdateTargetRotation();
};
