#pragma once

#include "GameplayTagContainer.h"


struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags &Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	/* Inputs */
	FGameplayTag Input_Mouse_LeftButton;
	FGameplayTag Input_Mouse_RightButton;
	FGameplayTag Input_Key_1;
	FGameplayTag Input_Key_2;
	FGameplayTag Input_Key_3;
	FGameplayTag Input_Key_4;
	FGameplayTag Input_Passive_1;
	FGameplayTag Input_Passive_2;

	/* Meta Attributes */
	FGameplayTag Attributes_Meta_IncomingXP;

	/* Vital Attributes */
	FGameplayTag Attributes_Vital_Health;
	FGameplayTag Attributes_Vital_Mana;

	/* Primary Attributes */
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;

	/* Secondary Attributes */
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

	/* Resistances */
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Physical;

	/* Damage and Types */
	FGameplayTag Damage;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Physical;

	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

	/* Debuffs */
	FGameplayTag Debuff_Arcane;
	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Physical;
	FGameplayTag Debuff_Stun;

	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;

	/* Debuff Parameters */
	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Duration;
	FGameplayTag Debuff_Frequency;

	/* Abilities */
	FGameplayTag Abilities_None;
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;

	FGameplayTag Abilities_HitReact;

	/* Ability Status */
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Equipped;
	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Unlocked;

	/* Ability Type */
	FGameplayTag Abilities_Type_None;
	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;

	/* Spells */
	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Abilities_Lightning_Electrocute;

	/* Cooldowns */
	FGameplayTag Cooldown_Fire_FireBolt;
	FGameplayTag Cooldown_Lightning_Electrocute;

	/* Combat Sockets */
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;

	/* Montages */
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

	/* Effects */
	FGameplayTag Effects_HitReact;

	/* Player */
	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_InputHeld;
	FGameplayTag Player_Block_InputReleased;
	FGameplayTag Player_Block_CursorTrace;

private:
	static FAuraGameplayTags GameplayTags;
};
