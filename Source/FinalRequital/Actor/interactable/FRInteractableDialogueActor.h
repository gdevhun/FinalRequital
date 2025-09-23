// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Interactable/FRInteractableBase.h"
#include "Interface/FRDialogueInterface.h"
#include "FRInteractableDialogueActor.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API AFRInteractableDialogueActor : public AFRInteractableBase, public IFRDialogueInterface
{
	GENERATED_BODY()

public:
	AFRInteractableDialogueActor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dialogue")
	TSubclassOf<class UFRDialogueWidget> DialogueWidgetClass;

	UPROPERTY()
	TObjectPtr<class UFRDialogueWidget> DialogueWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dialogue")
	TArray<FText> DialogueLines;

	int32 CurrentDialogueIndex;

	virtual void StartDialogue() override;
	virtual void ProceedDialogue() override;
	virtual void ExitDialogue() override;
};
