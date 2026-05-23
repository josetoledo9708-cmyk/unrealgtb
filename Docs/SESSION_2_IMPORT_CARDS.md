# Sesión 2 — Importar DataTables en Editor

Tras la sesión 2 se generaron 2 CSVs + 2 USTRUCTs. Para que sean usables como `UDataTable*` en C++/Blueprint hay que importarlos manualmente en el Editor.

## Archivos generados

| Archivo | Struct | Filas |
|---|---|---|
| `Content/Data/DT_Cards.csv` | `FCardDataRow` (en `CardData.h`) | 89 cartas |
| `Content/Data/DT_PresetDecks.csv` | `FPresetDeckRow` (en `PresetDeckData.h`) | 3 mazos |

## Pasos en Unreal Editor

### 1. Rebuild C++ (necesario para que UE vea los structs)

- En el Editor: **Ctrl + Alt + F11** (Live Coding rebuild)
- O cerrar Editor → comando: `Build.bat TheGreatBookEditor Win64 Development -Project=...uproject` → reabrir

### 2. Importar `DT_Cards.csv`

1. Editor → **Almacén de contenido (Content Browser)** → carpeta `Content/Data/`
2. Click derecho → **Import to /Game/Data/**
3. Seleccionar `DT_Cards.csv`
4. Diálogo "Choose DataTable Row Type": elegir **`FCardDataRow`**
5. **Import**
6. Genera `DT_Cards.uasset`

### 3. Importar `DT_PresetDecks.csv`

1. Mismo proceso: click derecho en Content/Data → Import
2. Seleccionar `DT_PresetDecks.csv`
3. Row Type: **`FPresetDeckRow`**
4. **Import**
5. Genera `DT_PresetDecks.uasset`

### 4. Asignar DataTable al GameMode

1. Editor → Content Browser → `Content/Blueprints/`
2. Crear Blueprint hijo de `ATheGreatBookGameMode` (botón "Add → Blueprint Class")
3. Nombrar `BP_TheGreatBookGameMode`
4. Abrir BP → tab **Class Defaults**
5. Buscar prop **Cards Table** → asignar `DT_Cards`
6. (Próxima sesión añadirá `PresetDecksTable`)
7. Compile + Save

### 5. Setear GameMode por defecto

1. `Edit → Project Settings → Maps & Modes`
2. **Default GameMode**: `BP_TheGreatBookGameMode`
3. Save

## Verificación

Para verificar que las cartas se cargaron:

1. Open Level Blueprint
2. Drag `Get Game Mode` → Cast to `BP_TheGreatBookGameMode` → `Get Card Row` con `dia1`
3. Print el nombre devuelto
4. Play (PIE) → debería printear "La Luz Y Tinieblas"

## Próximas sesiones (roadmap)

- Sesión 3: UDataAsset alternativo para decks + UI de selección de mazo
- Sesión 4: UMG main menu (botones CONTRA IA, MULTIPLAYER, etc.)
- Sesión 5: ACardActor + UMG widget visual con datos del DataTable
- ...

Ver `Docs/GODOT_TO_UNREAL_MAPPING.md` para el roadmap completo de 30 sesiones.

## Notas técnicas

### CSV format

UE DataTable CSV format:
- Primera columna sin header (o `---`) → row key (FName)
- Resto de columnas: nombre exacto del UPROPERTY del USTRUCT
- Strings con comas/comillas: encapsular en `"..."` con `""` para escape
- `FText`: simple quoted string (sin namespace/key → non-localized)
- `TArray<FString>`: `"(""item1"",""item2"",""item3"")"` (paréntesis dentro de comillas)
- `TSoftObjectPtr<UTexture2D>`: path `/Game/Path/T_Tex.T_Tex` o `None`
- Enums (`ECardType`): nombre del valor sin prefijo (`DIA`, `TIERRA`, etc.)

### Si la importación falla

- Verificar UTF-8 sin BOM
- Verificar que `FPresetDeckRow` y `FCardDataRow` aparezcan en el dropdown "Row Type" — si no, falta rebuild
- Errores comunes: mismatch en el nombre de un UPROPERTY entre CSV y struct
