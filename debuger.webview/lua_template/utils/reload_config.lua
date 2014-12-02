local loader = require('utils.loader')
local conf_mgr = loader.load('data.conf_manager')
conf_mgr.instance():reload()