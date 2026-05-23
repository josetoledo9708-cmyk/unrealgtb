#include "UI/MainMenuWidget.h"
#include "UI/DeckSelectionWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (VsAIButton)        VsAIButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnVsAIClicked);
	if (MultiplayerButton) MultiplayerButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnMultiplayerClicked);
	if (DeckBuilderButton) DeckBuilderButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnDeckBuilderClicked);
	if (QuitButton)        QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);
}

void UMainMenuWidget::OnVsAIClicked()
{
	if (!DeckSelectionWidgetClass) return;
	UDeckSelectionWidget* DeckUI = CreateWidget<UDeckSelectionWidget>(GetWorld(), DeckSelectionWidgetClass);
	if (!DeckUI) return;
	DeckUI->AddToViewport();
	this->RemoveFromParent();
	// TODO: cuando OnDeckChosen dispara, iniciar partida vs IA con ese mazo
}

void UMainMenuWidget::OnMultiplayerClicked()
{
	// TODO: abrir UI de multiplayer (host/join)
}

void UMainMenuWidget::OnDeckBuilderClicked()
{
	// TODO: abrir UI del deck builder custom
}

void UMainMenuWidget::OnQuitClicked()
{
	APlayerController* PC = GetOwningPlayer();
	UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
}
