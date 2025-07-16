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
	void ShowCrossHair(bool bShow);
	void ChangeCrossHairColor();
	void ResetCrossHairColor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> IMG_CrossHair;

private:

};
