#include "AuraGameplayTags.h"

#include "GameplayTagsManager.h"


FAuraGameplayTags FAuraGameplayTags::GameplayTags;


void FAuraGameplayTags::InitializeNativeGameplayTags()
{ 
   /*
	* Inputs
	*/
	GameplayTags.Input_Mouse_LeftButton = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Input.Mouse.LeftButton" ), FString( "Input Tag for Left Mouse Button" ) );
	GameplayTags.Input_Mouse_RightButton = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Input.Mouse.RightButton" ), FString( "Input Tag for Right Mouse Button" ) );
	GameplayTags.Input_Key_1 = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Input.Key.1" ), FString( "Input Tag for Key 1" ) );
	GameplayTags.Input_Key_2 = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Input.Key.2" ), FString( "Input Tag for Key 2" ) );
	GameplayTags.Input_Key_3 = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Input.Key.3" ), FString( "Input Tag for Key 3" ) );
	GameplayTags.Input_Key_4 = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Input.Key.4" ), FString( "Input Tag for Key 4" ) );

   /*
	* Vital Attributes
	*/
	GameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Attributes.Vital.Health" ), FString( "Amount of damage a player can take before death" ) );
	GameplayTags.Attributes_Vital_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Attributes.Vital.Mana" ), FString( "A resource used to cast spells" ) );

   /*
	* Primary Attributes
	*/
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Attributes.Primary.Strength" ), FString( "Increases physical damage" ) );
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Attributes.Primary.Intelligence" ), FString( "Increases magical damage" ) );
	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Attributes.Primary.Resilience" ), FString( "Increases Armor and Armor Penetration" ) );
	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Attributes.Primary.Vigor" ), FString( "Increases Health" ) );

   /*
	* Secondary Attributes
	*/
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Attributes.Secondary.Armor" ), FString( "Reduces damage taken, improves Block Chance" ) );
	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Attributes.Secondary.ArmorPenetration" ), FString( "Ignores percentage of enemy Armor, increases Critical Hit Chance" ) );
	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Attributes.Secondary.BlockChance" ), FString( "Chance to cut incoming damage in half" ) );
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Attributes.Secondary.CriticalHitChance" ), FString( "Chance to double damage plus Critical Hit Bonus" ) );
	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Attributes.Secondary.CriticalHitDamage" ), FString( "Bonus damage added when a critical hit is scored" ) );
	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Attributes.Secondary.CriticalHitResistance" ), FString( "Reduces Critical Hit Chance of attacking enemies" ) );
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Attributes.Secondary.HealthRegeneration" ), FString( "Amount of Health regenerated every 1 second" ) );
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Attributes.Secondary.ManaRegeneration" ), FString( "Amount of Mana regeneration every 1 second" ) );
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Attributes.Secondary.MaxHealth" ), FString( "Maximum amount of Health obtainable" ) );
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Attributes.Secondary.MaxMana" ), FString( "Maximum amount of Mana obtainable" ) );

   /*
	* Resistances
	*/
	GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Attributes.Resistance.Arcane" ), FString( "Resistance to Arcane damage" ) );
	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Attributes.Resistance.Fire" ), FString( "Resistance to Fire damage" ) );
	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Attributes.Resistance.Lightning" ), FString( "Resistance to Lightning damage" ) );
	GameplayTags.Attributes_Resistance_Physical= UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Attributes.Resistance.Physical" ), FString( "Resistance to Pyhsical damage" ) );

   /*
	* Damage and Types
	*/
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Damage" ), FString( "Damage" ) );
	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Damage.Arcane" ), FString( "Arcane Damage Type" ) );
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Damage.Fire" ), FString( "Fire Damage Type" ) );
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Damage.Lightning" ), FString( "Lightning Damage Type" ) );
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Damage.Physical" ), FString( "Physical Damage Type" ) );

   /*
	* Map of Damage Types to Resistances
	*/
	GameplayTags.DamageTypesToResistances.Add( GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane );
	GameplayTags.DamageTypesToResistances.Add( GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire );
	GameplayTags.DamageTypesToResistances.Add( GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning );
	GameplayTags.DamageTypesToResistances.Add( GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical );

   /*
	* Effects
	*/
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Effects.HitReact" ), FString( "Tag granted when Hit Reacting" ) );

   /*
	* Abilities
	*/
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag( FName( "Abilities.Attack" ), FString( "Attack Ability Tag" ) );
}