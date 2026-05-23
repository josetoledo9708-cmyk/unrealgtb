#include "UI/MainMenuWidget.h"
#include "UI/DeckSelectionWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

namespace
{
	UButton* MakeButton(UWidgetTree* Tree, UVerticalBox* Parent, const FString& Name, const FText& Label)
	{
		UButton* Btn = Tree->ConstructWidget<UButton>(UButton::StaticClass(), FName(*Name));
		UVerticalBoxSlot* Slot = Parent->AddChildToVerticalBox(Btn);
		if (Slot)
		{
			Slot->SetPadding(FMargin(0.0f, 6.0f, 0.0f, 6.0f));
			Slot->SetHorizontalAlignment(HAlign_Fill);
		}
		UTextBlock* Text = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), FName(*(Name + TEXT("_Text"))));
		Text->SetText(Label);
		Text->SetJustification(ETextJustify::Center);
		FSlateFontInfo Font = Text->GetFont();
		Font.Size = 16;
		Text->SetFont(Font);
		Btn->AddChild(Text);
		return Btn;
	}
}

void UMainMenuWidget::BuildLayout()
{
	if (!WidgetTree) return;
	if (WidgetTree->RootWidget) return; // ya construido (caso edit-time / hot reload)

	UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootCanvas"));
	WidgetTree->RootWidget = Root;

	UVerticalBox* VBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("MainVBox"));
	UCanvasPanelSlot* VBoxSlot = Root->AddChildToCanvas(VBox);
	if (VBoxSlot)
	{
		VBoxSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
		VBoxSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		VBoxSlot->SetPosition(FVector2D(0.0f, 0.0f));
		VBoxSlot->SetSize(FVector2D(320.0f, 360.0f));
	}

	VsAIButton        = MakeButton(WidgetTree, VBox, TEXT("VsAIButton"),        FText::FromString(TEXT("CONTRA IA")));
	MultiplayerButton = MakeButton(WidgetTree, VBox, TEXT("MultiplayerButton"), FText::FromString(TEXT("MULTIJUGADOR (Beta)")));
	DeckBuilderButton = MakeButton(WidgetTree, VBox, TEXT("DeckBuilderButton"), FText::FromString(TEXT("CONSTRUCTOR DE MAZOS")));
	QuitButton        = MakeButton(WidgetTree, VBox, TEXT("QuitButton"),        FText::FromString(TEXT("SALIR")));
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BuildLayout();

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
