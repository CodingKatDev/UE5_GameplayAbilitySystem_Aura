#include "AuraAssetManager.h"

#include <AbilitySystemGlobals.h>
#include "AuraGameplayTags.h"


UAuraAssetManager &UAuraAssetManager::Get()
{
	check( GEngine );

	UAuraAssetManager *AuraAssetManager = Cast<UAuraAssetManager>( GEngine->AssetManager );
	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{ 
	Super::StartInitialLoading();
	FAuraGameplayTags::InitializeNativeGameplayTags();

	// This is required to use Target Data! (called automatically in UE5.3+)
	UAbilitySystemGlobals::Get ().InitGlobalData ();
}
