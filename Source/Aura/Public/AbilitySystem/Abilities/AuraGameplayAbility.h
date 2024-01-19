#pragma once

#include "Abilities/GameplayAbility.h"
#include "CoreMinimal.h"
#include "AuraGameplayAbility.generated.h"


UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY( EditDefaultsOnly, Category = "Input" )
	FGameplayTag StartupInputTag;
};
