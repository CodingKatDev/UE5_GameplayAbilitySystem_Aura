#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"


class UAuraAbilitySystemComponent;
class UAuraInputConfig;
class UDamageTextComponent;
class IEnemyInterface;
class UInputAction;
struct FInputActionValue;
class UInputMappingContext;
class UNiagaraSystem;
class USplineComponent;


UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAuraPlayerController();
	virtual void PlayerTick( float DeltaTime ) override;

	UFUNCTION( Client, Reliable )
	void ShowDamageNumber( float DamageAmount, ACharacter *TargetCharacter, bool bBlockedHit, bool bCriticalHit );

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY( EditAnywhere, Category = "Input" )
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY( EditAnywhere, Category = "Input" )
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY ( EditAnywhere, Category = "Input" )
	TObjectPtr<UInputAction> ShiftAction;
	
	void Move ( const FInputActionValue &InputActionValue );
	
	void ShiftPressed () { bShiftKeyDown = true; };
	void ShiftReleased () { bShiftKeyDown = false; };
	bool bShiftKeyDown = false;

	void CursorTrace();
	FHitResult CursorHit;;
	TObjectPtr<IEnemyInterface> LastActor;
	TObjectPtr<IEnemyInterface> ThisActor;

	void AbilityInputTagPressed( FGameplayTag InputTag );
	void AbilityInputTagReleased( FGameplayTag InputTag );
	void AbilityInputTagHeld( FGameplayTag InputTag );

	UPROPERTY( EditDefaultsOnly, Category = "Input" )
	TObjectPtr<UAuraInputConfig>InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent *GetASC();

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;

	UPROPERTY( EditDefaultsOnly, Category = "Aura Player Controller" )
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY( VisibleAnywhere )
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY( EditDefaultsOnly, Category = "Aura Player Controller" )
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	void AutoRun();

	UPROPERTY( EditDefaultsOnly, Category = "Aura Player Controller" )
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};
