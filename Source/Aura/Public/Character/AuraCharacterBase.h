#pragma once

#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"


class UAbilitySystemComponent;
class UAnimMontage;
class UAttributeSet;
class UDebuffNiagaraComponent;
class UGameplayAbility;
class UGameplayEffect;
class UNiagaraSystem;


UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

	virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;
	UAttributeSet *GetAttributeSet() const { return AttributeSet; }

	/** Combat Interface */
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual void Die() override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor *GetAvatar_Implementation() override;
	virtual FVector GetCombatSocketLocation_Implementation( const FGameplayTag &MontageTag ) override;
	virtual UAnimMontage *GetHitReactMontage_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation( const FGameplayTag &MontageTag ) override;
	virtual UNiagaraSystem *GetBloodEffect_Implementation() override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void UpdateMinionCount_Implementation( int32 Amount ) override;
	virtual FOnASCRegisteredSignature GetOnASCRegisteredDelegate() override;
	//virtual FOnDeathSignature GetOnDeathDelegate() override;
	/** end Combat Interface */

	FOnASCRegisteredSignature OnASCRegistered;
	//FOnDeathSignature OnDeath;

	UFUNCTION( NetMulticast, Reliable )
	virtual void MulticastHandleDeath();

	UPROPERTY( EditAnywhere, Category = "Combat" )
	TArray<FTaggedMontage> AttackMontages;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();
	void ApplyEffectToSelf( TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level ) const;
	virtual void InitializeDefaultAttributes() const;
	void AddCharacterAbilities();

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY( BlueprintReadOnly, EditAnywhere, Category = "Attributes" )
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY( BlueprintReadOnly, EditAnywhere, Category = "Attributes" )
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY( BlueprintReadOnly, EditAnywhere, Category = "Attributes" )
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults" )
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	/* Combat */
	bool bDead = false;

	UPROPERTY( VisibleAnywhere )
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent; 	

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Combat" )
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY( EditAnywhere, Category = "Combat" )
	FName WeaponTipSocketName;

	UPROPERTY( EditAnywhere, Category = "Combat" )
	FName RightHandSocketName;

	UPROPERTY( EditAnywhere, Category = "Combat" )
	FName LeftHandSocketName;

	UPROPERTY( EditAnywhere, Category = "Combat" )
	FName TailSocketName;
	int32 MinionCount = 0;

	/* Dissolve Effects */
	void Dissolve();

	UFUNCTION( BlueprintImplementableEvent )
	void StartDissolveTimeline( UMaterialInstanceDynamic *DynamicMaterialInstance );

	UFUNCTION( BlueprintImplementableEvent )
	void StartWeaponDissolveTimeline( UMaterialInstanceDynamic *DynamicMaterialInstance );

	UPROPERTY( EditAnywhere, BlueprintReadOnly )
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY( EditAnywhere, BlueprintReadOnly )
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Combat" )
	UNiagaraSystem *BloodEffect;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Combat" )
	USoundBase *DeathSound;

private:
	UPROPERTY( EditAnywhere, Category = "Abilities" )
	TArray<TSubclassOf<UGameplayAbility>>StartupAbilities;

	UPROPERTY( EditAnywhere, Category = "Abilities" )
	TArray<TSubclassOf<UGameplayAbility>>StartupPassiveAbilities;

	UPROPERTY( EditAnywhere, Category = "Combat" )
	TObjectPtr<UAnimMontage> HitReactMontage;
};
