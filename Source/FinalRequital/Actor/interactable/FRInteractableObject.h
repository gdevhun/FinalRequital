// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Interactable/FRInteractableBase.h"
#include "FRInteractableObject.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API AFRInteractableObject : public AFRInteractableBase
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable)
	void ShowSelectionWidget();

	UFUNCTION(BlueprintCallable)
	void HideSelectionWidget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<class UUserWidget> SelectionWidgetClass; 
	UPROPERTY()
	TObjectPtr<UUserWidget> SelectionWidget;

};
