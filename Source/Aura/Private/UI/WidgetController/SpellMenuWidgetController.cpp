#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AuraGameplayTags.h"
#include "Player/AuraPlayerState.h"


void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointsChangedDelegate.Broadcast( GetAuraPS()->GetSpellPoints() );
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraASC()->AbilityStatusChangedDelegate.AddLambda( [ this ]( const FGameplayTag &AbilityTag, const FGameplayTag &StatusTag, int32 NewLevel )
		{
			if( SelectedAbility.Ability.MatchesTagExact( AbilityTag ) )
			{
				SelectedAbility.Status = StatusTag;
				ShouldEnableButtons( StatusTag );
				SpellGlobeAbilityDescriptions( AbilityTag );
			}
			
			if( AbilityInfo )
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag( AbilityTag );
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast( Info );
			}
		} );

	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda( [ this ]( int32 SpellPoints )
		{
			SpellPointsChangedDelegate.Broadcast( SpellPoints );
			CurrentSpellPoints = SpellPoints;
			ShouldEnableButtons( SelectedAbility.Status );
			SpellGlobeAbilityDescriptions( SelectedAbility.Ability );
		} );
}

void USpellMenuWidgetController::SpellGlobeSelected( const FGameplayTag &SpellGlobeAbilityTag )
{
	const FGameplayAbilitySpec *AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag( SpellGlobeAbilityTag );
	FGameplayTag SpellGlobeStatusTag = GetAuraASC()->GetStatusFromSpec( *AbilitySpec );

	SelectedAbility.Ability = SpellGlobeAbilityTag;
	SelectedAbility.Status = SpellGlobeStatusTag;

	SpellGlobeSelectedDelegate.Broadcast( SpellGlobeAbilityTag );
	SpellGlobeAbilityDescriptions( SpellGlobeAbilityTag );
	ShouldEnableButtons( SpellGlobeStatusTag );
}

void USpellMenuWidgetController::ClearSelectedGlobe()
{
	SelectedAbility.Ability = FAuraGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FAuraGameplayTags::Get().Abilities_Status_Locked;

	ButtonsEnabledStatusDelegate.Broadcast( false, false );
	AbilityDescriptionsDelegate.Broadcast( FString(), FString() );
}

void USpellMenuWidgetController::SpellGlobeAbilityDescriptions( const FGameplayTag &AbilityTag )
{
	FString Description;
	FString NextLevelDescription;

	GetAuraASC()->GetDescriptionsByAbilityTag( AbilityTag, Description, NextLevelDescription );
	AbilityDescriptionsDelegate.Broadcast( Description, NextLevelDescription );
}

void USpellMenuWidgetController::ShouldEnableButtons( const FGameplayTag &StatusTag )
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	bool bEnableSpendPoint = false;
	bool bEnableEquip = false;

	if( SelectedAbility.Ability.MatchesTagExact( GameplayTags.Abilities_None ) || StatusTag.MatchesTagExact( GameplayTags.Abilities_Status_Locked ) ) return;

	if( CurrentSpellPoints > 0 )
	{
		bEnableSpendPoint = true;
	}

	if( StatusTag.MatchesTagExact( GameplayTags.Abilities_Status_Unlocked ) || StatusTag.MatchesTagExact( GameplayTags.Abilities_Status_Equipped ) )
	{
		bEnableEquip = true;
	}

	ButtonsEnabledStatusDelegate.Broadcast( bEnableSpendPoint, bEnableEquip );
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if( GetAuraASC() )
	{
		GetAuraASC()->ServerSpendSpellPoint( SelectedAbility.Ability );
	}
}