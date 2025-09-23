// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FRDialogueWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFRDialogueWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_Dialogue;

	UFUNCTION(BlueprintCallable, Category="Dialogue")
	void UpdateDialogueText(const FText& NewText);
};
