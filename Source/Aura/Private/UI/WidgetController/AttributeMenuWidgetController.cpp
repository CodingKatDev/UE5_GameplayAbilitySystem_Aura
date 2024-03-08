#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player//AuraPlayerState.h"


void UAttributeMenuWidgetController::BroadcastInitialValues()
{ 
	UAuraAttributeSet *AuraAS = CastChecked<UAuraAttributeSet>( AttributeSet );
	check( AttributeInfo );

	for( auto &Pair : AuraAS->TagsToAttributes )
	{
		//BroadcastAttributeInfo( Pair.Key, Pair.Value() );
		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag( Pair.Key );
		Info.AttributeValue = Pair.Value().GetNumericValue( AuraAS );
		AttributeInfoDelegate.Broadcast( Info );
	}

	AttributePointsChangedDelegate.Broadcast( GetAuraPS()->GetAttributePoints());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{ 
	check( AttributeInfo );
	for( auto &Pair : GetAuraAS()->TagsToAttributes )
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( Pair.Value() ).AddLambda(
			[ this, Pair ]( const FOnAttributeChangeData &Data ) 
			{ 
				//BroadcastAttributeInfo( Pair.Key, Pair.Value() );
				FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag( Pair.Key );
				Info.AttributeValue = Pair.Value().GetNumericValue( GetAuraAS() );
				AttributeInfoDelegate.Broadcast( Info );
			} 
		);
	}

	GetAuraPS()->OnAttributePointsChangedDelegate.AddLambda( [ this ]( int32 Points ) { AttributePointsChangedDelegate.Broadcast( Points ); } );
}

void UAttributeMenuWidgetController::UpgradeAttribute( const FGameplayTag &AttributeTag )
{
	UAuraAbilitySystemComponent *AuraASC = CastChecked<UAuraAbilitySystemComponent>( AbilitySystemComponent );
	AuraASC->UpgradeAttribute(AttributeTag);
}

//void UAttributeMenuWidgetController::BroadcastAttributeInfo( const FGameplayTag &AttributeTag, const FGameplayAttribute &Attribute ) const
//{ 
//	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag( AttributeTag );
//	Info.AttributeValue = Attribute.GetNumericValue( AttributeSet );
//	AttributeInfoDelegate.Broadcast( Info );
//}
