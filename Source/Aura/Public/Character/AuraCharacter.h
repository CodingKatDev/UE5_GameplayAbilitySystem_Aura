#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;


UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
	
public:
	AAuraCharacter();

	virtual void PossessedBy( AController *NewController ) override;
	virtual void OnRep_PlayerState() override;

private:
	UPROPERTY( VisibleAnywhere, Category = "Camera" )
	TObjectPtr<USpringArmComponent> CameraArm;

	UPROPERTY( VisibleAnywhere, Category = "Camera" )
	TObjectPtr<UCameraComponent> FollowCamera;

	void InitAbilityActorInfo();
};
