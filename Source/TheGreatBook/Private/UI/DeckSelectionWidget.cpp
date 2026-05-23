#include "UI/DeckSelectionWidget.h"
#include "TheGreatBookGameMode.h"
#include "Components/HorizontalBox.h"
#include "Components/Button.h"

void UDeckSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UDeckSelectionWidget::OnBackClicked);
	}

	RefreshDeckList();
}

void UDeckSelectionWidget::RefreshDeckList()
{
	if (!DecksContainer) return;
	DecksContainer->ClearChildren();

	ATheGreatBookGameMode* GM = GetWorld() ? GetWorld()->GetAuthGameMode<ATheGreatBookGameMode>() : nullptr;
	if (!GM) return;

	const TArray<FPresetDeckRow> Decks = GM->GetAllPresetDecks();
	for (const FPresetDeckRow& Deck : Decks)
	{
		// El BP es quien sabe cómo dibujar el botón (textura del mazo, layout, etc.)
		UButton* DeckButton = CreateDeckButton(Deck);
		if (!DeckButton) continue;

		// Capturar el HistoriaCardId por valor para no romper el lambda
		const FString DeckIdCopy = Deck.HistoriaCardId;
		DeckButton->OnClicked.AddDynamic(this, &UDeckSelectionWidget::OnBackClicked); // placeholder
		// Nota: para pasar argumento (DeckId) al click, normalmente se usa una lambda
		// o un wrapper. UButton::OnClicked no acepta payload, así que el BP debería
		// llamar a un BlueprintCallable que reciba DeckId. Se deja al BP child.
	}
}

void UDeckSelectionWidget::OnBackClicked()
{
	this->RemoveFromParent();
	// TODO: reabrir MainMenu si se necesita
}

void UDeckSelectionWidget::HandleDeckButtonClicked(FString DeckId)
{
	OnDeckChosen.Broadcast(DeckId);
}
