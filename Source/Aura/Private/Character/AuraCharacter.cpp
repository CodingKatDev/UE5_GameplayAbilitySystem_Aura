#include "Character/AuraCharacter.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"


AAuraCharacter::AAuraCharacter()
{ 
	this->Tags.Add( "Player" );

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator( 0.f, 400.f, 0.f );
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>( TEXT( "CameraArm" ) );
	CameraArm->SetupAttachment( GetCapsuleComponent() );
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

void AAuraCharacter::AddToXP_Implementation( int32 InXP )
{
	AAuraPlayerState *AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check( AuraPlayerState );
	AuraPlayerState->AddToXP( InXP );
}

int32 AAuraCharacter::GetPlayerLevel()
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
