# Sesión 3+4 — Automación con Python

Reemplaza los pasos manuales de UMG con un script Python que UE Editor ejecuta una vez.

## Pre-requisitos

1. **C++ compilado** (Live Coding o rebuild). Las clases `UMainMenuWidget`, `UDeckSelectionWidget`, `ATheGreatBookPlayerController` deben existir en runtime.
2. **DataTables importadas** (sesión 2): `DT_Cards` y `DT_PresetDecks` en `/Game/Data/`.
3. **BP_TheGreatBookGameMode** creado (sesión 2) en `/Game/`.
4. **Python Editor Script Plugin** habilitado:
   - `Editar → Plugins → buscar "Python" → Python Editor Script Plugin → Enable`
   - Reiniciar Editor si lo activas por primera vez.

## Ejecutar el script

### Opción A: Editor menu

1. Menú superior del Editor: **Herramientas → Ejecutar script de Python** (o "Tools → Execute Python Script").
2. Navegar a: `C:\Users\Rinco\.gemini\antigravity\scratch\TheGreatBook_Unreal\Scripts\setup_umg.py`
3. Click **Abrir**.
4. El Output Log muestra mensajes `[setup_umg] ...` indicando progreso.
5. Resultado esperable: 5-10 segundos.

### Opción B: consola de Python (alternativa)

1. **Ventana → Editor Python Console** o **Output Log**, tab "Cmd": seleccionar **"Python"** del dropdown.
2. Ejecutar:
   ```python
   exec(open(r'C:\Users\Rinco\.gemini\antigravity\scratch\TheGreatBook_Unreal\Scripts\setup_umg.py').read())
   ```

## Qué hace el script

| Acción | Asset |
|---|---|
| Crea (si no existe) | `/Game/UI/WBP_MainMenu` (parent: `UMainMenuWidget`) |
| Construye layout: CanvasPanel + VerticalBox + 4 Buttons | VsAIButton, MultiplayerButton, DeckBuilderButton, QuitButton — con TextBlocks |
| Crea (si no existe) | `/Game/UI/WBP_DeckSelection` (parent: `UDeckSelectionWidget`) |
| Construye layout: CanvasPanel + Title + HorizontalBox + BackButton | `DecksContainer`, `BackButton` |
| Conecta `WBP_MainMenu.DeckSelectionWidgetClass` → `WBP_DeckSelection` | Class default |
| Crea (si no existe) | `/Game/BP_TheGreatBookPlayerController` (parent: `ATheGreatBookPlayerController`) |
| Conecta `BP_PlayerController.MainMenuWidgetClass` → `WBP_MainMenu` | Class default |
| Actualiza `BP_TheGreatBookGameMode` | `PlayerControllerClass`, `PresetDecksTable` |

## Idempotencia

Re-ejecutar es seguro: si los assets existen, solo verifica los class defaults y layout. No duplica.

## Validación

Después de ejecutar:

1. **Output Log**: ver mensaje final `[setup_umg] Setup complete.` Sin errores rojos.
2. **Content Browser**:
   - `/Game/UI/WBP_MainMenu.uasset`
   - `/Game/UI/WBP_DeckSelection.uasset`
   - `/Game/BP_TheGreatBookPlayerController.uasset`
3. **PIE**: click **Reproducir** → menú principal con 4 botones visibles.
4. Click **CONTRA IA** → abre selección de mazos (vacío sin `CreateDeckButton` BP override).
5. Click **SALIR** → cierra el juego.

## Pendientes (no automatizados)

El script NO implementa el override de `CreateDeckButton` (BlueprintImplementableEvent). Eso debe hacerse manualmente en el Designer del `WBP_DeckSelection` si quieres que aparezcan los botones de mazos visualmente. Alternativa: implementar la generación de botones en C++ puro en una próxima sesión.

## Si algo falla

| Síntoma | Causa | Fix |
|---|---|---|
| `Could not load class /Script/TheGreatBook.MainMenuWidget` | C++ no compilado | Ctrl+Alt+F11 en Editor o cerrar y compilar |
| `BP_TheGreatBookGameMode not found` | No lo creaste manualmente en sesión 2 | Crear `/Game/BP_TheGreatBookGameMode` antes |
| `DT_PresetDecks not found` | No importaste CSV | Importar `Content/Data/DT_PresetDecks.csv` |
| Botones sin estilo / negros | Default UE; no aplicamos theme | Setear stylebox en BP después si querés |
| `Plugin "Python Editor Script" disabled` | Habilitar en Plugins y reiniciar | — |
