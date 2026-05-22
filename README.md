# The Great Book — Unreal Engine port

Port del TCG bíblico **The Great Book** (originalmente Godot 4) a Unreal Engine 5.4.

Proyecto hermano de [`../TheGreatBook`](../TheGreatBook).

## Estado

**Sesión 1 / ~30 sesiones estimadas** — solo skeleton.

- Project skeleton (compilable, sin gameplay)
- Documentación de mapping Godot → Unreal en [`Docs/GODOT_TO_UNREAL_MAPPING.md`](Docs/GODOT_TO_UNREAL_MAPPING.md)
- Clases C++ base: `GameMode`, `GameState`, `PlayerController`, `CardActor`, `CardData` USTRUCT
- Config básico (`DefaultEngine.ini`, `DefaultGame.ini`, `DefaultInput.ini`)

**No funciona como juego todavía**: faltan assets, lógica de gameplay, UI, cartas, IA, multiplayer, tutorial.

## Pre-requisitos

- Unreal Engine 5.4 o superior instalado
- Visual Studio 2022 con workload "Game development with C++"
- Windows 10/11

## Setup

1. Click derecho en `TheGreatBook.uproject` → **Generate Visual Studio project files**
2. Abrir `TheGreatBook.sln` en VS2022
3. Build Configuration: `Development Editor` + `Win64`
4. Build (F7) — debería compilar el skeleton
5. Run (F5) o doble-click en `TheGreatBook.uproject` para abrir en el editor

## Estructura

```
TheGreatBook_Unreal/
├── TheGreatBook.uproject           # archivo del proyecto
├── Source/
│   ├── TheGreatBook.Target.cs      # target build runtime
│   ├── TheGreatBookEditor.Target.cs# target build editor
│   └── TheGreatBook/
│       ├── TheGreatBook.Build.cs   # dependencias del módulo
│       ├── TheGreatBook.h/.cpp     # módulo principal
│       ├── Public/                 # headers públicos
│       │   ├── CardData.h          # USTRUCT FCardDataRow + FCardInstance
│       │   ├── CardActor.h         # ACardActor
│       │   ├── TheGreatBookGameMode.h
│       │   ├── TheGreatBookGameState.h
│       │   └── TheGreatBookPlayerController.h
│       └── Private/                # implementaciones .cpp
├── Config/
│   ├── DefaultEngine.ini
│   ├── DefaultGame.ini
│   └── DefaultInput.ini
├── Content/                        # (vacío — assets pendientes)
└── Docs/
    └── GODOT_TO_UNREAL_MAPPING.md  # guía de traducción
```

## Roadmap

Ver [`Docs/GODOT_TO_UNREAL_MAPPING.md`](Docs/GODOT_TO_UNREAL_MAPPING.md) sección "Plan de implementación por sesiones".

## Licencia

Pendiente.
