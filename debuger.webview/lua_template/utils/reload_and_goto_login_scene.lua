-- 重新加载脚本
local loader = require('utils.loader')
do
    loader.reload('bootstrap')
    class = loader.load('utils.class')
    loader = loader.load('utils.loader')
end
local scene_mgr = loader.load('logic.scene.manager')
local login_scene = loader.load('logic.scene.login_scene')
local scene_inst = scene_mgr.create(login_scene)
-- 载入登入场景
scene_inst:init()
scene_mgr.load(scene_inst, game.ui.LuaScene.EN_TT_FADE, 1)
