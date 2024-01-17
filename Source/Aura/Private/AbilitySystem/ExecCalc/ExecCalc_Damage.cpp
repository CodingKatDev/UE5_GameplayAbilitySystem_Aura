#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"


struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF( Armor );
	DECLARE_ATTRIBUTE_CAPTUREDEF( ArmorPenetration );
	DECLARE_ATTRIBUTE_CAPTUREDEF( BlockChance );

	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, Armor, Target, false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, ArmorPenetration, Source, false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, BlockChance, Target, false );
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
}

void UExecCalc_Damage::Execute_Implementation( const FGameplayEffectCustomExecutionParameters & ExecutionParams, 
											   FGameplayEffectCustomExecutionOutput & OutExecutionOutput ) const
{
	const UAbilitySystemComponent *SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent *TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor *SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor *TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

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

	// ArmorPenetration ignores a percentage of the Target's Armor
	const float EffectiveArmor = TargetArmor *= ( 100 - SourceArmorPenetration * 0.25f ) / 100.f;

	// Armor ignores a percentage of incoming Damage
	Damage *= ( 100 - EffectiveArmor * 0.333f ) / 100.f;

	const FGameplayModifierEvaluatedData EvaluatedData( UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier( EvaluatedData );
}
