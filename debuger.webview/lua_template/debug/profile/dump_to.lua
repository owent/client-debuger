-- local data = profile.dump_to() -- 转储统计结果并返回
profile.dump_to('D:/prof.csv') -- 转储统计结果到文件

-- 重置统计数据
profile.reset()
-- 停止统计
-- profile.stop()