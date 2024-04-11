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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FWaitForEquipSelectionSignature, const FGameplayTag &, AbilityType );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FButtonsEnabledStatusSignature, bool, bSpendPointButtonEnable, bool, bEquipButtonEnabled );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FAbilityDescriptionsSignature, FString, DescriptionString, FString, NextLevelDescriptionString );


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
	FWaitForEquipSelectionSignature WaitForEquipSelectionDelegate;

	UPROPERTY( BlueprintAssignable, Category = "GAS|Spell Menu" )
	FWaitForEquipSelectionSignature StopWaitingForEquipSelectionDelegate;

	UPROPERTY( BlueprintAssignable, Category = "GAS|Spell Menu" )
	FButtonsEnabledStatusSignature ButtonsEnabledStatusDelegate;

	UPROPERTY( BlueprintAssignable, Category = "GAS|Spell Menu" )
	FAbilityDescriptionsSignature AbilityDescriptionsDelegate;

	UFUNCTION( BlueprintCallable, Category = "GAS|Spell Menu" )
	void SpellGlobeSelected( const FGameplayTag &SpellGlobeAbilityTag );

	UFUNCTION( BlueprintCallable, Category = "GAS|Spell Menu" )
	void ClearSelectedGlobe();

	UFUNCTION( BlueprintCallable, Category = "GAS|Spell Menu" )
	void SpendPointButtonPressed();

	UFUNCTION( BlueprintCallable, Category = "GAS|Spell Menu" )
	void EquipButtonPressed();

	UFUNCTION( BlueprintCallable, Category = "GAS|Spell Menu" )
	void SpellRowGlobePressed( const FGameplayTag &SlotTag, const FGameplayTag &AbilityType );

	void OnAbilityEquipped( const FGameplayTag &AbilityTag, const FGameplayTag &Status, const FGameplayTag &Slot, const FGameplayTag &PreviousSlot );

private:
	FSelectedAbility SelectedAbility = { FAuraGameplayTags::Get().Abilities_None, FAuraGameplayTags::Get().Abilities_Status_Locked };
	FGameplayTag SelectedSlot;
	int32 CurrentSpellPoints = 0;
	bool bWaitingForEquipSelection = false;
	
	void SpellGlobeAbilityDescriptions( const FGameplayTag &AbilityTag );
	void ShouldEnableButtons( const FGameplayTag &AbilityStatus );	
};
