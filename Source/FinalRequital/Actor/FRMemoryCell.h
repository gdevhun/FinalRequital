// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "FRMemoryCell.generated.h"

UCLASS()
class FINALREQUITAL_API AFRMemoryCell : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	AFRMemoryCell();

protected:
	virtual void BeginPlay() override;
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:	
	UFUNCTION()
	void OnDetectedOnce();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AttachToSoul();
	void AttachToSoul_Implementation();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//bool bIsTutorialObj = false;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USceneComponent> RootSceneComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UParticleSystemComponent> FXComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAudioComponent> AudioComponent;

	bool bIsDetected;
};
