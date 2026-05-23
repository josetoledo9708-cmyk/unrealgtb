#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PresetDeckData.generated.h"

/** Fila de DataTable equivalente a CardData.PRESET_DECKS en Godot.
 *  Un preset_deck contiene: nombre, id de Historia asociada y lista de cartas.
 */
USTRUCT(BlueprintType)
struct FPresetDeckRow : public FTableRowBase
{
	GENERATED_BODY()

	/** Nombre legible del mazo (ej. "La Caída del Edén") */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Deck")
	FText DisplayName;

	/** Id de la carta HISTORIA asociada a este mazo (h1..h7) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Deck")
	FString HistoriaCardId;

	/** Lista de ids de cartas (FName key del DT_Cards) que componen el mazo.
	 *  Tamaño típico: 30-35 cartas. Permite duplicados. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Deck")
	TArray<FString> CardIds;
};
