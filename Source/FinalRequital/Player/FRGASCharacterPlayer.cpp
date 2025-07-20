// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FRGASCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Player/FRPlayerState.h"
#include "EnhancedInputComponent.h"
#include "FRDebugHelper.h"
#include "FRGameplayTag.h"
#include "FRWeaponComponent.h"
#include "FRMaskSkillComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Attribute/FRCharacterAttributeSet.h"

AFRGASCharacterPlayer::AFRGASCharacterPlayer()
{
	ASC = nullptr;
	WeaponComponent = CreateDefaultSubobject<UFRWeaponComponent>(TEXT("WeaponComponent"));
	MaskSkillComponent = CreateDefaultSubobject<UFRMaskSkillComponent>(TEXT("MaskSkillComponent"));
}

void AFRGASCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AFRPlayerState* GASPS = GetPlayerState<AFRPlayerState>();

	if (GASPS)
	{
		ASC = GASPS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(GASPS, this);

		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			ASC->GiveAbility(StartSpec);
		}
		for (const auto& StartInputAbility : StartInputAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			StartSpec.InputID = StartInputAbility.Key;
			ASC->GiveAbility(StartSpec);
		}

		SetupGASInputComponent();
	}
}

void AFRGASCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	SetupGASInputComponent();
}

void AFRGASCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	if (ASC)
	{
		ASC->RegisterGameplayTagEvent(FRTAG_CHARACTER_STUNNED, EGameplayTagEventType::NewOrRemoved)
			.AddUObject(this, &AFRGASCharacterPlayer::OnStunTagChanged);

		if (const UFRCharacterAttributeSet* Attribute = Cast<UFRCharacterAttributeSet>(ASC->GetSet<UAttributeSet>()))
		{
			Attribute->OnTakeDamage.AddDynamic(this, &AFRGASCharacterPlayer::HandleTakeDamage); 
		}
	}
}

void AFRGASCharacterPlayer::SetupGASInputComponent()
{
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AFRGASCharacterPlayer::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AFRGASCharacterPlayer::GASInputReleased, 0);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AFRGASCharacterPlayer::GASInputPressed, 1);
		EnhancedInputComponent->BindAction(SpecialAttackAction, ETriggerEvent::Triggered, this, &AFRGASCharacterPlayer::GASInputPressed, 2);
		EnhancedInputComponent->BindAction(SkillAttackAction, ETriggerEvent::Triggered, this, &AFRGASCharacterPlayer::GASInputPressed, 3);
		EnhancedInputComponent->BindAction(QuickSlotActions[0], ETriggerEvent::Triggered, this, &AFRGASCharacterPlayer::QuickSlot1);
		EnhancedInputComponent->BindAction(QuickSlotActions[1], ETriggerEvent::Triggered, this, &AFRGASCharacterPlayer::QuickSlot2);
		EnhancedInputComponent->BindAction(QuickSlotActions[2], ETriggerEvent::Triggered, this, &AFRGASCharacterPlayer::QuickSlot3);
		EnhancedInputComponent->BindAction(QuickSlotActions[3], ETriggerEvent::Triggered, this, &AFRGASCharacterPlayer::QuickSlot4);
	}
}

void AFRGASCharacterPlayer::HandleQuickSlot(EWeaponType WeaponType)
{
	// WEAPON SWAP EXCEPTION
	if (ASC && ASC->HasMatchingGameplayTag(FRTAG_CHARACTER_STUNNED))
	{
		return;
	}
	if (ASC && ASC->HasMatchingGameplayTag(FRTAG_CHARACTER_ISSKILLING)) 
	{
		return; 
	}

	if (WeaponComponent)
	{
		WeaponComponent->EquipWeapon(WeaponType);
	}
}

void AFRGASCharacterPlayer::QuickSlot1() { HandleQuickSlot(EWeaponType::Sword); }
void AFRGASCharacterPlayer::QuickSlot2() { HandleQuickSlot(EWeaponType::Bow); }
void AFRGASCharacterPlayer::QuickSlot3() { HandleQuickSlot(EWeaponType::IronMace); }
void AFRGASCharacterPlayer::QuickSlot4() { HandleQuickSlot(EWeaponType::BronzeBell); }


void AFRGASCharacterPlayer::OnStunTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
	{
		GetCharacterMovement()->DisableMovement();
		bIsStunned = true;

		if (HitReactAbilityClass)
		{
			FGameplayAbilitySpec Spec(HitReactAbilityClass);
			ASC->TryActivateAbility(ASC->GiveAbilityAndActivateOnce(Spec));
		}
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		bIsStunned = false;
	}
}

void AFRGASCharacterPlayer::HandleTakeDamage()
{
	if (!ASC || ASC->HasMatchingGameplayTag(FRTAG_CHARACTER_IMMUNE))
		return;

	if (HitReactAbilityClass)
	{
		FGameplayAbilitySpec Spec(HitReactAbilityClass);
		ASC->TryActivateAbility(ASC->GiveAbilityAndActivateOnce(Spec));
	}
}

void AFRGASCharacterPlayer::GASInputPressed(int32 InputId)
{
	if (ASC && ASC->HasMatchingGameplayTag(FRTAG_CHARACTER_STUNNED))
	{
		return;
	}

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = true;

		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void AFRGASCharacterPlayer::GASInputReleased(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = false;

		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

UAbilitySystemComponent* AFRGASCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AFRGASCharacterPlayer::AdjustUnarmedLayerAnim_Implementation() { SetMovementSpeed(500.0f); }
void AFRGASCharacterPlayer::AdjustBronzeBellLayerAnim_Implementation()  { SetMovementSpeed(350.0f); }
void AFRGASCharacterPlayer::AdjustBowLayerAnim_Implementation() { SetMovementSpeed(350.0f); }
void AFRGASCharacterPlayer::AdjustSwordLayerAnim_Implementation() { SetMovementSpeed(350.0f); }
void AFRGASCharacterPlayer::AdjustIronMaceLayerAnim_Implementation() { SetMovementSpeed(350.0f); }

