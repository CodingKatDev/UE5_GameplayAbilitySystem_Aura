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

	/* Effects */
	FGameplayTag Effects_HitReact;

	/* Abilities */
	FGameplayTag Abilities_Attack;

private:
	static FAuraGameplayTags GameplayTags;
};
