#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Interaction/CombatInterface.h"


struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF( Armor );
	DECLARE_ATTRIBUTE_CAPTUREDEF( ArmorPenetration );
	DECLARE_ATTRIBUTE_CAPTUREDEF( BlockChance );
	DECLARE_ATTRIBUTE_CAPTUREDEF( CriticalHitChance );
	DECLARE_ATTRIBUTE_CAPTUREDEF( CriticalHitResistance );
	DECLARE_ATTRIBUTE_CAPTUREDEF( CriticalHitDamage );

	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, Armor, Target, false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, ArmorPenetration, Source, false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, BlockChance, Target, false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, CriticalHitChance, Source, false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, CriticalHitResistance, Target, false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, CriticalHitDamage, Source, false );
	}
};

static const AuraDamageStatics &DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add( DamageStatics().ArmorDef );
	RelevantAttributesToCapture.Add( DamageStatics().ArmorPenetrationDef );
	RelevantAttributesToCapture.Add( DamageStatics().BlockChanceDef );
	RelevantAttributesToCapture.Add( DamageStatics().CriticalHitChanceDef );
	RelevantAttributesToCapture.Add( DamageStatics().CriticalHitResistanceDef );
	RelevantAttributesToCapture.Add( DamageStatics().CriticalHitDamageDef );
}

void UExecCalc_Damage::Execute_Implementation( const FGameplayEffectCustomExecutionParameters & ExecutionParams, 
											   FGameplayEffectCustomExecutionOutput & OutExecutionOutput ) const
{
	const UAbilitySystemComponent *SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent *TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor *SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor *TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	ICombatInterface *SourceCombatInterface = Cast<ICombatInterface>( SourceAvatar );
	ICombatInterface *TargetCombatInterface = Cast<ICombatInterface>( TargetAvatar );

	const FGameplayEffectSpec &Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer *SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer *TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	// Get Damage Set by Caller Magnitude
	float Damage = Spec.GetSetByCallerMagnitude( FAuraGameplayTags::Get().Damage );

	// Capture BlockChance on Target, and determine if there was a successful Block
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( DamageStatics().BlockChanceDef, EvaluationParams, TargetBlockChance );
	TargetBlockChance = FMath::Max<float>( TargetBlockChance, 0.f );

	const bool bBlocked = FMath::FRandRange( UE_SMALL_NUMBER, 100.0f ) <= TargetBlockChance;
	
	// If Blocked, halve Damage
	Damage = bBlocked ? Damage / 2.f : Damage;

	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( DamageStatics().ArmorDef, EvaluationParams, TargetArmor );
	TargetArmor = FMath::Max<float>( TargetArmor, 0.f );

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( DamageStatics().ArmorPenetrationDef, EvaluationParams, SourceArmorPenetration );
	SourceArmorPenetration = FMath::Max<float>( SourceArmorPenetration, 0.f );

	const UCharacterClassInfo *CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo( SourceAvatar );
	const FRealCurve *ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve( FName( "ArmorPenetration" ), FString() );
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval( SourceCombatInterface->GetPlayerLevel() );

	// ArmorPenetration ignores a percentage of the Target's Armor
	const float EffectiveArmor = TargetArmor * ( 100 - SourceArmorPenetration * ArmorPenetrationCoefficient ) / 100.f;

	const FRealCurve *EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve( FName( "EffectiveArmor" ), FString() );
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval( TargetCombatInterface->GetPlayerLevel() );

	// Armor ignores a percentage of incoming Damage
	Damage *= ( 100 - EffectiveArmor * EffectiveArmorCoefficient ) / 100.f;

	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( DamageStatics().CriticalHitChanceDef, EvaluationParams, SourceCriticalHitChance );
	SourceCriticalHitChance = FMath::Max( SourceCriticalHitChance, 0.f );

	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( DamageStatics().CriticalHitResistanceDef, EvaluationParams, TargetCriticalHitResistance );
	TargetCriticalHitResistance = FMath::Max( TargetCriticalHitResistance, 0.f );
	
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( DamageStatics().CriticalHitDamageDef, EvaluationParams, SourceCriticalHitDamage );
	SourceCriticalHitDamage = FMath::Max( SourceCriticalHitDamage, 0.f );
	
	const FRealCurve *CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve( FName( "CriticalHitResistance" ), FString() );
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval( TargetCombatInterface->GetPlayerLevel() );
	
	// Critical Hit Resistance reduces Critical Hit Chance by a certain percentage
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const bool bCriticalHit = FMath::FRandRange( UE_SMALL_NUMBER, 100.f ) <= EffectiveCriticalHitChance;

	// Double damage plus a bonus if Critical Hit
	Damage = bCriticalHit ? Damage * 2.f + SourceCriticalHitDamage : Damage;

	const FGameplayModifierEvaluatedData EvaluatedData( UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier( EvaluatedData );
}
