#pragma once

#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "CoreMinimal.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"


UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintCallable )
	void CauseDamage( AActor *TargetActor );
	
protected:
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY( EditDefaultsOnly, Category = "Damage" )
	FGameplayTag DamageType;

	UPROPERTY( EditDefaultsOnly, Category = "Damage" )
	FScalableFloat Damage;

	UFUNCTION( BlueprintPure )
	FTaggedMontage GetRandomTaggedMontageFromArray( const TArray<FTaggedMontage> &TaggedMontages ) const;
};