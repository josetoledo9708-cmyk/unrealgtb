#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;

/**
 * Widget base del menú principal. Espejo de StartMenu en Main.tscn (Godot).
 *
 * Botones (CONTRA IA, MULTIJUGADOR, CONSTRUCTOR DE MAZOS, SALIR) se conectan
 * en el WBP hijo desde el editor — esta clase solo provee los handlers.
 *
 * El BP_WBP_MainMenu debe tener UButtons nombrados:
 *   - VsAIButton
 *   - MultiplayerButton
 *   - DeckBuilderButton
 *   - QuitButton
 * que el meta BindWidget enlaza automáticamente.
 */
UCLASS(Abstract)
class THEGREATBOOK_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> VsAIButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> MultiplayerButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> DeckBuilderButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;

	UFUNCTION()
	void OnVsAIClicked();

	UFUNCTION()
	void OnMultiplayerClicked();

	UFUNCTION()
	void OnDeckBuilderClicked();

	UFUNCTION()
	void OnQuitClicked();

	/** Widget que se mostrará al elegir un modo de juego (deck selection).
	 *  Asignar en el Blueprint hijo (WBP_DeckSelection). */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UDeckSelectionWidget> DeckSelectionWidgetClass;
};
