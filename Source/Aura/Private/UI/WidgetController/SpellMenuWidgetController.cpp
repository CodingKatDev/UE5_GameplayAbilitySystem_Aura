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
	GetAuraASC()->AbilityStatusChangedDelegate.AddLambda(
		[ this ]( const FGameplayTag &AbilityTag, const FGameplayTag &StatusTag )
		{
			if( SelectedAbility.Ability.MatchesTagExact( AbilityTag ) )
			{
				SelectedAbility.Status = StatusTag;
				UpdateButtonEnabledStatus( StatusTag, CurrentSpellPoints );
			}
			
			if( AbilityInfo )
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag( AbilityTag );
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast( Info );
			}
		} );

	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda( 
		[ this ]( int32 SpellPoints ) 
		{ 
			SpellPointsChangedDelegate.Broadcast( SpellPoints ); 
			CurrentSpellPoints = SpellPoints;
			UpdateButtonEnabledStatus( SelectedAbility.Status, CurrentSpellPoints );
		} );
}

void USpellMenuWidgetController::SpellGlobeSelected( const FGameplayTag &SpellGlobeAbilityTag )
{
	SelectedAbility.Ability = SpellGlobeAbilityTag;

	SpellGlobeSelectedDelegate.Broadcast( SpellGlobeAbilityTag );
	SpellGlobeAbilityStatus();
}

void USpellMenuWidgetController::SpellGlobeAbilityStatus()
{
	const FGameplayAbilitySpec *AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag( SelectedAbility.Ability );
	FGameplayTag AbilityStatus = GetAuraASC()->GetStatusFromSpec( *AbilitySpec );
	SelectedAbility.Status = AbilityStatus;

	UpdateButtonEnabledStatus( AbilityStatus, CurrentSpellPoints );
}

void USpellMenuWidgetController::UpdateButtonEnabledStatus( const FGameplayTag &AbilityStatus, int32 SpellPoints )
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	bool bEnableSpendPoint = false;
	bool bEnableEquip = false;

	if( SpellPoints > 0 && AbilityStatus.MatchesTagExact( GameplayTags.Abilities_Status_Eligible ) )
	{
		bEnableSpendPoint = true;
	}

	if( AbilityStatus.MatchesTagExact( GameplayTags.Abilities_Status_Unlocked ) || AbilityStatus.MatchesTagExact( GameplayTags.Abilities_Status_Equipped ) )
	{
		bEnableEquip = true;
	}

	ButtonEnabledStatusDelegate.Broadcast( bEnableSpendPoint, bEnableEquip );
}
