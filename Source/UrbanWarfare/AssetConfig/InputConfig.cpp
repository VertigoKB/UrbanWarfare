#include "InputConfig.h"

void UInputConfig::PostLoad()
{
	Super::PostLoad();

	if (!DefaultMappingContext)
	{
		UE_LOG(LogTemp, Log, TEXT("[UInputConfig::PostLoad] DefaultMappingContext is null."));
	}

	if (!Movement)
	{
		UE_LOG(LogTemp, Log, TEXT("[UInputConfig::PostLoad] Movement is null."));
	}

	if (!Look)
	{
		UE_LOG(LogTemp, Log, TEXT("[UInputConfig::PostLoad] Look is null."));
	}
}
