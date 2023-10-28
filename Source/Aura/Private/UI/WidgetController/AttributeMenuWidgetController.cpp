#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"


void UAttributeMenuWidgetController::BroadcastInitialValues()
{ 
	UAuraAttributeSet *AS = CastChecked<UAuraAttributeSet>( AttributeSet );
	
	check( AttributeInfo );

	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag( FAuraGameplayTags::Get().Attributes_Primary_Strength );
	Info.AttributeValue = AS->GetStrength();
	AttributeInfoDelegate.Broadcast( Info );
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{ 
	
}
