#include "TheGreatBookPlayerController.h"
#include "TheGreatBookGameMode.h"
#include "UI/MainMenuWidget.h"
#include "Blueprint/UserWidget.h"
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

	// Mostrar menú principal solo en cliente local. Si la propiedad
	// MainMenuWidgetClass no se asignó en BP, usar la clase C++ base.
	if (IsLocalController())
	{
		TSubclassOf<UMainMenuWidget> ClassToUse = MainMenuWidgetClass
			? MainMenuWidgetClass
			: TSubclassOf<UMainMenuWidget>(UMainMenuWidget::StaticClass());
		UE_LOG(LogTemp, Log, TEXT("[PC] BeginPlay creating menu. ClassToUse=%s"),
			ClassToUse ? *ClassToUse->GetName() : TEXT("NULL"));
		MainMenuWidgetInstance = CreateWidget<UMainMenuWidget>(this, ClassToUse);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport(100);
			SetShowMouseCursor(true);
			FInputModeGameAndUI Mode;
			Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			Mode.SetHideCursorDuringCapture(false);
			SetInputMode(Mode);
			UE_LOG(LogTemp, Log, TEXT("[PC] Menu instance added to viewport."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[PC] CreateWidget returned NULL."));
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
