// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Interactable/FRInteractableObject.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "Player/FRPlayerController.h"

AFRInteractableObject::AFRInteractableObject()
{

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	//AudioComponent->bAutoActivate = false;
}

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
				AFRPlayerController* FRPC = Cast<AFRPlayerController>(PC);
				FRPC->bIsInteracting = true;
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
				AFRPlayerController* FRPC = Cast<AFRPlayerController>(PC);
				FRPC->bIsInteracting = false;
				PC->SetInputMode(FInputModeGameOnly());
				PC->bShowMouseCursor = false;
			}
		}
	}
}

void AFRInteractableObject::PlayInteractionSound()
{
	if (AudioComponent && !AudioComponent->IsPlaying())
	{
		AudioComponent->Play();
	}
}

void AFRInteractableObject::StopInteractionSound()
{
	if (AudioComponent && AudioComponent->IsPlaying())
	{
		AudioComponent->Stop();
	}
}

void AFRInteractableObject::RestartInteractionSound()
{
	if (AudioComponent)
	{
		AudioComponent->Stop();
		AudioComponent->Play();
	}
}