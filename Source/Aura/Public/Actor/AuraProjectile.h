#pragma once

#include "AuraAbilityTypes.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"


class UNiagaraSystem;
class UProjectileMovementComponent;
class USphereComponent;


UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraProjectile();

	UPROPERTY( VisibleAnywhere )
	TObjectPtr<UProjectileMovementComponent>ProjectileMovement;

	UPROPERTY( BlueprintReadWrite, meta = ( ExposeOnSpawn = true ))
	FDamageEffectParams DamageEffectParams;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed () override;

	UFUNCTION()
	void OnSphereOverlap( UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult );

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
	TObjectPtr<USphereComponent> Sphere;

private:
	void OnHit();

	bool bHit = false;

	UPROPERTY( EditDefaultsOnly )
	float LifeSpan = 15.f;

	UPROPERTY( EditAnywhere )
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY ( EditAnywhere )
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY ( EditAnywhere )
	TObjectPtr<USoundBase> LoopingSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent>LoopingSoundComponent;
};
