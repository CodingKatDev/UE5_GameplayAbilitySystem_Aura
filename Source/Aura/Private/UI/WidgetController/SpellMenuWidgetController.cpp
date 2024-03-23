#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AuraGameplayTags.h"
#include "Aura/AuraLogChannels.h"
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
			if( AbilityInfo )
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag( AbilityTag );
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast( Info );
			}
		} );

	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda( [ this ]( int32 SpellPoints ) { SpellPointsChangedDelegate.Broadcast( SpellPoints ); } );
}

void USpellMenuWidgetController::SpellGlobeSelected( const FGameplayTag &SpellGlobeAbilityTag )
{
	SpellGlobeSelectedDelegate.Broadcast( SpellGlobeAbilityTag );

	UE_LOG( LogAura, Warning, TEXT( "SpellGlobeAbilityTag: [%s]" ), *SpellGlobeAbilityTag.ToString(), *GetNameSafe( this ) );

	SpellGlobeAbilityStatus( SpellGlobeAbilityTag );
}

void USpellMenuWidgetController::SpellGlobeAbilityStatus( const FGameplayTag &AbilityTag )
{
	const FGameplayAbilitySpec *AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag( AbilityTag );
	FGameplayTag AbilityStatus = GetAuraASC()->GetStatusFromSpec( *AbilitySpec );
	
	if( !AbilityStatus.IsValid() || AbilitySpec == nullptr ) return;

	UE_LOG( LogAura, Warning, TEXT( "AbilityStatus: [%s]" ), *AbilityStatus.ToString(), *GetNameSafe( this ) );

	UpdateButtonEnabledStatus( AbilityStatus, GetAuraPS()->GetSpellPoints() );
}

void USpellMenuWidgetController::UpdateButtonEnabledStatus( const FGameplayTag &AbilityStatus, int32 SpellPoints )
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	bool bEnableSpendPoint = false;
	bool bEnableEquip = false;

	if( !AbilityStatus.IsValid() ) return;

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
