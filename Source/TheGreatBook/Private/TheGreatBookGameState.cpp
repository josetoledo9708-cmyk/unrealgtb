#include "TheGreatBookGameState.h"
#include "Net/UnrealNetwork.h"

void ATheGreatBookGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATheGreatBookGameState, PlayerHand);
	DOREPLIFETIME(ATheGreatBookGameState, RivalHand);
	DOREPLIFETIME(ATheGreatBookGameState, PlayerTierras);
	DOREPLIFETIME(ATheGreatBookGameState, RivalTierras);
	DOREPLIFETIME(ATheGreatBookGameState, PlayerSers);
	DOREPLIFETIME(ATheGreatBookGameState, RivalSers);
	DOREPLIFETIME(ATheGreatBookGameState, PlayerConceptoTemp);
	DOREPLIFETIME(ATheGreatBookGameState, RivalConceptoTemp);
	DOREPLIFETIME(ATheGreatBookGameState, PlayerRetired);
	DOREPLIFETIME(ATheGreatBookGameState, RivalRetired);
	DOREPLIFETIME(ATheGreatBookGameState, PlayerFavorDivino);
	DOREPLIFETIME(ATheGreatBookGameState, RivalFavorDivino);
	DOREPLIFETIME(ATheGreatBookGameState, TurnNumber);
	DOREPLIFETIME(ATheGreatBookGameState, bPlayerActive);
	DOREPLIFETIME(ATheGreatBookGameState, Phase);
	DOREPLIFETIME(ATheGreatBookGameState, SelectionMode);
	DOREPLIFETIME(ATheGreatBookGameState, bIsTutorial);
}
