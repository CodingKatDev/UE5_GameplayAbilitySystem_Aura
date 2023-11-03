#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"


void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{ 
	OnGameplayEffectAppliedDelegateToSelf.AddUObject( this, &UAuraAbilitySystemComponent::EffectApplied );
}

void UAuraAbilitySystemComponent::AddCharacterAbilities( const TArray<TSubclassOf<UGameplayAbility>> &StartupAbilities )
{ 
	for( TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities )
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec( AbilityClass, 1 );
		// GiveAbility( AbilitySpec ); // FGameplayAbilitySpec CAN be const 
		GiveAbilityAndActivateOnce( AbilitySpec ); // FGameplayAbilitySpec CANNOT be const
	}
}

void UAuraAbilitySystemComponent::EffectApplied( UAbilitySystemComponent *AbilitySystemComponent, const FGameplayEffectSpec &EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle )
{ 
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags( TagContainer );

	EffectAssetTags.Broadcast( TagContainer );
}
