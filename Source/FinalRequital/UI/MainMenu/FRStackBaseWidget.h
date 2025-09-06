// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FRStackBaseWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRStackBaseWidget : public UUserWidget
{
	GENERATED_BODY()
	// ESC Key Remove UI Widget Stack Base Class Parent

public:
	UFRStackBaseWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void RemoveUIWithPlayerController();

	UFUNCTION(BlueprintCallable)
	void AddUIWithPlayerController(UFRStackBaseWidget* NewWidget);
};
