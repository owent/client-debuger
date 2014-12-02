// WebKit的编码转换有点奇怪的

window.lua = window.lua || {}
window.lua.template = {
    "基本": {
        "重载脚本&进入主场景": "utils/reload_and_goto_login_scene.lua",
        "重载脚本&进入战斗场景": "utils/reload_and_goto_fight_scene.lua",
        "重新加载配置": "utils/reload_config.lua"
    },
    "场景": {
        "暂停": "scene/pause.lua",
        "恢复": "scene/resume.lua",
        "缩放控制": "scene/scale.lua",
        "视点控制": "scene/view_point.lua"
    },
    "单位": {
        "英雄回血": "unit/hero_heal.lua"
    },
    "CD池": {
        "清空全部CD": "skill/clear_all_cd.lua"
    },
    "调试": {
        "显示当前所有单位边框": "debug/show_debug_frame_border.lua",
        "隐藏当前所有单位边框": "debug/hide_debug_frame_border.lua",
        "性能分析": {
            "启动性能分析": "debug/profile/start.lua",
            "停止性能分析": "debug/profile/stop.lua",
            "暂停性能分析": "debug/profile/disable.lua",
            "恢复性能分析": "debug/profile/enable.lua",
            "转储性能分析结果": "debug/profile/dump_to.lua"
        }
    }
};