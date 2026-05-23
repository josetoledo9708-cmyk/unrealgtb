#include "TheGreatBookGameMode.h"
#include "TheGreatBookGameState.h"
#include "TheGreatBookPlayerController.h"
#include "PresetDeckData.h"
#include "Engine/DataTable.h"

ATheGreatBookGameMode::ATheGreatBookGameMode()
{
	GameStateClass = ATheGreatBookGameState::StaticClass();
	PlayerControllerClass = ATheGreatBookPlayerController::StaticClass();
	bUseSeamlessTravel = true;
}

void ATheGreatBookGameMode::BeginPlay()
{
	Super::BeginPlay();
	// TODO: cargar mazos, repartir manos iniciales según modo (PvE/PvP/Tutorial)
}

FString ATheGreatBookGameMode::GenerateInstanceId()
{
	return FString::Printf(TEXT("inst_%d"), ++InstanceIdCounter);
}

void ATheGreatBookGameMode::DealInitialHands(int32 PlayerHandSize, int32 RivalHandSize)
{
	// TODO: implementar reparto inicial desde mazos seleccionados
	// Espejo de GameManager.gd::_deal_initial_hands
}

void ATheGreatBookGameMode::PlayCard(const FString& InstanceId, bool bIsPlayer)
{
	// TODO: implementar resolución de jugar carta
	// 1) Buscar carta en mano correspondiente
	// 2) Validar coste FD vs disponible
	// 3) Según tipo:
	//    - TIERRA: mover a PlayerTierras / RivalTierras
	//    - SER (HUMANO/DIVINO/ANIMAL): mover a PlayerSers / RivalSers con dur_left
	//    - CONCEPTO: aplicar efecto, mover a ConceptoTemp 4s, luego Retired
	// 4) Disparar OnHandChanged / OnFDChanged
}

void ATheGreatBookGameMode::TapTierra(const FString& InstanceId, bool bIsPlayer)
{
	// TODO: marcar tierra como tapped y sumar FD (fd del CardDataRow)
}

void ATheGreatBookGameMode::AdvancePhase()
{
	// TODO: transición de fase (PREPARACION → ENTREGA → CONCEPCION → REVELACION → PREPARACION del rival)
}

void ATheGreatBookGameMode::EndTurn()
{
	// TODO:
	// 1) Decrementar DurationLeft de cada SER en campo
	// 2) Mover SERes con dur_left == 0 a Retired
	// 3) Cambiar bPlayerActive
	// 4) Resetear FD del jugador entrante (a definir)
	// 5) Incrementar TurnNumber si vuelve al jugador inicial
}

const FCardDataRow* ATheGreatBookGameMode::GetCardRow(const FString& CardId) const
{
	if (!CardsTable) return nullptr;
	return CardsTable->FindRow<FCardDataRow>(FName(*CardId), TEXT("GetCardRow"));
}

bool ATheGreatBookGameMode::GetPresetDeckById(const FString& DeckId, FPresetDeckRow& OutDeck) const
{
	if (!PresetDecksTable) return false;
	const FPresetDeckRow* Row = PresetDecksTable->FindRow<FPresetDeckRow>(FName(*DeckId), TEXT("GetPresetDeckById"));
	if (!Row) return false;
	OutDeck = *Row;
	return true;
}

TArray<FPresetDeckRow> ATheGreatBookGameMode::GetAllPresetDecks() const
{
	TArray<FPresetDeckRow> Result;
	if (!PresetDecksTable) return Result;

	TArray<FName> RowNames = PresetDecksTable->GetRowNames();
	Result.Reserve(RowNames.Num());
	for (const FName& RowName : RowNames)
	{
		if (const FPresetDeckRow* Row = PresetDecksTable->FindRow<FPresetDeckRow>(RowName, TEXT("GetAllPresetDecks")))
		{
			Result.Add(*Row);
		}
	}
	return Result;
}
