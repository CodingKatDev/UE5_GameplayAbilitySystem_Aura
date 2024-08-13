#pragma once

#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"


class UAbilitySystemComponent;
class UAnimMontage;
class UNiagaraSystem;

USTRUCT( BlueprintType )
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	UAnimMontage *Montage = nullptr;
	
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	FGameplayTag MontageTag;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	FGameplayTag SocketTag;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	USoundBase *ImpactSound = nullptr;
};


DECLARE_MULTICAST_DELEGATE_OneParam( FOnASCRegisteredSignature, UAbilitySystemComponent * );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnDeathSignature, AActor *, DeadActor );


// This class does not need to be modified.
UINTERFACE( MinimalAPI, BlueprintType  )
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};


class AURA_API ICombatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintNativeEvent )
	int32 GetPlayerLevel();

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	ECharacterClass GetCharacterClass();

	virtual void Die( const FVector &DeathImpulse ) = 0;
	virtual FOnDeathSignature &GetOnDeathDelegate() = 0;

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	bool IsDead() const;

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	AActor *GetAvatar();

	UFUNCTION( BlueprintImplementableEvent, BlueprintCallable )
	void UpdateFacingTarget( const FVector &Target );

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	FVector GetCombatSocketLocation( const FGameplayTag &MontageTag );

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	UAnimMontage *GetHitReactMontage();

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	TArray<FTaggedMontage>GetAttackMontages();

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	FTaggedMontage GetTaggedMontageByTag( const FGameplayTag &MontageTag );

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	UNiagaraSystem *GetBloodEffect();

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	int32 GetMinionCount();

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	void UpdateMinionCount(int32 Amount);

	virtual FOnASCRegisteredSignature GetOnASCRegisteredDelegate() = 0;

	UFUNCTION( BlueprintImplementableEvent, BlueprintCallable )
	void SetInShockLoop( bool bInLoop );

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	USkeletalMeshComponent *GetWeapon();
};