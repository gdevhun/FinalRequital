// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Interactable/FRInteractableBase.h"
#include "FRDebugHelper.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Player/FRGASCharacterPlayer.h"
#include "UI/FRWidgetComponent.h"

AFRInteractableBase::AFRInteractableBase()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = RootSceneComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionBox"));
	TriggerSphere->SetupAttachment(RootComponent);
	TriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	TriggerSphere->SetGenerateOverlapEvents(true);
	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AFRInteractableBase::OnOverlapBegin);
	TriggerSphere->OnComponentEndOverlap.AddDynamic(this, &AFRInteractableBase::OnOverlapEnd);

	InteractionPromptWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionPromptWidget->SetupAttachment(Mesh);
	InteractionPromptWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));


	static ConstructorHelpers::FClassFinder<UUserWidget> InteractionWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_InteractionPrompt.WBP_InteractionPrompt_C'"));
	if (InteractionWidgetRef.Succeeded())
	{
		InteractionPromptWidget->SetWidgetClass(InteractionWidgetRef.Class);
		InteractionPromptWidget->SetWidgetSpace(EWidgetSpace::Screen);
		InteractionPromptWidget->SetDrawSize(FVector2D(48.f, 48.f));
		InteractionPromptWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AFRInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	HideInteractionPrompt();
}

void AFRInteractableBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsDoOnce) return;

	if (AFRCharacterBase* Player = Cast<AFRCharacterBase>(OtherActor))
	{
		ShowInteractionPrompt();
		Player->SetInteractiveActor(this);
	}
}

void AFRInteractableBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bIsDoOnce) return;

	if (AFRCharacterBase* Player = Cast<AFRCharacterBase>(OtherActor))
	{
		HideInteractionPrompt();
		Player->SetInteractiveActor(nullptr);
	}
}

void AFRInteractableBase::ShowInteractionPrompt() const
{
	(bIsDoOnce)? InteractionPromptWidget->SetVisibility(false) : InteractionPromptWidget->SetVisibility(true);
}

void AFRInteractableBase::HideInteractionPrompt() const
{
	InteractionPromptWidget->SetVisibility(false);
}


void AFRInteractableBase::Interact_Implementation()
{
	if (bIsDoOnce)
	{
		return;
	}
}


