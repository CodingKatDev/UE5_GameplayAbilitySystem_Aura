#include "Character/AuraCharacter.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "NiagaraComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"


AAuraCharacter::AAuraCharacter()
{ 
	this->Tags.Add( "Player" );

	CameraArm = CreateDefaultSubobject<USpringArmComponent>( TEXT( "CameraArm" ) );
	CameraArm->SetupAttachment( GetRootComponent() );
	CameraArm->TargetArmLength = 800.f;
	CameraArm->SetRelativeRotation( FRotator( -45.f, 0.f, 0.f ) );
	CameraArm->bUsePawnControlRotation = false;
	CameraArm->bEnableCameraLag = true;
	CameraArm->bInheritPitch = false;
	CameraArm->bInheritYaw = false;
	CameraArm->bInheritRoll = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>( TEXT( "FollowCamera" ) );
	FollowCamera->SetupAttachment( CameraArm, USpringArmComponent::SocketName );
	FollowCamera->bUsePawnControlRotation = false;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>( "LevelUpNiagaraComponent" );
	LevelUpNiagaraComponent->SetupAttachment( GetRootComponent() );
	LevelUpNiagaraComponent->bAutoActivate = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator( 0.f, 400.f, 0.f );
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CharacterClass = ECharacterClass::Elementalist;
}

void AAuraCharacter::PossessedBy( AController *NewController )
{ 
	Super::PossessedBy( NewController );

	// Init ability actor info for the server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{ 
	Super::OnRep_PlayerState();

	// Init ability actor info for the client
	InitAbilityActorInfo();
}

int32 AAuraCharacter::FindLevelForXP_Implementation( int32 InXP ) const
{
	const AAuraPlayerState *AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check( AuraPlayerState );
	return AuraPlayerState->LevelUpInfo->FindLevelForXP( InXP );
}

int32 AAuraCharacter::GetXP_Implementation() const
{
	const AAuraPlayerState *AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check( AuraPlayerState );
	return AuraPlayerState->GetXP();
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation( int32 Level ) const
{
	const AAuraPlayerState *AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check( AuraPlayerState );
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[ Level ].AttributePointAward;
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation( int32 Level ) const
{
	const AAuraPlayerState *AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check( AuraPlayerState );
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[ Level ].SpellPointAward;
}

int32 AAuraCharacter::GetAttrributePoints_Implementation() const
{
	AAuraPlayerState *AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check( AuraPlayerState );
	return AuraPlayerState->GetAttributePoints();
}

int32 AAuraCharacter::GetSpellPoints_Implementation() const
{
	AAuraPlayerState *AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check( AuraPlayerState );
	return AuraPlayerState->GetSpellPoints();
}

void AAuraCharacter::AddToPlayerLevel_Implementation( int32 InPlayerLevel )
{
	AAuraPlayerState *AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check( AuraPlayerState );
	AuraPlayerState->AddToLevel( InPlayerLevel );

	if( UAuraAbilitySystemComponent *AuraASC = Cast<UAuraAbilitySystemComponent>( GetAbilitySystemComponent() ) )
	{
		AuraASC->UpdateAbilityStatuses( AuraPlayerState->GetPlayerLevel() );
	}
}

void AAuraCharacter::AddToAttributePoints_Implementation( int32 InAttributePoints )
{
	AAuraPlayerState *AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check( AuraPlayerState );
	AuraPlayerState->AddToAttributePoints( InAttributePoints );
}

void AAuraCharacter::AddToSpellPoints_Implementation( int32 InSpellPoints )
{
	AAuraPlayerState *AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check( AuraPlayerState );
	AuraPlayerState->AddToSpellPoints( InSpellPoints );
}

void AAuraCharacter::AddToXP_Implementation( int32 InXP )
{
	AAuraPlayerState *AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check( AuraPlayerState );
	AuraPlayerState->AddToXP( InXP );
}

void AAuraCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	const AAuraPlayerState *AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check( AuraPlayerState );
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::InitAbilityActorInfo()
{ 
	AAuraPlayerState *AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check( AuraPlayerState );
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo( AuraPlayerState, this );
	Cast<UAuraAbilitySystemComponent>( AuraPlayerState->GetAbilitySystemComponent() )->AbilityActorInfoSet();
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	OnASCRegistered.Broadcast( AbilitySystemComponent );

	// In multi-player client's player controller will be valid on their machine, but other players' controllers won't
	// Only want to continue if not null, but don't assert as this will cause unwanted crash
	if( AAuraPlayerController *AuraPlayerController = Cast<AAuraPlayerController>( GetController() ) )
	{
		if( AAuraHUD *AuraHUD = Cast<AAuraHUD>( AuraPlayerController->GetHUD() ) )
		{
			AuraHUD->InitOverlay( AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet );
		}
	}
	InitializeDefaultAttributes();
}

void AAuraCharacter::MulticastLevelUpParticles_Implementation() const
{
	if( IsValid( LevelUpNiagaraComponent ) )
	{
		const FVector CameraLocation = FollowCamera->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraRotation = ( CameraLocation - NiagaraSystemLocation ).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation( ToCameraRotation );
		LevelUpNiagaraComponent->Activate( true );
	}
}
