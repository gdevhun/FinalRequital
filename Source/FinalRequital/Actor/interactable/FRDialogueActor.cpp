// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Interactable/FRDialogueActor.h"
#include "UI/FRDialogueWidget.h"

AFRDialogueActor::AFRDialogueActor()
{
	CurrentDialogueIndex = 0;
}

void AFRDialogueActor::StartDialogue()
{
	CurrentDialogueIndex = 0;
	if (DialogueWidgetClass && !DialogueWidget)
	{
		DialogueWidget = CreateWidget<UFRDialogueWidget>(GetWorld(), DialogueWidgetClass);
		if (DialogueWidget)
		{
			DialogueWidget->AddToViewport();
			DialogueWidget->UpdateDialogue(DialogueLines[CurrentDialogueIndex]);
		}
	}
}

void AFRDialogueActor::ProceedDialogue()
{
	if (!DialogueWidget) return;

	CurrentDialogueIndex++;
	if (DialogueLines.IsValidIndex(CurrentDialogueIndex))
	{
		DialogueWidget->UpdateDialogue(DialogueLines[CurrentDialogueIndex]);
	}
	else
	{
		ExitDialogue();
	}
}

void AFRDialogueActor::ExitDialogue()
{
	if (DialogueWidget)
	{
		DialogueWidget->RemoveFromParent();
		DialogueWidget = nullptr;
		DialogueFinishCallback();
	}
	
}

void AFRDialogueActor::DialogueFinishCallback_Implementation()
{
}
