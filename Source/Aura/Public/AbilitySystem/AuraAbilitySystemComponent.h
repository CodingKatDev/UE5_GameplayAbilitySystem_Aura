#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AuraAbilitySystemComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam( FEffectAssetTags, const FGameplayTagContainer & /*AssetTags*/ );
DECLARE_MULTICAST_DELEGATE( FAbilitiesGiven );
DECLARE_DELEGATE_OneParam( FForEachAbility, const FGameplayAbilitySpec & );
DECLARE_MULTICAST_DELEGATE_ThreeParams( FAbilityStatusChanged, const FGameplayTag & /*AbilityTag*/, const FGameplayTag & /*StatusTag*/, int32 /*AbilityLevel*/ );
DECLARE_MULTICAST_DELEGATE_FourParams( FABilityEquipped, const FGameplayTag & /*AbilityTag*/, const FGameplayTag & /*Status*/, const FGameplayTag & /*Slot*/, const FGameplayTag & /*PrevSlot*/ );


UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTagsDelegate;
	FAbilitiesGiven AbilitiesGivenDelegate;
	FAbilityStatusChanged AbilityStatusChangedDelegate;
	FABilityEquipped AbilityEquippedDelegate;

	void AddCharacterAbilities( const TArray<TSubclassOf<UGameplayAbility>> &StartupAbilities );
	void AddCharacterPassiveAbilities( const TArray<TSubclassOf<UGameplayAbility>> &StartupPassiveAbilities );

	bool bStartupAbilitiesGiven = false;

	void AbilityInputTagPressed( const FGameplayTag &InputTag );
	void AbilityInputTagHeld( const FGameplayTag &InputTag );
	void AbilityInputTagReleased( const FGameplayTag &InputTag );
	void ForEachAbility( const FForEachAbility &Delegate );

	static FGameplayTag GetAbilityTagFromSpec( const FGameplayAbilitySpec &AbilitySpec );
	static FGameplayTag GetInputTagFromSpec( const FGameplayAbilitySpec &AbilitySpec );
	static FGameplayTag GetStatusFromSpec( const FGameplayAbilitySpec &AbilitySpec );
	FGameplayTag GetStatusFromAbilityTag( const FGameplayTag &AbilityTag );
	FGameplayTag GetInputTagFromAbilityTag( const FGameplayTag &AbilityTag );

	FGameplayAbilitySpec *GetSpecFromAbilityTag( const FGameplayTag &AbilityTag );

	void UpgradeAttribute( const FGameplayTag &AttributeTag );

	UFUNCTION( Server, Reliable )
	void ServerUpgradeAttribute( const FGameplayTag &AttributeTag );

	void UpdateAbilityStatuses( int32 Level );

	UFUNCTION( Server, Reliable )
	void ServerSpendSpellPoint( const FGameplayTag &AbilityTag );

	UFUNCTION( Server, Reliable )
	void ServerEquipAbility( const FGameplayTag &AbilityTag, const FGameplayTag &Slot );

	void ClientEquipAbility( const FGameplayTag &AbilityTag, const FGameplayTag &Status, const FGameplayTag &Slot, const FGameplayTag &PreviousSlot );

	bool GetDescriptionsByAbilityTag( const FGameplayTag &AbilityTag, FString &OutDescription, FString &OutNextLevelDescription );

	void ClearSlot( FGameplayAbilitySpec *Spec );
	void ClearAbilitiesOfSlot( const FGameplayTag &Slot );
	static bool AbilityHasSlot( FGameplayAbilitySpec *Spec, const FGameplayTag &Slot );

protected:
	virtual void OnRep_ActivateAbilities() override;

	UFUNCTION( Client, Reliable )
	void ClientUpdateAbilityStatus( const FGameplayTag &AbilityTag, const FGameplayTag &StatusTag, int32 AbilityLevel );
	
	UFUNCTION( Client, Reliable )
	void ClientEffectApplied( UAbilitySystemComponent *AbilitySystemComponent, const FGameplayEffectSpec &EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle );
};