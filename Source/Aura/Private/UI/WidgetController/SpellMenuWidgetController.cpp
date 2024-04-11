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

	GetAuraASC()->AbilityEquippedDelegate.AddUObject( this, &USpellMenuWidgetController::OnAbilityEquipped );

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
	if( bWaitingForEquipSelection )
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag( SpellGlobeAbilityTag ).AbilityType;
		StopWaitingForEquipSelectionDelegate.Broadcast( SelectedAbilityType );
		bWaitingForEquipSelection = false;
	}

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
	if( bWaitingForEquipSelection )
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag( SelectedAbility.Ability ).AbilityType;
		StopWaitingForEquipSelectionDelegate.Broadcast( SelectedAbilityType );
		bWaitingForEquipSelection = false;
	}

	SelectedAbility.Ability = FAuraGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FAuraGameplayTags::Get().Abilities_Status_Locked;

	ButtonsEnabledStatusDelegate.Broadcast( false, false );
	AbilityDescriptionsDelegate.Broadcast( FString(), FString() );
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if( GetAuraASC() )
	{
		GetAuraASC()->ServerSpendSpellPoint( SelectedAbility.Ability );
	}
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag( SelectedAbility.Ability ).AbilityType;

	WaitForEquipSelectionDelegate.Broadcast( AbilityType );
	bWaitingForEquipSelection = true;

	const FGameplayTag SelectedStatus = GetAuraASC()->GetStatusFromAbilityTag( SelectedAbility.Ability );
	if( SelectedStatus.MatchesTagExact( FAuraGameplayTags::Get().Abilities_Status_Equipped ) )
	{
		SelectedSlot = GetAuraASC()->GetInputTagFromAbilityTag( SelectedAbility.Ability );
	}
}

void USpellMenuWidgetController::SpellRowGlobePressed( const FGameplayTag &SlotTag, const FGameplayTag &AbilityType )
{
	if( !bWaitingForEquipSelection ) return;
	// Check selected ability against the slot's ability type. (don't equip an offensive spell in a passive slot and vise versa)
	const FGameplayTag &SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag( SelectedAbility.Ability ).AbilityType;
	if( !SelectedAbilityType.MatchesTagExact( AbilityType ) ) return;

	GetAuraASC()->ServerEquipAbility( SelectedAbility.Ability, SlotTag );
}

void USpellMenuWidgetController::OnAbilityEquipped( const FGameplayTag &AbilityTag, const FGameplayTag &Status, const FGameplayTag &Slot, const FGameplayTag &PreviousSlot )
{
	bWaitingForEquipSelection = false;

	const FAuraGameplayTags &GameplayTags = FAuraGameplayTags::Get();

	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	// Broadcast empty info if PreviousSlot is a valid slot. Only if equipping an already-equipped spell
	AbilityInfoDelegate.Broadcast( LastSlotInfo );

	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag( AbilityTag );
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast( Info );

	StopWaitingForEquipSelectionDelegate.Broadcast( AbilityInfo->FindAbilityInfoForTag( AbilityTag ).AbilityType );
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
