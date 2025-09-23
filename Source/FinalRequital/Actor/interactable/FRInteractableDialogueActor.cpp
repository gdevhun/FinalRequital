// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Interactable/FRInteractableDialogueActor.h"
#include "UI/FRDialogueWidget.h"

AFRInteractableDialogueActor::AFRInteractableDialogueActor()
{
	CurrentDialogueIndex = 0;
}

void AFRInteractableDialogueActor::StartDialogue()
{
	CurrentDialogueIndex = 0;
	if (DialogueWidgetClass && !DialogueWidget)
	{
		DialogueWidget = CreateWidget<UFRDialogueWidget>(GetWorld(), DialogueWidgetClass);
		if (DialogueWidget)
		{
			DialogueWidget->AddToViewport();
			DialogueWidget->UpdateDialogueText(DialogueLines[CurrentDialogueIndex]);
		}
	}
}

void AFRInteractableDialogueActor::ProceedDialogue()
{
	if (!DialogueWidget) return;

	CurrentDialogueIndex++;
	if (DialogueLines.IsValidIndex(CurrentDialogueIndex))
	{
		DialogueWidget->UpdateDialogueText(DialogueLines[CurrentDialogueIndex]);
	}
	else
	{
		ExitDialogue();
	}
}

void AFRInteractableDialogueActor::ExitDialogue()
{
	if (DialogueWidget)
	{
		DialogueWidget->RemoveFromParent();
		DialogueWidget = nullptr;
	}
}
