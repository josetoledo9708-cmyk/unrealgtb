#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CardData.generated.h"

/** Tipo de carta (espejo de CardData.gd::Type) */
UENUM(BlueprintType)
enum class ECardType : uint8
{
	DIA       UMETA(DisplayName = "Día"),
	TIERRA    UMETA(DisplayName = "Tierra"),
	DIVINO    UMETA(DisplayName = "Divino"),
	HUMANO    UMETA(DisplayName = "Humano"),
	ANIMAL    UMETA(DisplayName = "Animal"),
	CONCEPTO  UMETA(DisplayName = "Concepto"),
	HISTORIA  UMETA(DisplayName = "Historia")
};

/** Fila de DataTable equivalente a una entrada de CardData.CARDS en Godot.
 *  Una DataTable de FCardDataRow contiene todas las cartas del juego.
 */
USTRUCT(BlueprintType)
struct FCardDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	FString CardId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	ECardType Type = ECardType::TIERRA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	FText SubName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	int32 Cost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card", meta = (Tooltip = "Solo para TIERRA: cuánto FD genera al tapear"))
	int32 FavorDivino = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card", meta = (Tooltip = "Duración del SER en turnos"))
	int32 Duration = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card", meta = (Tooltip = "Coste de habilidad activada (si aplica)"))
	int32 ActCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	FString Glyph;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	FText Effect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	FText Penalty;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	FText Condition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	FText Verse;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	TSoftObjectPtr<UTexture2D> Art;
};

/** Instancia de una carta en juego (con inst_id único y estado runtime).
 *  Equivale al dict {id, inst_id, tapped, dur_left} que GameState.gd guarda
 *  en arrays como player_hand, player_sers, player_tierras.
 */
USTRUCT(BlueprintType)
struct FCardInstance
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Card")
	FString CardId;

	UPROPERTY(BlueprintReadWrite, Category = "Card")
	FString InstanceId;

	UPROPERTY(BlueprintReadWrite, Category = "Card", meta = (Tooltip = "Solo TIERRAS y SERes: si está tapeada"))
	bool bTapped = false;

	UPROPERTY(BlueprintReadWrite, Category = "Card", meta = (Tooltip = "Turnos restantes de un SER en campo"))
	int32 DurationLeft = 0;
};
