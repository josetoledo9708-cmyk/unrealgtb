#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardData.h"
#include "CardActor.generated.h"

class UWidgetComponent;
class UStaticMeshComponent;

/**
 * Actor que representa una carta en el mundo 3D. Equivale a la escena
 * Card.tscn de Godot. Aplica datos visuales (nombre, arte, glow, etc.)
 * y maneja hover/click si está en mano.
 *
 * El layout en mano (abanico) lo maneja un HandLayoutComponent
 * o un Widget UMG, no esta Actor directamente.
 */
UCLASS()
class THEGREATBOOK_API ACardActor : public AActor
{
	GENERATED_BODY()

public:
	ACardActor();

	/** Datos estáticos de la carta (de la DataTable) */
	UPROPERTY(BlueprintReadWrite, Category = "Card")
	FCardDataRow CardData;

	/** Instancia única de esta carta (inst_id) */
	UPROPERTY(BlueprintReadWrite, Category = "Card")
	FString InstanceId;

	/** Si está face-down (rival) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	bool bShowBack = false;

	/** Mesh quad de la carta */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> CardMesh;

	/** Widget UMG para tipo/nombre/coste/glyph/efecto/verse */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWidgetComponent> CardWidget;

	/** Setup inicial con datos */
	UFUNCTION(BlueprintCallable, Category = "Card")
	void Setup(const FCardDataRow& InData, const FString& InInstanceId, bool bInShowBack);

	/** Activa glow verde (carta jugable) */
	UFUNCTION(BlueprintCallable, Category = "Card")
	void SetPlayableGlow(bool bOn);

	/** Activa glow dorado (tutorial) */
	UFUNCTION(BlueprintCallable, Category = "Card")
	void SetTutorialGlow(bool bOn);
};
