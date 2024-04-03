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

	virtual FString GetDescription( int32 Level );
	virtual FString GetNextLevelDescription( int32 Level );
	static FString GetLockedDesciption( int32 Level );

protected:
	float GetManaCost( float InLevel = 1.f ) const;
	float GetCooldown( float InLevel = 1.f ) const;
};
