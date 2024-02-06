#include "Character/AuraCharacterBase.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "AuraGameplayTags.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel( ECC_Camera, ECR_Ignore );
	GetCapsuleComponent()->SetGenerateOverlapEvents( false );
	GetMesh()->SetCollisionResponseToChannel( ECC_Camera, ECR_Ignore );
	GetMesh()->SetCollisionResponseToChannel( ECC_Projectile, ECR_Overlap );
	GetMesh()->SetGenerateOverlapEvents( true );

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>( "Weapon" );
	Weapon->SetupAttachment( GetMesh(), FName( "WeaponHandSocket" ) );
	Weapon->SetCollisionEnabled( ECollisionEnabled::NoCollision );
}

UAbilitySystemComponent *AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent; 
}

void AAuraCharacterBase::Die()
{
	Weapon->DetachFromComponent( FDetachmentTransformRules( EDetachmentRule::KeepWorld, true ) );
	MulticastHandleDeath();
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor *AAuraCharacterBase::GetAvatar_Implementation()
{
	return this;
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation( const FGameplayTag &MontageTag )
{
	// TODO: Could make this a TMap
	const FAuraGameplayTags &GameplayTags = FAuraGameplayTags::Get();
	if( MontageTag.MatchesTagExact( GameplayTags.CombatSocket_Weapon ) && IsValid( Weapon ) )
	{
		return Weapon->GetSocketLocation( WeaponTipSocketName );
	}
	if( MontageTag.MatchesTagExact( GameplayTags.CombatSocket_RightHand ) )
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if( MontageTag.MatchesTagExact( GameplayTags.CombatSocket_LeftHand ) )
	{
		return GetMesh()->GetSocketLocation( LeftHandSocketName );
	}
	if( MontageTag.MatchesTagExact( GameplayTags.CombatSocket_Tail ) )
	{
		return GetMesh()->GetSocketLocation( TailSocketName );
	}
	return FVector();
}

UAnimMontage *AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

TArray<FTaggedMontage> AAuraCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

UNiagaraSystem *AAuraCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

FTaggedMontage AAuraCharacterBase::GetTaggedMontageByTag_Implementation( const FGameplayTag &MontageTag )
{
	for( FTaggedMontage TaggedMontage : AttackMontages )
	{
		if( TaggedMontage.MontageTag == MontageTag )
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation( this, DeathSound, GetActorLocation(), GetActorRotation() );

	Weapon->SetSimulatePhysics( true );
	Weapon->SetEnableGravity( true );
	Weapon->SetCollisionEnabled( ECollisionEnabled::PhysicsOnly );

	GetMesh()->SetSimulatePhysics( true );
	GetMesh()->SetEnableGravity( true );
	GetMesh()->SetCollisionEnabled( ECollisionEnabled::PhysicsOnly );
	GetMesh()->SetCollisionResponseToChannel( ECC_WorldStatic, ECR_Block );

	GetCapsuleComponent()->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	Dissolve();
	bDead = true;
}

void AAuraCharacterBase::BeginPlay()
{ 
	Super::BeginPlay();
}

void AAuraCharacterBase::InitAbilityActorInfo()
{ }

void AAuraCharacterBase::ApplyEffectToSelf( TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level ) const
{ 
	check( IsValid( GetAbilitySystemComponent() ) );
	check( GameplayEffectClass );
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject( this );
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec( GameplayEffectClass, Level, ContextHandle );
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget( *SpecHandle.Data.Get(), GetAbilitySystemComponent() );
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{ 
	ApplyEffectToSelf( DefaultPrimaryAttributes, 1.f );
	ApplyEffectToSelf( DefaultSecondaryAttributes, 1.f );
	ApplyEffectToSelf( DefaultVitalAttributes, 1.f );
}

void AAuraCharacterBase::AddCharacterAbilities()
{ 
	UAuraAbilitySystemComponent *AuraASC = CastChecked<UAuraAbilitySystemComponent>( AbilitySystemComponent );
	if( !HasAuthority() ) return;

	AuraASC->AddCharacterAbilities( StartupAbilities );
}

void AAuraCharacterBase::Dissolve()
{
	if( IsValid( DissolveMaterialInstance ) )
	{
		UMaterialInstanceDynamic *DynamicMatInst = UMaterialInstanceDynamic::Create( DissolveMaterialInstance, this );
		GetMesh()->SetMaterial( 0, DynamicMatInst );
		StartDissolveTimeline( DynamicMatInst );
	}
	if( IsValid( WeaponDissolveMaterialInstance ) )
	{
		UMaterialInstanceDynamic *DynamicMatInst = UMaterialInstanceDynamic::Create( WeaponDissolveMaterialInstance, this );
		Weapon->SetMaterial( 0, DynamicMatInst );
		StartWeaponDissolveTimeline( DynamicMatInst );
	}
}
