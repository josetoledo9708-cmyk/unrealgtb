#include "CardActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"

ACardActor::ACardActor()
{
	PrimaryActorTick.bCanEverTick = false;

	CardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CardMesh"));
	RootComponent = CardMesh;

	CardWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("CardWidget"));
	CardWidget->SetupAttachment(RootComponent);
	CardWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void ACardActor::Setup(const FCardDataRow& InData, const FString& InInstanceId, bool bInShowBack)
{
	CardData = InData;
	InstanceId = InInstanceId;
	bShowBack = bInShowBack;
	// TODO: actualizar widget UMG con datos (nombre, tipo, coste, glyph, efecto, verse)
	// TODO: si bShowBack → cargar textura de reverso (assets/CArdback.png en Godot)
	// TODO: si no → cargar CardData.Art como textura del CardMesh
}

void ACardActor::SetPlayableGlow(bool bOn)
{
	// TODO: activar outline post-process verde (StyleBoxFlat con border en Godot)
}

void ACardActor::SetTutorialGlow(bool bOn)
{
	// TODO: activar outline post-process dorado
}
