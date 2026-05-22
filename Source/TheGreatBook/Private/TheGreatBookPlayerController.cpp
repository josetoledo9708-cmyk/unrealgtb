#include "TheGreatBookPlayerController.h"
#include "TheGreatBookGameMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

ATheGreatBookPlayerController::ATheGreatBookPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void ATheGreatBookPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}

void ATheGreatBookPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// TODO: bind CancelAction → opens options panel / cancels selection
	}
}

void ATheGreatBookPlayerController::ServerPlayCard_Implementation(const FString& InstanceId)
{
	if (ATheGreatBookGameMode* GM = GetWorld()->GetAuthGameMode<ATheGreatBookGameMode>())
	{
		const bool bIsPlayer = true; // TODO: derivar de PlayerState/PlayerId
		GM->PlayCard(InstanceId, bIsPlayer);
	}
}

void ATheGreatBookPlayerController::ServerTapTierra_Implementation(const FString& InstanceId)
{
	if (ATheGreatBookGameMode* GM = GetWorld()->GetAuthGameMode<ATheGreatBookGameMode>())
	{
		const bool bIsPlayer = true;
		GM->TapTierra(InstanceId, bIsPlayer);
	}
}

void ATheGreatBookPlayerController::ServerAdvancePhase_Implementation()
{
	if (ATheGreatBookGameMode* GM = GetWorld()->GetAuthGameMode<ATheGreatBookGameMode>())
	{
		GM->AdvancePhase();
	}
}
