package testunicode;

import unicode;
from basic import print, sprintf;

local hello = unicode.utf8_to_uc("hello ");
local world = unicode.utf8_to_uc("world ");
print( hello, unicode.uc_to_utf8( hello + world ), "\n");
