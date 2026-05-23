"""
Setup UMG widgets and Blueprints for The Great Book.

Run from Editor: Tools → Execute Python Script → select this file.

WidgetTree (layout) is built in C++ NativeConstruct — this script only
creates the empty WBPs and wires class defaults. No widget tree manipulation
(UE 5.7 marks WidgetTree as protected from Python).

Creates (if not exist):
  /Game/UI/WBP_MainMenu          (parent: UMainMenuWidget)
  /Game/UI/WBP_DeckSelection     (parent: UDeckSelectionWidget)
  /Game/BP_TheGreatBookPlayerController

Wires:
  WBP_MainMenu.DeckSelectionWidgetClass        → WBP_DeckSelection
  BP_TheGreatBookPlayerController.MainMenuWidgetClass → WBP_MainMenu
  BP_TheGreatBookGameMode.PlayerControllerClass → BP_TheGreatBookPlayerController
  BP_TheGreatBookGameMode.PresetDecksTable     → DT_PresetDecks

Idempotent: re-running won't duplicate assets.
"""

import unreal


asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
asset_lib = unreal.EditorAssetLibrary


def log(msg):
    unreal.log(f"[setup_umg] {msg}")


def load_or_none(path):
    return asset_lib.load_asset(path) if asset_lib.does_asset_exist(path) else None


def load_class(class_path):
    cls = unreal.load_class(None, class_path)
    if cls is None:
        unreal.log_error(f"[setup_umg] Could not load class {class_path}. Compile C++ first.")
    return cls


def create_wbp(name, package_path, parent_class):
    asset_path = f"{package_path}/{name}"
    existing = load_or_none(asset_path)
    if existing:
        log(f"WBP {asset_path} already exists; reusing.")
        return existing
    factory = unreal.WidgetBlueprintFactory()
    factory.parent_class = parent_class
    wbp = asset_tools.create_asset(name, package_path, None, factory)
    log(f"Created WBP {asset_path}")
    return wbp


def create_bp(name, package_path, parent_class):
    asset_path = f"{package_path}/{name}"
    existing = load_or_none(asset_path)
    if existing:
        log(f"BP {asset_path} already exists; reusing.")
        return existing
    factory = unreal.BlueprintFactory()
    factory.parent_class = parent_class
    bp = asset_tools.create_asset(name, package_path, None, factory)
    log(f"Created BP {asset_path}")
    return bp


def set_cdo_prop(bp_asset, prop_name, value):
    cls = bp_asset.generated_class()
    if cls is None:
        unreal.log_error(f"Blueprint {bp_asset.get_path_name()} has no generated_class. Compile first.")
        return False
    cdo = unreal.get_default_object(cls)
    try:
        cdo.set_editor_property(prop_name, value)
        return True
    except Exception as e:
        unreal.log_error(f"set_editor_property('{prop_name}') on {bp_asset.get_name()} failed: {e}")
        return False


def compile_and_save(bp_asset):
    unreal.BlueprintEditorLibrary.compile_blueprint(bp_asset)
    asset_lib.save_loaded_asset(bp_asset)


def run():
    main_cls = load_class('/Script/TheGreatBook.MainMenuWidget')
    deck_cls = load_class('/Script/TheGreatBook.DeckSelectionWidget')
    pc_cls = load_class('/Script/TheGreatBook.TheGreatBookPlayerController')
    if not all([main_cls, deck_cls, pc_cls]):
        unreal.log_error("Missing C++ classes. Aborting.")
        return

    # 1. WBPs (empty layout — C++ NativeConstruct builds it)
    wbp_main = create_wbp('WBP_MainMenu', '/Game/UI', main_cls)
    wbp_deck = create_wbp('WBP_DeckSelection', '/Game/UI', deck_cls)
    compile_and_save(wbp_main)
    compile_and_save(wbp_deck)

    # 2. Wire WBP_MainMenu.DeckSelectionWidgetClass = WBP_DeckSelection.generated_class
    set_cdo_prop(wbp_main, 'DeckSelectionWidgetClass', wbp_deck.generated_class())
    compile_and_save(wbp_main)

    # 3. PlayerController BP
    bp_pc = create_bp('BP_TheGreatBookPlayerController', '/Game', pc_cls)
    compile_and_save(bp_pc)
    set_cdo_prop(bp_pc, 'MainMenuWidgetClass', wbp_main.generated_class())
    compile_and_save(bp_pc)

    # 4. Update existing BP_TheGreatBookGameMode
    bp_gm = load_or_none('/Game/BP_TheGreatBookGameMode')
    if bp_gm:
        set_cdo_prop(bp_gm, 'PlayerControllerClass', bp_pc.generated_class())
        dt_decks = load_or_none('/Game/Data/DT_PresetDecks')
        if dt_decks:
            set_cdo_prop(bp_gm, 'PresetDecksTable', dt_decks)
        else:
            unreal.log_warning("DT_PresetDecks not found at /Game/Data/. Import CSV first.")
        compile_and_save(bp_gm)
    else:
        unreal.log_warning("BP_TheGreatBookGameMode not found at /Game/. Create it manually.")

    log("Setup complete.")


run()
