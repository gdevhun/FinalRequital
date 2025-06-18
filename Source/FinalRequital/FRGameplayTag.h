

#pragma once

#include "GameplayTagContainer.h"

#define FRTAG_DATA_DAMAGE FGameplayTag::RequestGameplayTag(FName("Data.Damage"))
#define FRTAG_CHARACTER_ISDEAD FGameplayTag::RequestGameplayTag(FName("Character.State.IsDead"))
#define FRTAG_CHARACTER_INVISIBLE FGameplayTag::RequestGameplayTag(FName("Character.State.Invisible"))
#define FRTAG_CHARACTER_STUNNED FGameplayTag::RequestGameplayTag(FName("Character.State.Stunned"))

#define GAMEPLAYCUE_CHARACTER_MELEEATTACKHIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.MeleeAttackHit"))
#define GAMEPLAYCUE_CHARACTER_ARROWHIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.ArrowHit"))
#define GAMEPLAYCUE_CHARACTER_BRONZEBELLHIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.BronzeBellHit"))
