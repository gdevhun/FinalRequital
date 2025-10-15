// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameFramework/Character.h"
#include "Interface/FRHighlightInterface.h"
#include "FRMonsterBase.generated.h"


UCLASS()
class FINALREQUITAL_API AFRMonsterBase : public ACharacter , public IAbilitySystemInterface, public IFRHighlightInterface
{
	GENERATED_BODY()

public:

	AFRMonsterBase();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Highlight() override;
	virtual void UnHighlight() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UFRMonsterAttributeSet> AttributeSet;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> InitStatEffect;

	UPROPERTY(EditAnywhere, Category = GAS)
	float Level;

	UFUNCTION()
	virtual void OnOutOfHealth();

	UPROPERTY(EditAnywhere, Category = Stat)
	TObjectPtr<class UFRWidgetComponent> HpBar;

	UPROPERTY(EditAnywhere, Category = Stat)
	float WalkMaxSpeed = 200.0f;

	//Anim Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	UFUNCTION()
	void HitReact();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	TObjectPtr<class USphereComponent> DetectSphere;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	FGameplayAbilitySpecHandle HitReactAbilityHandle;
	void PlayAnimMontage(UAnimMontage* Montage) const;

	void SetDead();
	float DeadEventDelayTime = 3.0f;
};
