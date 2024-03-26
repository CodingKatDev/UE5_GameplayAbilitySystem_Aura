#pragma once

#include "AuraGameplayTags.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"


class UAbilityInfo;
class UAuraUserWidget;

struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FSpellGlobeSelectedSignature, FGameplayTag, SpellGlobeAbilityTag );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FButtonEnabledStatusSignature, bool, bSpendPointButtonEnable, bool, bEquipButtonEnabled );


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

	UPROPERTY( BlueprintAssignable, Category = "GAS|Spell Menu" )
	FButtonEnabledStatusSignature ButtonEnabledStatusDelegate;

	UFUNCTION( BlueprintCallable, Category = "GAS|Spell Menu" )
	void SpellGlobeSelected( const FGameplayTag &SpellGlobeAbilityTag );

private:
	FSelectedAbility SelectedAbility = { FAuraGameplayTags::Get().Abilities_None, FAuraGameplayTags::Get().Abilities_Status_Locked };
	int32 CurrentSpellPoints = 0;

	void SpellGlobeAbilityStatus();
	void UpdateButtonEnabledStatus( const FGameplayTag &AbilityStatus, int32 SpellPoints );
};
