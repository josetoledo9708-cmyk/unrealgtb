#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TheGreatBookPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 * PlayerController. Espejo de las interacciones del jugador en GameManager.gd:
 *   - Click sobre carta de mano → ServerPlayCard
 *   - Click sobre tierra propia (no tapeada) → ServerTapTierra
 *   - Drag&drop carta → ServerPlayCard con zona objetivo
 *   - ESC → cancelar selección actual o abrir opciones
 *
 * Cliente envía intents al server (RPCs). Sin lógica autoritativa aquí.
 */
UCLASS()
class THEGREATBOOK_API ATheGreatBookPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATheGreatBookPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> CancelAction;

	/** Pide al server jugar una carta */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Game")
	void ServerPlayCard(const FString& InstanceId);

	/** Pide al server tapear una tierra propia */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Game")
	void ServerTapTierra(const FString& InstanceId);

	/** Pide al server avanzar fase */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Game")
	void ServerAdvancePhase();
};
