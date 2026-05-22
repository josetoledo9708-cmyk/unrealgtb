# Godot 4 → Unreal 5.4 mapping — The Great Book

Guía de traducción del proyecto Godot original al port Unreal. Se actualiza a medida que cada sistema se porta.

## Estado general

| Subsistema | Godot (origen) | Unreal (destino) | Status |
|---|---|---|---|
| Datos de carta | `scripts/CardData.gd` const dict | `Source/.../CardData.h` USTRUCT + DataTable | Skeleton |
| Estado de partida | `scripts/GameState.gd` autoload | `ATheGreatBookGameState` (replicated) | Skeleton |
| Lógica autoritativa | `scripts/GameManager.gd` | `ATheGreatBookGameMode` (server-only) | Skeleton |
| Input jugador | `Card.gd::_gui_input` + GameManager handlers | `ATheGreatBookPlayerController` + Enhanced Input | Skeleton |
| Visual de carta | `scenes/Card.tscn` + `Card.gd` | `ACardActor` + UMG Widget | Skeleton |
| Layout de mano (abanico) | `GameManager.gd::_layout_hand` | (TODO) `UHandLayoutComponent` ó UMG WidgetSwitcher | Pendiente |
| Multiplayer | `scripts/NetworkManager.gd` ENet | OnlineSubsystem + Replication nativa UE | Pendiente |
| IA del rival | `scripts/RivalAI.gd` | (TODO) `ATheGreatBookRivalAI` | Pendiente |
| Menú principal | `Main.tscn::StartMenu` CanvasLayer | UMG `WBP_MainMenu` | Pendiente |
| Mazos pre-construidos | `GameState.gd` deck arrays | DataTable + UDataAsset por mazo | Pendiente |
| Tutorial | GameManager tutorial state machine | (TODO) `ATheGreatBookTutorialDirector` | Pendiente |
| Partículas (clic/cursor) | CPUParticles2D + Click.png | Niagara System | Pendiente |
| Cursor custom | Image.load_from_file + Input.set_custom_mouse_cursor | `USoftwareCursor` ó UMG cursor widget | Pendiente |

## Equivalencias técnicas

### Tipos básicos

| Godot | Unreal C++ |
|---|---|
| `Dictionary` | `TMap<FString, FCardInstance>` ó `USTRUCT` con campos |
| `Array` | `TArray<T>` |
| `String` | `FString` |
| `int` (ids) | `int32` |
| `float` | `float` (UE) |
| `Vector2` | `FVector2D` |
| `Color` | `FLinearColor` |
| `Node` con `@onready` | `UPROPERTY()` + asignación en `BeginPlay` o BP |
| `signal X` + `emit_signal` | `DECLARE_DYNAMIC_MULTICAST_DELEGATE` + `Broadcast` |
| `await get_tree().create_timer(4.0).timeout` | `FTimerHandle` + `GetWorld()->GetTimerManager().SetTimer` |
| `preload("res://x")` | `TSoftObjectPtr<T>` + `LoadSynchronous()` |
| `class_name X extends Node` | `UCLASS() class X : public AActor/UObject` |
| `const CARDS = {...}` | `UDataTable*` de `FCardDataRow` |
| `enum Type {...}` | `UENUM(BlueprintType)` |
| CanvasLayer | UMG `UUserWidget` o UI HUD layer |
| `queue_free()` | `Destroy()` (Actor) ó `MarkAsGarbage()` (UObject) |
| `_render()` (full redraw) | Replicated property OnRep + UMG widget refresh |
| `GameState autoload` | Subsistema Game Instance (`UGameInstanceSubsystem`) o GameState |

### Estructura de la mano

Godot:
- `GameState.player_hand` = `Array[Dictionary]` con `{id, inst_id}`
- `GameManager._layout_hand` posiciona cartas en `PlayerHand` Control con fórmula parabólica
- Cada Card es escena AspectRatioContainer

Unreal:
- `GameState.PlayerHand` = `TArray<FCardInstance>` (replicado)
- `WBP_HandWidget` (UMG): Overlay con cartas hijas posicionadas en arco
  - Si se prefiere 3D: `ACardActor` por carta + `UHandLayoutComponent` que computa posiciones
- Por cada cambio en `PlayerHand` (OnRep), regenerar los visuales

### Sistema de turnos

Godot enum:
```gdscript
enum Phase { PREPARACION, ENTREGA, CONCEPCION, REVELACION }
```

Unreal:
```cpp
UENUM(BlueprintType)
enum class EGamePhase : uint8 { PREPARACION, ENTREGA, CONCEPCION, REVELACION };
```

Transiciones manejadas server-side por `ATheGreatBookGameMode::AdvancePhase()`.

### Multiplayer

Godot usa ENet directo via `NetworkManager.gd` con `multiplayer_peer`. Cada acción es un mensaje serializado.

Unreal: usar replicación nativa.
- Servidor autoritativo: `ATheGreatBookGameMode`
- Cliente envía intents via `Server` RPCs (en `ATheGreatBookPlayerController`)
- Servidor muta el `GameState`
- Replication automática propaga cambios a clientes
- Eventos puntuales (efectos visuales, mensajes de log) via `NetMulticast` RPCs

## Mazos pre-construidos

Godot tiene mazos en `GameState.gd` (arrays de card ids):
- La Caída del Edén (equilibrado)
- El Diluvio Universal (control)
- La Alianza (velocidad)
- Mazo personalizado (deck builder)

Unreal:
- `UDataAsset` por mazo: `UPreBuiltDeckAsset` con `TArray<FString> CardIds`
- Pre-built decks en `Content/Decks/`
- Deck builder UMG con drag&drop de cards

## Assets a portar

| Tipo | Godot | Unreal target |
|---|---|---|
| Fondo main menu | `assets/main_menu_bg.jpg` | `/Game/UI/Backgrounds/T_MainMenuBG` |
| Logo | `assets/TGB.png` | `/Game/UI/T_Logo` |
| Card back | `assets/CArdback.png` | `/Game/Cards/T_CardBack` |
| Cursor | `assets/cursor_custom.png` | `/Game/UI/Cursors/T_CursorCustom` |
| Partícula clic | `assets/click_particle.png` | `/Game/FX/T_ClickParticle` + Niagara |
| Arte por carta | `assets/<categoría>/<id>.png` | `/Game/Cards/Art/T_<id>` |
| Música | `assets/*.mp3` | `/Game/Audio/Music/` (importar como WAV o usar plugin OGG) |

## Conversión de efectos de carta

Godot tiene efectos hard-coded en `GameManager.gd::_apply_concepto_effect`, `_apply_entry_effect`, etc.

Unreal alternativas:
1. **Lo mismo en C++**: switch sobre CardId, métodos en GameMode. Rápido pero no extensible.
2. **GameplayAbilitySystem (GAS)**: cada carta es un `UGameplayAbility`. Excesivo para un TCG pequeño.
3. **Recomendado**: Strategy pattern con `UCardEffect` (UObject) por efecto, asignable en DataTable. Cada FCardDataRow tiene `TSoftClassPtr<UCardEffect> EffectClass`. Cuando se juega la carta, GameMode instancia y ejecuta `Effect->Apply(Context)`.

## Plan de implementación por sesiones

| Sesión | Sistema | Entregable |
|---|---|---|
| 1 (actual) | Skeleton + doc | Project compilable vacío |
| 2 | DataTable de cartas (port del dict) | Todas las cartas en `DT_Cards.uasset` |
| 3 | Pre-built decks como UDataAsset | 3 mazos seleccionables |
| 4 | UMG main menu + deck selection | Botones funcionales |
| 5 | ACardActor + UMG widget | Cartas visualizables con datos |
| 6 | HandLayoutComponent (abanico) | Mano del jugador con layout |
| 7 | Drag&drop de cartas | Jugar carta visualmente |
| 8 | Sistema de turnos (fases) | Loop básico de turno |
| 9 | TIERRA / FD economy | Tapeo de tierras genera FD |
| 10 | Jugar SERes con coste | Pagar FD para invocar SERes |
| 11 | Decremento dur SER + cementerio | Lifecycle completo de SER |
| 12 | Sistema de efectos (Strategy pattern) | Efectos de CONCEPTO funcionando |
| 13-15 | Días (condiciones + victoria día 7) | Win condition completa |
| 16-18 | Rival AI básica | Single-player jugable |
| 19-22 | Multiplayer (OnlineSubsystem) | PvP funcional |
| 23-25 | Tutorial | Tutorial guiado |
| 26+ | Pulido visual (partículas, audio, animaciones, post-process) | Versión presentable |

## Diferencias culturales clave Godot ↔ Unreal

- **Scenes vs Blueprints**: Godot escena = árbol de nodos serializado. Unreal Blueprint = clase serializada con sub-componentes. Ambos heredan de una clase base de código.
- **Signals vs Delegates**: equivalentes 1:1 funcionalmente. Unreal Multicast Dynamic Delegate = Godot signal exposed to BP.
- **Autoloads vs Subsystems**: Godot Autoload (singleton accesible global) ≈ Unreal Subsystem (`UGameInstanceSubsystem`, `UWorldSubsystem`, etc.).
- **Container layout**: Godot `HBoxContainer`/`VBoxContainer` etc. ≈ UMG `HorizontalBox`/`VerticalBox`. Más parecidos que cualquier otra cosa.
- **Replicación**: Godot tiene `@rpc` annotations. Unreal usa `UFUNCTION(Server/Client/NetMulticast)` + `UPROPERTY(Replicated)`. Mucho más robusto.
- **Hot reload**: Godot reinicia escena. Unreal Live Coding recompila C++ in-place.
