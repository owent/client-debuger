local current_scene = _G.game.scene_manager.current()
local campain = current_scene:getCampaign(1)
for k, v in pairs(campain.cd_pool) do
    v.left = 0.01
end
