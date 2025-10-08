// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Interactable/FRInteractableObject.h"
#include "Blueprint/UserWidget.h"

void AFRInteractableObject::ShowSelectionWidget()
{
	if (SelectionWidgetClass)
	{
		SelectionWidget = CreateWidget<UUserWidget>(GetWorld(), SelectionWidgetClass);
		if (SelectionWidget)
		{
			SelectionWidget->AddToViewport();
			APlayerController* PC = GetWorld()->GetFirstPlayerController();
			if (PC)
			{
				PC->SetInputMode(FInputModeGameAndUI());
				PC->bShowMouseCursor = true; 
			}
		}
	}
}

void AFRInteractableObject::HideSelectionWidget()
{
	if (SelectionWidgetClass)
	{
		if (SelectionWidget)
		{
			SelectionWidget->RemoveFromParent();
			APlayerController* PC = GetWorld()->GetFirstPlayerController();
			if (PC)
			{
				PC->SetInputMode(FInputModeGameOnly());
				PC->bShowMouseCursor = false;
			}
		}
	}
}
