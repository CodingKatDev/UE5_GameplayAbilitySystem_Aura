#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem//AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"


AAuraCharacter::AAuraCharacter()
{ 
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator( 0.f, 400.f, 0.f );
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>( TEXT( "CameraArm" ) );
	CameraArm->SetupAttachment( GetCapsuleComponent() );
	CameraArm->TargetArmLength = 750.f;
	CameraArm->SetRelativeRotation( FRotator( -45.f, 0.f, 0.f ) );
	CameraArm->bUsePawnControlRotation = false;
	CameraArm->bEnableCameraLag = true;
	CameraArm->bInheritPitch = false;
	CameraArm->bInheritYaw = false;
	CameraArm->bInheritRoll = false;


	FollowCamera = CreateDefaultSubobject<UCameraComponent>( TEXT( "FollowCamera" ) );
	FollowCamera->SetupAttachment( CameraArm, USpringArmComponent::SocketName );
	FollowCamera->bUsePawnControlRotation = false;
}

void AAuraCharacter::PossessedBy( AController *NewController )
{ 
	Super::PossessedBy( NewController );

	// Init ability actor info for the server
	InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState()
{ 
	Super::OnRep_PlayerState();

	// Init ability actor info for the client
	InitAbilityActorInfo();
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

	// Only want to continue if not null, but don't want to place an assert b/c don't want to crash 
	// In multiplayer client's player controller will be valid on their machine, but other players' won't
	if( AAuraPlayerController *AuraPlayerController = Cast<AAuraPlayerController>( GetController() ) )
	{
		if( AAuraHUD *AuraHUD = Cast<AAuraHUD>( AuraPlayerController->GetHUD() ) )
		{
			AuraHUD->InitOverlay( AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet );
		}
	}
	InitializeDefaultAttributes();
}
