"""
Setup UMG widgets and Blueprints for The Great Book.

Run from Editor: Tools → Execute Python Script → select this file.

Creates (if not exist):
  /Game/UI/WBP_MainMenu          (parent: UMainMenuWidget)
  /Game/UI/WBP_DeckSelection     (parent: UDeckSelectionWidget)
  /Game/BP_TheGreatBookPlayerController

Updates:
  /Game/BP_TheGreatBookGameMode  → PlayerControllerClass + PresetDecksTable
  BP_TheGreatBookPlayerController → MainMenuWidgetClass
  WBP_MainMenu                    → DeckSelectionWidgetClass

Idempotent: re-running won't duplicate assets, only ensures bindings.
"""

import unreal


# ── helpers ───────────────────────────────────────────────────────────────────
asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
asset_lib = unreal.EditorAssetLibrary


def log(msg):
    unreal.log(f"[setup_umg] {msg}")


def load_or_none(path):
    return asset_lib.load_asset(path) if asset_lib.does_asset_exist(path) else None


def load_class(class_path):
    """class_path like '/Script/TheGreatBook.MainMenuWidget'"""
    cls = unreal.load_class(None, class_path)
    if cls is None:
        unreal.log_error(f"[setup_umg] Could not load class {class_path}. Make sure C++ is compiled.")
    return cls


def create_wbp(name, package_path, parent_class):
    """Create or load a Widget Blueprint with the given parent class."""
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
    """Create or load a regular Blueprint with the given parent class."""
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


def find_or_create_widget(tree, widget_cls, name, parent=None, set_as_variable=True):
    """Find a child widget by name on the tree, or construct one. Optionally parent it."""
    existing = tree.find_widget(name)
    if existing:
        return existing
    w = tree.construct_widget(widget_cls, name)
    if set_as_variable:
        w.set_editor_property('bIsVariable', True)
    if parent is not None:
        if hasattr(parent, 'add_child_to_canvas'):
            parent.add_child_to_canvas(w)
        else:
            parent.add_child(w)
    return w


# ── layout builders ───────────────────────────────────────────────────────────
def build_main_menu_layout(wbp_main):
    tree = wbp_main.widget_tree

    # Root: CanvasPanel
    root = tree.root_widget
    if root is None or not isinstance(root, unreal.CanvasPanel):
        root = tree.construct_widget(unreal.CanvasPanel, 'RootCanvas')
        tree.set_root_widget(root)

    # VerticalBox centered
    vbox = find_or_create_widget(tree, unreal.VerticalBox, 'MainVBox', parent=None, set_as_variable=False)
    if not vbox.get_parent():
        slot = root.add_child_to_canvas(vbox)
    else:
        slot = vbox.slot
    if isinstance(slot, unreal.CanvasPanelSlot):
        slot.set_anchors(unreal.Anchors(0.5, 0.5, 0.5, 0.5))
        slot.set_alignment(unreal.Vector2D(0.5, 0.5))
        slot.set_position(unreal.Vector2D(0.0, 0.0))
        slot.set_size(unreal.Vector2D(320.0, 360.0))

    # Buttons: name must match meta=(BindWidget) in UMainMenuWidget
    button_specs = [
        ('VsAIButton', 'CONTRA IA'),
        ('MultiplayerButton', 'MULTIJUGADOR (Beta)'),
        ('DeckBuilderButton', 'CONSTRUCTOR DE MAZOS'),
        ('QuitButton', 'SALIR'),
    ]
    for btn_name, label in button_specs:
        btn = find_or_create_widget(tree, unreal.Button, btn_name, parent=vbox, set_as_variable=True)
        # Set VBox slot padding + fill
        if isinstance(btn.slot, unreal.VerticalBoxSlot):
            btn.slot.set_padding(unreal.Margin(0, 6, 0, 6))
            btn.slot.set_horizontal_alignment(unreal.HorizontalAlignment.H_ALIGN_FILL)

        # Inner TextBlock
        tb_name = f"{btn_name}_Text"
        tb = find_or_create_widget(tree, unreal.TextBlock, tb_name, parent=btn, set_as_variable=False)
        tb.set_text(unreal.Text(label))
        tb.set_justification(unreal.HorizontalAlignment.H_ALIGN_CENTER)
        font = tb.get_editor_property('font')
        font.size = 16
        tb.set_editor_property('font', font)

    log("Main menu layout built.")


def build_deck_selection_layout(wbp_deck):
    tree = wbp_deck.widget_tree

    root = tree.root_widget
    if root is None or not isinstance(root, unreal.CanvasPanel):
        root = tree.construct_widget(unreal.CanvasPanel, 'RootCanvas')
        tree.set_root_widget(root)

    # Title
    title = find_or_create_widget(tree, unreal.TextBlock, 'TitleText', parent=None, set_as_variable=False)
    if not title.get_parent():
        title_slot = root.add_child_to_canvas(title)
    else:
        title_slot = title.slot
    if isinstance(title_slot, unreal.CanvasPanelSlot):
        title_slot.set_anchors(unreal.Anchors(0.5, 0.0, 0.5, 0.0))
        title_slot.set_alignment(unreal.Vector2D(0.5, 0.0))
        title_slot.set_position(unreal.Vector2D(0.0, 40.0))
        title_slot.set_size(unreal.Vector2D(600.0, 60.0))
    title.set_text(unreal.Text("ELIGE TU CAMINO"))
    title.set_justification(unreal.HorizontalAlignment.H_ALIGN_CENTER)
    title_font = title.get_editor_property('font')
    title_font.size = 32
    title.set_editor_property('font', title_font)

    # DecksContainer (HorizontalBox, BindWidget)
    container = find_or_create_widget(tree, unreal.HorizontalBox, 'DecksContainer', parent=None, set_as_variable=True)
    if not container.get_parent():
        c_slot = root.add_child_to_canvas(container)
    else:
        c_slot = container.slot
    if isinstance(c_slot, unreal.CanvasPanelSlot):
        c_slot.set_anchors(unreal.Anchors(0.5, 0.5, 0.5, 0.5))
        c_slot.set_alignment(unreal.Vector2D(0.5, 0.5))
        c_slot.set_position(unreal.Vector2D(0.0, 0.0))
        c_slot.set_size(unreal.Vector2D(800.0, 300.0))

    # BackButton (BindWidget)
    back_btn = find_or_create_widget(tree, unreal.Button, 'BackButton', parent=None, set_as_variable=True)
    if not back_btn.get_parent():
        bb_slot = root.add_child_to_canvas(back_btn)
    else:
        bb_slot = back_btn.slot
    if isinstance(bb_slot, unreal.CanvasPanelSlot):
        bb_slot.set_anchors(unreal.Anchors(0.5, 1.0, 0.5, 1.0))
        bb_slot.set_alignment(unreal.Vector2D(0.5, 1.0))
        bb_slot.set_position(unreal.Vector2D(0.0, -60.0))
        bb_slot.set_size(unreal.Vector2D(200.0, 50.0))

    # BackButton inner TextBlock
    back_tb = find_or_create_widget(tree, unreal.TextBlock, 'BackButton_Text', parent=back_btn, set_as_variable=False)
    back_tb.set_text(unreal.Text("VOLVER"))
    back_tb.set_justification(unreal.HorizontalAlignment.H_ALIGN_CENTER)
    back_font = back_tb.get_editor_property('font')
    back_font.size = 16
    back_tb.set_editor_property('font', back_font)

    log("Deck selection layout built.")


# ── class defaults wiring ─────────────────────────────────────────────────────
def set_cdo_prop(bp_asset, prop_name, value):
    """Set a property on the CDO of a Blueprint and mark as modified."""
    cls = bp_asset.generated_class()
    if cls is None:
        unreal.log_error(f"Blueprint {bp_asset.get_path_name()} has no generated_class. Compile first.")
        return
    cdo = unreal.get_default_object(cls)
    cdo.set_editor_property(prop_name, value)


def compile_and_save(bp_asset):
    unreal.BlueprintEditorLibrary.compile_blueprint(bp_asset)
    asset_lib.save_loaded_asset(bp_asset)


# ── main ──────────────────────────────────────────────────────────────────────
def run():
    # 0. Load C++ classes
    main_cls = load_class('/Script/TheGreatBook.MainMenuWidget')
    deck_cls = load_class('/Script/TheGreatBook.DeckSelectionWidget')
    pc_cls = load_class('/Script/TheGreatBook.TheGreatBookPlayerController')
    if not all([main_cls, deck_cls, pc_cls]):
        unreal.log_error("Missing C++ classes. Aborting.")
        return

    # 1. Ensure /Game/UI/ exists (creating an asset there will auto-create folder)
    # 2. Create / open WBPs
    wbp_main = create_wbp('WBP_MainMenu', '/Game/UI', main_cls)
    wbp_deck = create_wbp('WBP_DeckSelection', '/Game/UI', deck_cls)

    # 3. Build layouts
    build_main_menu_layout(wbp_main)
    build_deck_selection_layout(wbp_deck)

    # 4. Compile WBPs (so generated_class is up to date for CDO ops)
    compile_and_save(wbp_main)
    compile_and_save(wbp_deck)

    # 5. Wire WBP_MainMenu.DeckSelectionWidgetClass = WBP_DeckSelection.generated_class
    set_cdo_prop(wbp_main, 'DeckSelectionWidgetClass', wbp_deck.generated_class())
    compile_and_save(wbp_main)

    # 6. Create BP_TheGreatBookPlayerController
    bp_pc = create_bp('BP_TheGreatBookPlayerController', '/Game', pc_cls)
    compile_and_save(bp_pc)
    set_cdo_prop(bp_pc, 'MainMenuWidgetClass', wbp_main.generated_class())
    compile_and_save(bp_pc)

    # 7. Update BP_TheGreatBookGameMode (already exists from user)
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
