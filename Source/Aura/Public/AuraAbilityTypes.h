#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"


class UGameplayEffect;


USTRUCT( BlueprintType )
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams(){}

	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY()
	float BaseDamage = 0.f;
	
	UPROPERTY()
	float AbilityLevel = 1.f;

	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY()
	float DebuffChance = 0.f;
	
	UPROPERTY()
	float DebuffDamage = 0.f;
	
	UPROPERTY()
	float DebuffDuration = 0.f;
	
	UPROPERTY()
	float DebuffFrequency = 0.f;

	UPROPERTY()
	float DeathImpulseMagnitude = 0.f;

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY()
	float KnockbackChance = 0.f;

	UPROPERTY()
	float KnockbackForceMagnitude = 0.f;

	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;
};


USTRUCT( BlueprintType )
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	float GetDebuffDamage() const { return DebuffDamage; }
	float GetDebuffDuration() const { return DebuffDuration; }
	float GetDebuffFrequency() const { return DebuffFrequency; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FVector GetDeathImpulse() const { return DeathImpulse; }
	FVector GetKnockbackForce() const { return KnockbackForce; }

	void SetIsBlockedHit( bool bInIsBlockedHit ) { bIsBlockedHit = bInIsBlockedHit; }
	void SetIsCriticalHit( bool bInIsCriticalHit ) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsSuccessfulDebuff( bool bInIsDebuff ) { bIsSuccessfulDebuff = bInIsDebuff; }
	void SetDebuffDamage( float InDamage ) { DebuffDamage = InDamage; }
	void SetDebuffDuration( float InDuration ) { DebuffDuration = InDuration; }
	void SetDebuffFrequency( float InFrequency ) { DebuffFrequency = InFrequency; }
	void SetDamageType( TSharedPtr<FGameplayTag> InDamageType ) { DamageType = InDamageType; }
	void SetDeathImpulse( const FVector &InImpulse ) { DeathImpulse = InImpulse; }
	void SetKnockbackForce( const FVector &InForce ) { KnockbackForce = InForce; }

	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct *GetScriptStruct() const
	{ 
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FAuraGameplayEffectContext *Duplicate() const
	{
		FAuraGameplayEffectContext *NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if( GetHitResult() )
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult( *GetHitResult(), true );
		}
		return NewContext;
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize( FArchive &Ar, class UPackageMap *Map, bool &bOutSuccess );

protected:
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;

	TSharedPtr<FGameplayTag> DamageType;

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;
};


template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
