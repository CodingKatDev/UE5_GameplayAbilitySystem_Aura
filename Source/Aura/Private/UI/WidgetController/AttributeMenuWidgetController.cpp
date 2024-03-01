#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Player//AuraPlayerState.h"


void UAttributeMenuWidgetController::BroadcastInitialValues()
{ 
	UAuraAttributeSet *AS = CastChecked<UAuraAttributeSet>( AttributeSet );
	check( AttributeInfo );

	for( auto &Pair : AS->TagsToAttributes )
	{
		//BroadcastAttributeInfo( Pair.Key, Pair.Value() );
		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag( Pair.Key );
		Info.AttributeValue = Pair.Value().GetNumericValue( AS );
		AttributeInfoDelegate.Broadcast( Info );
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{ 
	UAuraAttributeSet *AS = CastChecked<UAuraAttributeSet>( AttributeSet );
	check( AttributeInfo );

	for( auto &Pair : AS->TagsToAttributes )
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( Pair.Value() ).AddLambda(
			[ this, Pair, AS ]( const FOnAttributeChangeData &Data )
			{
				//BroadcastAttributeInfo( Pair.Key, Pair.Value() );
				FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag( Pair.Key );
				Info.AttributeValue = Pair.Value().GetNumericValue( AS );
				AttributeInfoDelegate.Broadcast( Info );
			} 
		);
	}

	AAuraPlayerState *AuraPlayerState = CastChecked<AAuraPlayerState>( AttributeSet );
	AuraPlayerState->OnAttributePointsChangedDelegate.AddLambda(
		[ this ]( int32 Points )
		{
			AttributePointsChangedDelegate.Broadcast( Points );
		}
	);
}

//void UAttributeMenuWidgetController::BroadcastAttributeInfo( const FGameplayTag &AttributeTag, const FGameplayAttribute &Attribute ) const
//{ 
//	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag( AttributeTag );
//	Info.AttributeValue = Attribute.GetNumericValue( AttributeSet );
//	AttributeInfoDelegate.Broadcast( Info );
//}
