#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CardData.h"
#include "TheGreatBookGameState.generated.h"

/** Fases del turno (espejo de GameState.gd::Phase) */
UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	PREPARACION  UMETA(DisplayName = "Preparación"),
	ENTREGA      UMETA(DisplayName = "Entrega"),
	CONCEPCION   UMETA(DisplayName = "Concepción"),
	REVELACION   UMETA(DisplayName = "Revelación")
};

/** Modo de selección activo */
UENUM(BlueprintType)
enum class EGBSelectionMode : uint8
{
	NONE          UMETA(DisplayName = "Ninguno"),
	DISCARD       UMETA(DisplayName = "Descarte"),
	TARGETING     UMETA(DisplayName = "Targeting")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFavorDivinoChanged, int32, NewFD);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHandChanged, const TArray<FCardInstance>&, NewHand);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPhaseChanged);

/**
 * GameState replicado. Espejo de GameState.gd:
 *   - player_hand / rival_hand
 *   - player_tierras / rival_tierras (con tapped)
 *   - player_sers / rival_sers (con dur_left, tapped)
 *   - player_concepto_temp / rival_concepto_temp (display 4s)
 *   - player_retired / rival_retired (cementerio)
 *   - player_fd / rival_fd
 *   - turn, active_player, phase
 *   - discard_mode, targeting_mode
 */
UCLASS()
class THEGREATBOOK_API ATheGreatBookGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	// ── Manos ──────────────────────────────────────────────────────
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Hand")
	TArray<FCardInstance> PlayerHand;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Hand")
	TArray<FCardInstance> RivalHand;

	// ── Campo de batalla ───────────────────────────────────────────
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Field")
	TArray<FCardInstance> PlayerTierras;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Field")
	TArray<FCardInstance> RivalTierras;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Field")
	TArray<FCardInstance> PlayerSers;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Field")
	TArray<FCardInstance> RivalSers;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Field")
	TArray<FCardInstance> PlayerConceptoTemp;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Field")
	TArray<FCardInstance> RivalConceptoTemp;

	// ── Cementerio ─────────────────────────────────────────────────
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Retired")
	TArray<FString> PlayerRetired;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Retired")
	TArray<FString> RivalRetired;

	// ── Recursos y turno ───────────────────────────────────────────
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Resources")
	int32 PlayerFavorDivino = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Resources")
	int32 RivalFavorDivino = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Turn")
	int32 TurnNumber = 1;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Turn")
	bool bPlayerActive = true;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Turn")
	EGamePhase Phase = EGamePhase::PREPARACION;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Turn")
	EGBSelectionMode SelectionMode = EGBSelectionMode::NONE;

	// ── Tutorial ───────────────────────────────────────────────────
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Tutorial")
	bool bIsTutorial = false;

	// ── Eventos UI ─────────────────────────────────────────────────
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnFavorDivinoChanged OnPlayerFDChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHandChanged OnPlayerHandChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPhaseChanged OnPhaseChanged;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
