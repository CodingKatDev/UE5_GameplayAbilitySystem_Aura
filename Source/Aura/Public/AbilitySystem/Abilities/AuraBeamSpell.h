#pragma once

#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "CoreMinimal.h"
#include "AuraBeamSpell.generated.h"


UCLASS()
class AURA_API UAuraBeamSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintCallable )
	void StoreMouseDataInfo( const FHitResult &HitResult );

	UFUNCTION( BlueprintCallable )
	void StoreOwnerPlayerController();

protected:
	UPROPERTY( BlueprintReadWrite, Category = "Beam" )
	FVector MouseHitLocation;

	UPROPERTY( BlueprintReadWrite, Category = "Beam" )
	TObjectPtr<AActor> MouseHitActor;

	UPROPERTY( BlueprintReadWrite, Category = "Beam" )
	TObjectPtr<APlayerController> OwnerPlayerController;
};
