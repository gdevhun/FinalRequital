// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FRHintBox.h"
#include "Components/SphereComponent.h"
#include "GAS/Attribute/FRHintBoxAttributeSet.h"
#include "Physics/FRCollision.h"
#include "Player/FRGASCharacterPlayer.h"
#include "UI/FRWidgetComponent.h"

AFRHintBox::AFRHintBox()
{
	PrimaryActorTick.bCanEverTick = false;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);

	//Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HintBoxMesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	Mesh->SetCollisionResponseToChannel(CCHANNEL_FRACTION, ECR_Block);

	// HpBar UI Detect Sphere
	DetectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HintBoxDetectSphere"));
	DetectSphere->SetupAttachment(RootComponent);
	DetectSphere->InitSphereRadius(150.0f);
	DetectSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	DetectSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// ASC
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UFRHintBoxAttributeSet>(TEXT("HintBoxAttributeSet"));

	// HPBar UI
	HpBar = CreateDefaultSubobject<UFRWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(RootComponent);
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> HpWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_HintBoxHpBar.WBP_HintBoxHpBar_C'"));
	if (HpWidgetRef.Succeeded())
	{
		HpBar->SetWidgetClass(HpWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(160.0f, 20.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

class UAbilitySystemComponent* AFRHintBox::GetAbilitySystemComponent() const
{
	return ASC;
}

void AFRHintBox::BeginPlay()
{
	Super::BeginPlay();

	ASC->InitAbilityActorInfo(this, this);
	HpBar->SetVisibility(false);

	if (AttributeSet)
	{
		AttributeSet->OnOutOfHealth.AddDynamic(this, &AFRHintBox::OnOutOfHealth);
	}

	if (DetectSphere)
	{
		DetectSphere->OnComponentBeginOverlap.AddDynamic(this, &AFRHintBox::OnSphereBeginOverlap);
		DetectSphere->OnComponentEndOverlap.AddDynamic(this, &AFRHintBox::OnSphereEndOverlap);
	}
}

void AFRHintBox::OnOutOfHealth_Implementation()
{
	Destroy();
}

void AFRHintBox::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(AFRGASCharacterPlayer::StaticClass()))
	{
		if (HpBar)
		{
			HpBar->SetVisibility(true);
		}
	}
}

void AFRHintBox::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA(AFRGASCharacterPlayer::StaticClass()))
	{
		if (HpBar)
		{
			HpBar->SetVisibility(false);
		}
	}
}


