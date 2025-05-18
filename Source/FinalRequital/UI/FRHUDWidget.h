// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FRHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFRHUDWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:

	void ShowCrosshair(bool bShow);

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<class UImage> Crosshair;
};
