#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CardData.h"
#include "TheGreatBookGameMode.generated.h"

class UDataTable;

/**
 * GameMode autoritativo. Espejo de las funciones de juego en GameManager.gd:
 *   - reparto inicial
 *   - resolución de fases de turno
 *   - jugar carta (TIERRA / SER / CONCEPTO)
 *   - tapear TIERRA → FD
 *   - decrementar dur de SERes
 *   - chequear condiciones de Días y victoria
 *   - aplicar efectos / penalties
 *
 * Solo corre en servidor. Mutaciones de estado vivan aquí y se replican
 * vía GameState (sin escribir directo desde clientes).
 */
UCLASS()
class THEGREATBOOK_API ATheGreatBookGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATheGreatBookGameMode();

	virtual void BeginPlay() override;

	/** DataTable con todas las cartas (FCardDataRow). Asignado en Blueprint */
	UPROPERTY(EditDefaultsOnly, Category = "Cards")
	TObjectPtr<UDataTable> CardsTable;

	/** Genera un inst_id único monotónico (espejo de _uid_counter de GameManager.gd) */
	FString GenerateInstanceId();

	/** Reparte cartas iniciales a ambos jugadores. */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void DealInitialHands(int32 PlayerHandSize = 5, int32 RivalHandSize = 5);

	/** Jugar carta desde mano. Server-only. */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void PlayCard(const FString& InstanceId, bool bIsPlayer);

	/** Tap TIERRA → genera FD */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void TapTierra(const FString& InstanceId, bool bIsPlayer);

	/** Avanza a la siguiente fase */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void AdvancePhase();

	/** Fin del turno → cambia jugador activo, decrementa duración SER, etc. */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void EndTurn();

	/** Devuelve fila de carta por id. Nullptr si no existe. */
	const FCardDataRow* GetCardRow(const FString& CardId) const;

private:
	int32 InstanceIdCounter = 0;
};
