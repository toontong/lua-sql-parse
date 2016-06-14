
p = require "sql_parse"

local function var_dump(data, max_level, prefix)  
    if type(prefix) ~= "string" then  
        prefix = ""  
    end  
    if type(data) ~= "table" then  
        print(prefix .. tostring(data))  
    else  
        print(data)  
        if max_level ~= 0 then  
            local prefix_next = prefix .. "\t"
            print(prefix .. "{")  
            for k,v in pairs(data) do  
                io.stdout:write(prefix_next .. k .. " = ")  
                if type(v) ~= "table" or (type(max_level) == "number" and max_level <= 1) then  
                    print(v)  
                else  
                    if max_level == nil then  
                        var_dump(v, nil, prefix_next)  
                    else  
                        var_dump(v, max_level - 1, prefix_next)  
                    end  
                end  
            end  
            print(prefix .. "}")  
        end  
    end  
end  

local function vd(data, max_level)  
    var_dump(data, max_level or 10)  
end 

sql=" (SELECT fileid,parent,groupid,userid,fsize,fver,ctime,mtime,ftype,fname,fsha FROM qing_filesys_0874 WHERE groupid IN (47591274)  AND (ftype='folder' OR ext IN (238762040, 2909198606, 2783438039, 2496574992, 4178793211, 1772900202, 2179408424, 3683590359, 3058877500, 2783438039, 998470004, 1449039263, 2726961358, 1776339790, 4067239963, 473390917, 838129763, 3252019869, 410646757, 1157813739, 2663211800, 1132820390, 3956261036, 481551977, 144986711)) AND fname LIKE  '%远成%') UNION (SELECT fileid,parent,groupid,userid,fsize,fver,ctime,mtime,ftype,fname,fsha FROM qing_filesys_0790 WHERE groupid IN (17353494)  AND (ftype='folder' OR ext IN (238762040, 2909198606, 2783438039, 2496574992, 4178793211, 1772900202, 2179408424, 3683590359, 3058877500, 2783438039, 998470004, 1449039263, 2726961358, 1776339790, 4067239963, 473390917, 838129763, 3252019869, 410646757, 1157813739, 2663211800, 1132820390, 3956261036, 481551977, 144986711)) AND fname LIKE  '%远成%') UNION (SELECT fileid,parent,groupid,userid,fsize,fver,ctime,mtime,ftype,fname,fsha FROM qing_filesys_0157 WHERE groupid IN (17353885)  AND (ftype='folder' OR ext IN (238762040, 2909198606, 2783438039, 2496574992, 4178793211, 1772900202, 2179408424, 3683590359, 3058877500, 2783438039, 998470004, 1449039263, 2726961358, 1776339790, 4067239963, 473390917, 838129763, 3252019869, 410646757, 1157813739, 2663211800, 1132820390, 3956261036, 481551977, 144986711)) AND fname LIKE  '%远成%') UNION (SELECT fileid,parent,groupid,userid,fsize,fver,ctime,mtime,ftype,fname,fsha FROM qing_filesys_0241 WHERE groupid IN (28494065)  AND (ftype='folder' OR ext IN (238762040, 2909198606, 2783438039, 2496574992, 4178793211, 1772900202, 2179408424, 3683590359, 3058877500, 2783438039, 998470004, 1449039263, 2726961358, 1776339790, 4067239963, 473390917, 838129763, 3252019869, 410646757, 1157813739, 2663211800, 1132820390, 3956261036, 481551977, 144986711)) AND fname LIKE  '%远成%') UNION (SELECT fileid,parent,groupid,userid,fsize,fver,ctime,mtime,ftype,fname,fsha FROM qing_filesys_0796 WHERE groupid IN (17353500)  AND (ftype='folder' OR ext IN (238762040, 2909198606, 2783438039, 2496574992, 4178793211, 1772900202, 2179408424, 3683590359, 3058877500, 2783438039, 998470004, 1449039263, 2726961358, 1776339790, 4067239963, 473390917, 838129763, 3252019869, 410646757, 1157813739, 2663211800, 1132820390, 3956261036, 481551977, 144986711)) AND fname LIKE  '%远成%') UNION (SELECT fileid,parent,groupid,userid,fsize,fver,ctime,mtime,ftype,fname,fsha FROM qing_filesys_0428 WHERE groupid IN (23556524)  AND (ftype='folder' OR ext IN (238762040, 2909198606, 2783438039, 2496574992, 4178793211, 1772900202, 2179408424, 3683590359, 3058877500, 2783438039, 998470004, 1449039263, 2726961358, 1776339790, 4067239963, 473390917, 838129763, 3252019869, 410646757, 1157813739, 2663211800, 1132820390, 3956261036, 481551977, 144986711)) AND fname LIKE  '%远成%')  ORDER BY ftype&4 DESC LIMIT 0, 50;"

sql = "(select fileid from qing_filesys_0874) union (select fileid from qing_filesys_0873) order by fileid desc limit 1, 100;"
sql = "select count(fsize),fileid from qing_filesys_0874 as A join B where A.fileid=B.id order by fileid desc limit 1, 100;"
res, err = p.parse(sql)
if err ~= nil then
    print ("fail to parse sql, err=> " .. err )
end
assert(res)
vd(res)
