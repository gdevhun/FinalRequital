// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameFramework/Character.h"
#include "FRMonsterBase.generated.h"


UCLASS()
class FINALREQUITAL_API AFRMonsterBase : public ACharacter , public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AFRMonsterBase();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UFRCharacterAttributeSet> AttributeSet;

public:
	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> InitStatEffect;

	UPROPERTY(EditAnywhere, Category = GAS)
	float Level;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayAbility> HitReactAbilityClass;

	//UPROPERTY(EditAnywhere, Category = HP)
	//TSubclassOf<class UUserWidget> HpWidgetClassRef;

	UFUNCTION()
	virtual void OnOutOfHealth();

	UPROPERTY(EditAnywhere, Category = Stat)
	TObjectPtr<class UFRWidgetComponent> HpBar;

	UPROPERTY(EditAnywhere, Category = Stat)
	float WalkMaxSpeed = 200.0f;

	//Anim Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> HitMontage;*/
	UFUNCTION()
	void HitReact();

private:
	FGameplayAbilitySpecHandle HitReactAbilityHandle;
	void PlayAnimMontage(UAnimMontage* Montage) const;

	void SetDead();
	float DeadEventDelayTime = 3.0f;
};
