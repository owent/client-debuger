local current_scene = _G.game.scene_manager.current()
for k, v in pairs(current_scene.units) do
    if v.conf.unit_type == v.UNIT_TYPE.HERO then -- 指定英雄类型
        if 1 == v.attr.campaign then -- 指定我方阵营
            v:setHP(v:getHP() + 500) -- 生命加500
        end
    end
end