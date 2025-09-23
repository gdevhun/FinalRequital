// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Interactable/FRInteractableBase.h"
#include "Interface/FRDialogueInterface.h"
#include "FRDialogueActor.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API AFRDialogueActor : public AFRInteractableBase, public IFRDialogueInterface
{
	GENERATED_BODY()

public:
	AFRDialogueActor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TSubclassOf<class UFRDialogueWidget> DialogueWidgetClass;

	UPROPERTY()
	TObjectPtr<class UFRDialogueWidget> DialogueWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<FText> DialogueLines;

	int32 CurrentDialogueIndex;

	UFUNCTION(BlueprintCallable)
	virtual void StartDialogue() override;
	UFUNCTION(BlueprintCallable)
	virtual void ProceedDialogue() override;
	UFUNCTION(BlueprintCallable)
	virtual void ExitDialogue() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void DialogueFinishCallback();
	virtual void DialogueFinishCallback_Implementation();
};
