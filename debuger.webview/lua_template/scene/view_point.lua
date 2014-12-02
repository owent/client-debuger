local current_scene = _G.game.scene_manager.current()
current_scene:pause() -- 战斗场景需要暂停，否者会自动跟踪单位
current_scene:scene():setFollowUnit(nil) -- 战斗需要关闭跟踪单位
current_scene:scene():setCamera({x = 2000, y = 255})