// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/interactable/FRInteractableBase.h"
#include "FRLevelPortal.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API AFRLevelPortal : public AFRInteractableBase
{
	GENERATED_BODY()

public:
	AFRLevelPortal();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
	TObjectPtr<class UNiagaraComponent> PortalEffect;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:
	UFUNCTION(BlueprintCallable)
	virtual void Interact_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void ActivatePortal();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	bool bIsActivated = false;

	UFUNCTION(BlueprintCallable)
	void OpenNextLevel();

	UPROPERTY(EditAnywhere, Category = "Level")
	FName NextLevelName;
};
