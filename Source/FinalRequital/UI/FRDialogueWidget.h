// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FRDialogueWidget.generated.h"

struct FDialogueLine;
/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFRDialogueWidget(const FObjectInitializer& ObjectInitializer);

	void NativeConstruct() override;
	void NativeDestruct() override;

	//Dialogue Top Text
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> IMG_BG_Dialogue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_Dialogue;

	//Monologue bottom Text
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> IMG_BG_Monologue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_Monologue;

	UFUNCTION(BlueprintCallable, Category="Dialogue")
	void UpdateDialogue(const FDialogueLine& Line);

};
