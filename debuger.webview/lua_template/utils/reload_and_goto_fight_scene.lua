-- 重新加载脚本
local loader = require('utils.loader')
do
    loader.reload('bootstrap')
    class = loader.load('utils.class')
    loader = loader.load('utils.loader')
end
local scene_mgr = loader.load('logic.scene.manager')
local scene_inst = scene_mgr.createFightScene(1001, 1) -- 第一个参数是战斗类型ID，第二个参数是战场level id
if false == scene_inst:init() then
    log_error('init fight scene failed')
end
-- 物理场景调试
scene_mgr.load(scene_inst, game.ui.LuaScene.EN_TT_FADE, 1)