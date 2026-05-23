# Sesión 3+4 — UMG Main Menu + Deck Selection

Tras esta sesión se generaron 2 clases C++ base de UMG. Hay que crear los **Blueprints (WBP)** hijos en el Editor para diseñar el layout visual.

## Archivos generados (C++)

| Archivo | Clase | Función |
|---|---|---|
| `Public/UI/MainMenuWidget.h/.cpp` | `UMainMenuWidget` | Base del menú principal con 4 botones |
| `Public/UI/DeckSelectionWidget.h/.cpp` | `UDeckSelectionWidget` | Selección de mazo, lista dinámica desde DataTable |

Además:
- `ATheGreatBookGameMode`: nuevos getters `GetPresetDeckById`, `GetAllPresetDecks` y propiedad `PresetDecksTable`
- `ATheGreatBookPlayerController`: nueva propiedad `MainMenuWidgetClass` y código en `BeginPlay` para mostrar el menú

## Pasos en Editor

### 1. Rebuild (necesario)

- En el Editor: **Ctrl + Alt + F11** → esperar "Live coding succeeded".

### 2. Asignar PresetDecksTable al GameMode

1. Open `BP_TheGreatBookGameMode` → tab **"Predeterminados de clase"**
2. Panel **Detalles**, sección **"Cards"**:
   - Campo **"Preset Decks Table"**: dropdown → seleccionar `DT_PresetDecks`
3. **Compilar** + **Guardar**

### 3. Crear `WBP_MainMenu` (Widget hijo de `UMainMenuWidget`)

1. Content Browser → carpeta `/Game/UI/` (créala si no existe)
2. Click derecho → **"Interfaz de usuario" → "Plano de widget"** (Widget Blueprint)
3. Diálogo "Pick Parent Class":
   - Buscar **`MainMenuWidget`** (clase C++ base)
   - Seleccionar → "Seleccionar"
4. Nombrar **`WBP_MainMenu`**
5. Doble click para abrir el UMG Designer

### 4. Diseñar layout de WBP_MainMenu

En el UMG Designer (pestaña "Designer" arriba):

1. Panel **"Paleta"** (izquierda) → drag a la zona central:
   - Un **CanvasPanel** (root) o **VerticalBox** centrado
2. Dentro del root, drag **4 botones (Button)** + asignar nombres en la pestaña "Jerarquía" (Hierarchy):
   - `VsAIButton`
   - `MultiplayerButton`
   - `DeckBuilderButton`
   - `QuitButton`
3. Para cada botón, drag un **TextBlock** como hijo y editar texto:
   - "CONTRA IA"
   - "MULTIJUGADOR (Beta)"
   - "CONSTRUCTOR DE MAZOS"
   - "SALIR"
4. Importante: los nombres exactos `VsAIButton`, `MultiplayerButton`, etc. — coinciden con `meta = (BindWidget)` en C++. Si los nombres no coinciden, **el compile del WBP fallará** con mensaje "Variable %s is not bound".

### 5. Designer: tab "Gráfico" (opcional)

- Pestaña **"Gráfico"** (arriba derecha del UMG)
- En "Defaults" / "Predeterminados de clase" del WBP:
  - Campo **"Deck Selection Widget Class"**: dropdown → seleccionar `WBP_DeckSelection` (lo creas en paso 6)
- **Compilar** + **Guardar**

### 6. Crear `WBP_DeckSelection`

1. `/Game/UI/` → click derecho → "Plano de widget"
2. Pick Parent Class: **`DeckSelectionWidget`**
3. Nombrar `WBP_DeckSelection`
4. Abrir en Designer

### 7. Diseñar layout de WBP_DeckSelection

1. Root: **CanvasPanel** o **VerticalBox**
2. Agregar:
   - Un **TextBlock** "ELIGE TU CAMINO" arriba
   - Un **HorizontalBox** llamado **`DecksContainer`** (centro) — donde irán los botones de mazos
   - Un **Button** llamado **`BackButton`** abajo con texto "Volver"
3. La función `CreateDeckButton(Deck)` es **BlueprintImplementableEvent** — debes implementarla:
   - Pestaña **"Gráfico"**
   - Click "Override Function" (panel "Funciones") → seleccionar **"Create Deck Button"**
   - En la función:
     - Create Widget → de tipo "Button" (o un WBP custom WBP_DeckCard si querés cards bonitas)
     - Set Button text al `Deck.DisplayName`
     - Add to `DecksContainer` (Add Child)
     - Return el Button
4. Compilar + Guardar

### 8. Crear `BP_TheGreatBookPlayerController` y asignar MainMenuWidgetClass

1. `/Game/` → click derecho → **"Plano Class"** (Blueprint Class)
2. Pick Parent Class: buscar **`TheGreatBookPlayerController`**
3. Nombrar `BP_TheGreatBookPlayerController`
4. Abrir → tab **Predeterminados de clase**
5. Sección **"UI"** → campo **"Main Menu Widget Class"**: dropdown → seleccionar `WBP_MainMenu`
6. **Compilar** + **Guardar**

### 9. Asignar el PlayerController al GameMode

1. Open `BP_TheGreatBookGameMode` → tab **Predeterminados de clase**
2. Sección **"Clases"** (default UE):
   - Campo **"Player Controller Class"**: dropdown → `BP_TheGreatBookPlayerController`
3. Compilar + Guardar

### 10. Play (PIE) — verificar

1. Botón **Reproducir** verde.
2. Debería aparecer el menú principal con 4 botones.
3. Click "CONTRA IA" → debería abrir la selección de mazos con 3 entradas (Edén, Abraham, José).
4. Click "Volver" → cierra deck selection.
5. Click "SALIR" → cierra el juego.

## Errores comunes

- **"Variable VsAIButton is not bound"**: nombre del botón en WBP no coincide exactamente con el UPROPERTY del header. Renombrar el button en la pestaña "Jerarquía".
- **Menú no aparece al Play**: `MainMenuWidgetClass` no asignado en `BP_TheGreatBookPlayerController`. Volver al paso 8.
- **Botones no responden**: el `OnClicked` se conecta automáticamente en `NativeConstruct`. Verificar que el rebuild Live Coding pasó OK.
- **DeckSelection sin botones**: `PresetDecksTable` no asignada al GameMode (paso 2) o `CreateDeckButton` no implementado (paso 7.3).

## Próximas sesiones

- Sesión 5: `ACardActor` con visual UMG widget de carta + texturas
- Sesión 6: Layout abanico para la mano del jugador
- Ver `Docs/GODOT_TO_UNREAL_MAPPING.md` para roadmap completo
