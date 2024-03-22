#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
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

void USpellMenuWidgetController::SpellGlobeSelected( UAuraUserWidget *SpellGlobe )
{
	SpellGlobeSelectedDelegate.Broadcast( SpellGlobe );
}
