package popen;

import basic;
import file;

local fh = file.popen("/bin/ls", "r");
local line;
while ((line = file.gets(fh)) != @nil)
	basic.print(line);
file.close(fh);
