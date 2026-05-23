#include "UI/DeckSelectionWidget.h"
#include "TheGreatBookGameMode.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Blueprint/WidgetTree.h"

void UDeckSelectionWidget::BuildLayout()
{
	if (!WidgetTree) return;
	if (WidgetTree->RootWidget) return;

	UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootCanvas"));
	WidgetTree->RootWidget = Root;

	// Title
	UTextBlock* Title = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("TitleText"));
	Title->SetText(FText::FromString(TEXT("ELIGE TU CAMINO")));
	Title->SetJustification(ETextJustify::Center);
	{
		FSlateFontInfo Font = Title->GetFont();
		Font.Size = 32;
		Title->SetFont(Font);
	}
	UCanvasPanelSlot* TitleSlot = Root->AddChildToCanvas(Title);
	if (TitleSlot)
	{
		TitleSlot->SetAnchors(FAnchors(0.5f, 0.0f, 0.5f, 0.0f));
		TitleSlot->SetAlignment(FVector2D(0.5f, 0.0f));
		TitleSlot->SetPosition(FVector2D(0.0f, 40.0f));
		TitleSlot->SetSize(FVector2D(600.0f, 60.0f));
	}

	// DecksContainer
	DecksContainer = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("DecksContainer"));
	UCanvasPanelSlot* ContSlot = Root->AddChildToCanvas(DecksContainer);
	if (ContSlot)
	{
		ContSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
		ContSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		ContSlot->SetPosition(FVector2D(0.0f, 0.0f));
		ContSlot->SetSize(FVector2D(800.0f, 300.0f));
	}

	// BackButton
	BackButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("BackButton"));
	UCanvasPanelSlot* BackSlot = Root->AddChildToCanvas(BackButton);
	if (BackSlot)
	{
		BackSlot->SetAnchors(FAnchors(0.5f, 1.0f, 0.5f, 1.0f));
		BackSlot->SetAlignment(FVector2D(0.5f, 1.0f));
		BackSlot->SetPosition(FVector2D(0.0f, -60.0f));
		BackSlot->SetSize(FVector2D(200.0f, 50.0f));
	}
	UTextBlock* BackText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("BackButton_Text"));
	BackText->SetText(FText::FromString(TEXT("VOLVER")));
	BackText->SetJustification(ETextJustify::Center);
	{
		FSlateFontInfo Font = BackText->GetFont();
		Font.Size = 16;
		BackText->SetFont(Font);
	}
	BackButton->AddChild(BackText);
}

void UDeckSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BuildLayout();

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
		UButton* DeckButton = CreateDeckButton(Deck);
		if (!DeckButton)
		{
			// Fallback: si el BP no implementa CreateDeckButton, construir un
			// botón mínimo en C++ con el nombre del mazo.
			UButton* Btn = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());
			UHorizontalBoxSlot* HBoxItemSlot = DecksContainer->AddChildToHorizontalBox(Btn);
			if (HBoxItemSlot)
			{
				HBoxItemSlot->SetPadding(FMargin(8.0f));
			}
			UTextBlock* Label = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			Label->SetText(Deck.DisplayName);
			Label->SetJustification(ETextJustify::Center);
			FSlateFontInfo Font = Label->GetFont();
			Font.Size = 18;
			Label->SetFont(Font);
			Btn->AddChild(Label);
		}
	}
}

void UDeckSelectionWidget::OnBackClicked()
{
	this->RemoveFromParent();
}

void UDeckSelectionWidget::HandleDeckButtonClicked(FString DeckId)
{
	OnDeckChosen.Broadcast(DeckId);
}
