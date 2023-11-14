#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Components/SplineComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"


AAuraPlayerController::AAuraPlayerController()
{ 
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>( "Spline" );
}

void AAuraPlayerController::PlayerTick( float DeltaTime )
{ 
	Super::PlayerTick( DeltaTime );
	
	CursorTrace();
	AutoRun();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check( AuraContext );

	UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( GetLocalPlayer() );
	if( Subsystem ) // For Multiplayer - Check, don't Assert
	{
		Subsystem->AddMappingContext( AuraContext, 0 );
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior( EMouseLockMode::DoNotLock );
	InputModeData.SetHideCursorDuringCapture( false );
	SetInputMode( InputModeData );
}

void AAuraPlayerController::SetupInputComponent()
{ 
	Super::SetupInputComponent();

	UAuraInputComponent *AuraInputComponent = CastChecked< UAuraInputComponent>( InputComponent );
	AuraInputComponent->BindAction( MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move );
	AuraInputComponent->BindAbilityActions( InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld );
}

void AAuraPlayerController::Move( const FInputActionValue &InputActionValue )
{ 
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation( 0.f, Rotation.Yaw, 0.f );

	const FVector ForwardDirection = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::X );
	const FVector RightDirection = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::Y );

	if( APawn *ControlledPawn = GetPawn<APawn>() )
	{
		ControlledPawn->AddMovementInput( ForwardDirection, InputAxisVector.Y );
		ControlledPawn->AddMovementInput( RightDirection, InputAxisVector.X );
	}
}

void AAuraPlayerController::CursorTrace()
{ 
	FHitResult CursorHit;
	GetHitResultUnderCursor( ECC_Visibility, false, CursorHit );
	if( !CursorHit.bBlockingHit ) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>( CursorHit.GetActor() );

	/**
	* Line trace from cursor. There are several scenarios:
	* A. LastActor is null && ThisActor is null
	*	- Do nothing
	* B. LastActor is null && ThisActor is valid
	*	- Highlight ThisActor
	* C. LastActor is valid && ThisActor is null
	*	- UnHightlight LastActor
	* D. Both actors are valid, but LastActor != ThisActor
	*	- UnHightlight LastActor, and Highlight ThisActor
	* E. Both actors are valid, and are the same actor
	*	- Do nothing
	**/

	//if( LastActor == nullptr && ThisActor == nullptr ) return; // Case A
	//else if( LastActor == nullptr && ThisActor != nullptr ) // Case B
	//{
	//	ThisActor->HighlightActor();
	//}
	//else if( LastActor != nullptr && ThisActor == nullptr ) // Case C
	//{
	//	LastActor->UnHighlightActor();
	//}
	//else if( LastActor != nullptr && ThisActor != nullptr ) 
	//{
	//	if( LastActor != ThisActor ) // Case D
	//	{
	//		LastActor->UnHighlightActor();
	//		ThisActor->HighlightActor();
	//	}
	//	else // LastActor == ThisActor // Case E
	//		return;		
	//}


	if( ThisActor != LastActor )
	{
		if( LastActor != nullptr )
		{
			LastActor->UnHighlightActor();
		}
		if( ThisActor != nullptr )
		{
			ThisActor->HighlightActor();
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed( FGameplayTag InputTag )
{ 
	if( InputTag.MatchesTagExact( FAuraGameplayTags::Get().Input_Mouse_LeftButton ) )
	{
		bTargeting = ThisActor ? true : false; // if ThisActor =! nullptr, return true, else return false
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased( FGameplayTag InputTag )
{
	if( !InputTag.MatchesTagExact( FAuraGameplayTags::Get().Input_Mouse_LeftButton ) )
	{
		if( GetASC() )
		{
			GetASC()->AbilityInputTagReleased( InputTag );
		}
		return;
	}

	if( bTargeting )
	{
		if( GetASC() )
		{
			GetASC()->AbilityInputTagReleased( InputTag );
		}
	}
	else
	{
		APawn *ControlledPawn = GetPawn();
		if( FollowTime <= ShortPressThreshold && ControlledPawn )
		{
			if( UNavigationPath *NavPath = UNavigationSystemV1::FindPathToLocationSynchronously( this, ControlledPawn->GetActorLocation(), CachedDestination ) )
			{
				Spline->ClearSplinePoints();
				for( const FVector &PointLoc : NavPath->PathPoints )
				{
					Spline->AddSplinePoint( PointLoc, ESplineCoordinateSpace::World );
					DrawDebugSphere( GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f );
				}
				CachedDestination = NavPath->PathPoints[ NavPath->PathPoints.Num() - 1 ];
				bAutoRunning = true;
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld( FGameplayTag InputTag )
{
	if( !InputTag.MatchesTagExact( FAuraGameplayTags::Get().Input_Mouse_LeftButton ) )
	{
		if( GetASC() )
		{
			GetASC()->AbilityInputTagHeld( InputTag );
		}
		return;
	}

	if( bTargeting )
	{
		if( GetASC() )
		{
			GetASC()->AbilityInputTagHeld( InputTag );
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		FHitResult Hit;
		if( GetHitResultUnderCursor( ECC_Visibility, false, Hit ) )
		{
			CachedDestination = Hit.ImpactPoint;
		}

		if( APawn *ControlledPawn = GetPawn() )
		{
			const FVector WorldDestination = ( CachedDestination - ControlledPawn->GetActorLocation() ).GetSafeNormal();
			ControlledPawn->AddMovementInput( WorldDestination );
		}
	}
}

UAuraAbilitySystemComponent *AAuraPlayerController::GetASC()
{
	if( AuraAbilitySystemComponent == nullptr )
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>( UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent( GetPawn<APawn>() ) );
	}
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::AutoRun()
{ 
	if( !bAutoRunning ) return;

	if( APawn *ControlledPawn = GetPawn() )
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation( ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World );
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation( LocationOnSpline, ESplineCoordinateSpace::World );
		ControlledPawn->AddMovementInput( Direction );

		const float DistanceToDestination = ( LocationOnSpline - CachedDestination ).Length();
		if( DistanceToDestination <= AutoRunAcceptanceRadius )
		{
			bAutoRunning = false;
		}
	}
}
