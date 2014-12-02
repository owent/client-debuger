// WebKit的编码转换有点奇怪的

function call_fn() {
    var ret = debuger.call('test_func', [document.getElementById('code').value]);
    alert(ret)
}

function alert_error(content, title) {
    jQuery('<div></div>')
        .html('<span class="ui-icon ui-icon-alert" style="float:left; margin:0 7px 20px 0;"></span>' + content + '<div style="clear: both;"></div>')
        .dialog({
        modal: true,
        'title': title || '出错啦',
        minWidth: 480,
        minHeight: 200
    });
}

(function ($, window) {
    var editor_info = {}
    $(document).ready(function () {
        // 编辑器
        editor_info.editor = CodeMirror.fromTextArea(document.getElementById("code"), {
            lineNumbers: true,
            styleActiveLine: true,
            matchBrackets: true,
            indentUnit: 4,
            smartIndent: true,
            mode: 'lua',
            extraKeys: {
                "F11": function (cm) {
                    cm.setOption("fullScreen", !cm.getOption("fullScreen"));
                },
                "Esc": function (cm) {
                    if (cm.getOption("fullScreen")) cm.setOption("fullScreen", false);
                }
            }
        });
       
        // 菜单函数
        var current_menu = null;
        $("#toolbar").mouseleave(function () {
            if (current_menu)
                current_menu.hide();
        });

        var show_menu = (function (selector) {
            if (current_menu)
                current_menu.hide();

            current_menu = $(selector);
            current_menu.show();
        });
        var close_menu = (function () {
            if (current_menu)
                current_menu.hide();
        });
        // ------ 菜单 ------

        // 模板列表
        $("#lua_template_btn").button().mouseenter(function () {
            show_menu('#lua_template_codes');
        });

        // 主题
        (function () {
            $("#editor_theme").selectmenu({
                icons: { button: "ui-icon-circle-triangle-s" },
                select: function () {
                    editor_info.editor.setOption("theme", $(this).val());
                }
            });
        })();

        // 语言
        (function () {
            $("#editor_mode").selectmenu({
                icons: { button: "ui-icon-circle-triangle-s" },
                select: function () {
                    editor_info.editor.setOption("mode", $(this).val());
                }
            });
        })();

        
        $("#code_reset_btn").button().click(function () {
            editor_info.editor.doc.setValue("");
        });

        $("#code_submit_btn").button().click(function () {
            editor_info.editor.save();
            var code_content = editor_info.editor.doc.getValue();
            if (window.debuger)
                debuger.call('run_code', [$("#select_mode").val(), code_content]);
            else
                alert_error("debuger 模块未导入, 不能执行脚本");
        });

        $("#code_editor_helper_btn").button().click(function () {
            if (window.debuger) {
                var content = debuger.getWebFileContent('editor_help.html');
                $(content).dialog({
                    modal: true,
                    'title': '编辑器帮助&快捷键',
                    minWidth: 480,
                    minHeight: 200
                });
            } else {
                alert_error("debuger 模块未导入, 无法加载帮助文件");
            }
        });

        // 代码模板列表
        (function (code_root, base_dir) {
            var menu_ele = $("#lua_template_codes");
            var add_item = function (parent_info) {
                var pn = parent_info.parent || menu_ele;

                $.each(parent_info.data, function (k, v) {
                    if ('string' == $.type(v)) {
                        pn.append($('<li></li>').html(k).attr("file", v));
                    } else {
                        var new_group = $('<ul></ul>');
                        pn.append($('<li></li>').html(k).append(new_group));
                        add_item({
                            parent: new_group,
                            data: v
                        });
                    }
                });
            }

            add_item({
                parent: undefined,
                data: code_root
            });

            menu_ele
                .menu({
                    select: function (event, ui) {
                        if (!ui.item.attr('file'))
                            return;

                        if (!window.debuger) {
                            alert_error("debuger 模块未导入, 无法加载本地文件");
                            close_menu();
                            return;
                        }

                        var file_path = base_dir + "/" + ui.item.attr('file')
                        var code = debuger.getWebFileContent(file_path);
                        if (code)
                            editor_info.editor.doc.setValue(code);
                        else
                            alert_error("读取脚本模板文件 '" + file_path + "' 失败!");
                        close_menu();
                    }
                })
                .position({ my: 'left top', at: 'left bottom', of: '#lua_template_btn' })
                .hide();
        })(window.lua.template, 'lua_template');
    });

})(jQuery, window);

