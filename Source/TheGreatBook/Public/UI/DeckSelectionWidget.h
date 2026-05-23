#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PresetDeckData.h"
#include "DeckSelectionWidget.generated.h"

class UVerticalBox;
class UHorizontalBox;
class UButton;
class ATheGreatBookGameMode;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeckChosen, const FString&, DeckId);

/**
 * Widget de selección de mazo. Espejo de DeckSelection en Main.tscn (Godot)
 * — sección "ELIGE TU CAMINO" con botones Edén / Diluvio / Alianza / Custom.
 *
 * Al iniciar: pregunta al GameMode los preset decks (UDataTable) y genera
 * un botón por cada uno dentro del HorizontalBox DecksContainer.
 *
 * El BP_WBP_DeckSelection debe tener:
 *   - DecksContainer (UHorizontalBox o UVerticalBox)
 *   - DeckButtonTemplate: TSubclassOf<UUserWidget> opcional para custom layout
 *   - BackButton (UButton)
 */
UCLASS()
class THEGREATBOOK_API UDeckSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	/** Disparado cuando el usuario elige un mazo. */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeckChosen OnDeckChosen;

protected:
	// Widgets construidos en C++ (no BindWidget). WBP queda vacío.
	UPROPERTY()
	TObjectPtr<UHorizontalBox> DecksContainer;

	UPROPERTY()
	TObjectPtr<UButton> BackButton;

	/** Construye CanvasPanel root + Title + DecksContainer + BackButton */
	void BuildLayout();

	/** Llamar para regenerar la lista de mazos desde el GameMode. */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void RefreshDeckList();

	UFUNCTION()
	void OnBackClicked();

	/** Implementable en BP: cómo dibujar la card de un mazo (un botón con nombre, icono, etc.).
	 *  El C++ llama a este event por cada mazo. El BP retorna el UButton creado y conectado.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	UButton* CreateDeckButton(const FPresetDeckRow& Deck);

private:
	void HandleDeckButtonClicked(FString DeckId);
};
