local current_scene = _G.game.scene_manager.current()
for k, v in pairs(current_scene.units) do
    v:showDebugHelperFrame()
end