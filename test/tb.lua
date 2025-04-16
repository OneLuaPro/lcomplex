package.cpath="C:\\misc\\lcomplex-100\\build64\\bin\\Release\\?.dll;"..package.cpath

local c=require("lcomplex")

print(c._VERSION)
local z1 = c.new(3,4)
local z2 = c.I
print(type(z1))
print(tostring(z1))
print(tostring(z2))

