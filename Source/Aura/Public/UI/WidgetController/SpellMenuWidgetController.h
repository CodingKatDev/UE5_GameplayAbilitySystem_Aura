#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"


class UAbilityInfo;
class UAuraUserWidget;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FSpellGlobeSelectedSignature, UAuraUserWidget*, SpellGlobe );


UCLASS( BlueprintType, Blueprintable )
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY( BlueprintAssignable, Category = "GAS|Spell Menu" )
	FOnPlayerStatChangedSignature SpellPointsChangedDelegate;

	UPROPERTY( BlueprintAssignable, Category = "GAS|Spell Menu" )
	FSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;

	UFUNCTION( BlueprintCallable, Category = "GAS|Spell Menu" )
	void SpellGlobeSelected( UAuraUserWidget *SpellGlobe );
};
