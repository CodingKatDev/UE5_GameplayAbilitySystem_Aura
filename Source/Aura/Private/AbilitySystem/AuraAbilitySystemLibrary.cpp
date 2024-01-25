#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AuraAbilityTypes.h"
#include "Game/AuraGameModeBase.h"
#include "GameplayEffectTypes.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"


UOverlayWidgetController *UAuraAbilitySystemLibrary::GetOverlayWidgetController( const UObject *WorldContextObject )
{
	if( APlayerController *PC = UGameplayStatics::GetPlayerController( WorldContextObject, 0 ) ) // for multiplayer instead of passing in 0, UWorld::GetFirstPlayerController() might be better
	{
		if( AAuraHUD *AuraHUD = Cast<AAuraHUD>( PC->GetHUD() ) )
		{
			AAuraPlayerState *PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent *ASC = PS->GetAbilitySystemComponent();
			UAttributeSet *AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams( PC, PS, ASC, AS );
			
			return AuraHUD->GetOverlayWidgetController( WidgetControllerParams );
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController *UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController( const UObject *WorldContextObject )
{
	if( APlayerController *PC = UGameplayStatics::GetPlayerController( WorldContextObject, 0 ) ) // for multiplayer instead of passing in 0, UWorld::GetFirstPlayerController() might be better
	{
		if( AAuraHUD *AuraHUD = Cast<AAuraHUD>( PC->GetHUD() ) )
		{
			AAuraPlayerState *PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent *ASC = PS->GetAbilitySystemComponent();
			UAttributeSet *AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams( PC, PS, ASC, AS );

			return AuraHUD->GetAttributeMenuWidgetController( WidgetControllerParams );
		}
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes( const UObject *WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent *ASC )
{
	const AActor *AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo *CharacterClassInfo = GetCharacterClassInfo( WorldContextObject );
	const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo( CharacterClass );

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject( AvatarActor );
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec( ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle );
	ASC->ApplyGameplayEffectSpecToSelf( *PrimaryAttributesSpecHandle.Data.Get() );

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject( AvatarActor );
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec( CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle );
	ASC->ApplyGameplayEffectSpecToSelf( *SecondaryAttributesSpecHandle.Data.Get() );

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject( AvatarActor );
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec( CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle );
	ASC->ApplyGameplayEffectSpecToSelf( *VitalAttributesSpecHandle.Data.Get() );
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities( const UObject *WorldContextObject, ECharacterClass CharacterClass, UAbilitySystemComponent *ASC )
{
	UCharacterClassInfo *CharacterClassInfo = GetCharacterClassInfo( WorldContextObject );
	if( CharacterClassInfo == nullptr ) return;

	for( TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities )
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec( AbilityClass, 1 );
		ASC->GiveAbility( AbilitySpec );
	}

	const FCharacterClassDefaultInfo &DefaultInfo = CharacterClassInfo->GetClassDefaultInfo( CharacterClass );
	for( TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities )
	{
		if( ICombatInterface *CombatInterface = Cast<ICombatInterface>( ASC->GetAvatarActor() ) )
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec( AbilityClass, CombatInterface->GetPlayerLevel() );
			ASC->GiveAbility( AbilitySpec );
		}
	}
}

UCharacterClassInfo *UAuraAbilitySystemLibrary::GetCharacterClassInfo( const UObject *WorldContextObject )
{
	AAuraGameModeBase *AuraGameMode = Cast<AAuraGameModeBase>( UGameplayStatics::GetGameMode( WorldContextObject ) );
	if( AuraGameMode == nullptr ) return nullptr;

	return AuraGameMode->CharacterClassInfo;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit( const FGameplayEffectContextHandle &EffectContextHandle )
{
	if( const FAuraGameplayEffectContext *AuraEffectContext = static_cast< const FAuraGameplayEffectContext * >( EffectContextHandle.Get() ) )
	{
		return AuraEffectContext->IsBlockedHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit( const FGameplayEffectContextHandle &EffectContextHandle )
{
	if( const FAuraGameplayEffectContext *AuraEffectContext = static_cast< const FAuraGameplayEffectContext * >( EffectContextHandle.Get() ) )
	{
		return AuraEffectContext->IsCriticalHit();
	}
	return false;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit( UPARAM( ref )FGameplayEffectContextHandle &EffectContextHandle, bool bInIsBlockedHit )
{
	if( FAuraGameplayEffectContext *AuraEffectContext = static_cast< FAuraGameplayEffectContext * >( EffectContextHandle.Get() ) )
	{
		AuraEffectContext->SetIsBlockedHit( bInIsBlockedHit );
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit( UPARAM( ref )FGameplayEffectContextHandle &EffectContextHandle, bool bInIsCriticalHit )
{
	if( FAuraGameplayEffectContext *AuraEffectContext = static_cast< FAuraGameplayEffectContext * >( EffectContextHandle.Get() ) )
	{
		AuraEffectContext->SetIsCriticalHit( bInIsCriticalHit );
	}
}