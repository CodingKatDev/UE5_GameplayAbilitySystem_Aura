#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"


USTRUCT( BlueprintType )
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	UAnimMontage *Montage = nullptr;
	
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	FGameplayTag MontageTag;
};


// This class does not need to be modified.
UINTERFACE( MinimalAPI, BlueprintType  )
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};


class UAnimMontage;


class AURA_API ICombatInterface
{
	GENERATED_BODY()

public:
	virtual int32 GetPlayerLevel();

	virtual void Die() = 0;

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	bool IsDead() const;

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	AActor *GetAvatar();

	UFUNCTION( BlueprintImplementableEvent, BlueprintCallable )
	void UpdateFacingTarget ( const FVector &Target );

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	FVector GetCombatSocketLocation( const FGameplayTag &MontageTag );

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	UAnimMontage *GetHitReactMontage();

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	TArray<FTaggedMontage>GetAttackMontages();
};
