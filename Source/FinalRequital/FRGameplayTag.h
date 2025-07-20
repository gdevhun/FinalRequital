

#pragma once

#include "GameplayTagContainer.h"

#define FRTAG_DATA_DAMAGE FGameplayTag::RequestGameplayTag(FName("Data.Damage"))

#define FRTAG_CHARACTER_WEAPONSWAPACTION FGameplayTag::RequestGameplayTag(FName("Character.Action.WeaponSwap"))

#define FRTAG_CHARACTER_ISSKILLING FGameplayTag::RequestGameplayTag(FName("Character.State.IsSkilling"))
#define FRTAG_CHARACTER_ISDEAD FGameplayTag::RequestGameplayTag(FName("Character.State.IsDead"))
#define FRTAG_CHARACTER_INVISIBLE FGameplayTag::RequestGameplayTag(FName("Character.State.Invisible"))
#define FRTAG_CHARACTER_STUNNED FGameplayTag::RequestGameplayTag(FName("Character.State.Stunned"))
#define FRTAG_CHARACTER_IMMUNE FGameplayTag::RequestGameplayTag(FName("Character.State.Immune"))

#define FRTAG_CHARACTER_COOLTIME_SWORD FGameplayTag::RequestGameplayTag(FName("Character.State.IronMaceSpecialAttackCooldown"))
#define FRTAG_CHARACTER_COOLTIME_IRONMACE FGameplayTag::RequestGameplayTag(FName("Character.State.SwordSpecialAttackCooldown"))

#define GAMEPLAYCUE_CHARACTER_MELEEATTACKHIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.MeleeAttackHit"))
#define GAMEPLAYCUE_CHARACTER_ARROWATTACKHIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.ArrowAttackHit"))
#define GAMEPLAYCUE_CHARACTER_BRONZEBELLHIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.BronzeBellHit"))
#define GAMEPLAYCUE_CHARACTER_AMULETDOTHIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.AmuletDotHit"))
