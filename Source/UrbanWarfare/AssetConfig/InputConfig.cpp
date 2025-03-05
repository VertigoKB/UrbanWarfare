#include "InputConfig.h"

void UInputConfig::PostLoad()
{
	Super::PostLoad();

	if (!InputMappingContext)
	{
		UE_LOG(LogTemp, Log, TEXT("[UInputConfig::PostLoad] InputMappingContext is null."));
	}

	if (!MyInputAction)
	{
		UE_LOG(LogTemp, Log, TEXT("[UInputConfig::PostLoad] MyInputAction is null."));
	}
}
