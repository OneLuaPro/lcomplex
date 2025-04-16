local c=require("lcomplex")
local cmplx = c.cmplx
local sqrt = c.sqrt
local i = c.I
local real = c.real
local imag = c.imag

print("Getting library version ...")
print(c._VERSION)
print("Done\n")

print("Testing sqrt(-1) = i ...")
local z0=cmplx(-1)
assert(sqrt(z0) == i)
print("OK\n")

print("Testing real(i*i) = -1 ...")
assert(real(i*i) == -1.0)
print("OK\n")

print("Testing creation with z1 = new(3,4) ...")
local z1 = c.new(3,4)
assert(type(z1) == "userdata")
print("z1 = "..tostring(z1))
print("Done\n")

print("Testing creation with z2 = cmplx(5,6) ...")
local z2 = cmplx(5,6)
assert(type(z2) == "userdata")
print("z2 = "..tostring(z1))
print("Done\n")

print("Testing addition z1+z2 = 8+10i...")
print("z1+z2 = "..tostring(z1+z2))
assert(real(z1+z2) == 8.0 and imag(z1+z2) == 10.0)
print("Done\n")

print("Testing subtraction z1-z2 = -2-2i...")
print("z1-z2 = "..tostring(z1-z2))
assert(real(z1-z2) == -2.0 and imag(z1-z2) == -2.0)
print("Done\n")

print("Testing multiplication z1*z2 = -9+38i...")
print("z1*z2 = "..tostring(z1*z2))
assert(real(z1*z2) == -9.0 and imag(z1*z2) == 38.0)
print("Done\n")

print("Testing division z1/z2 = 39/61+i*2/61...")
print("z1/z2 = "..tostring(z1/z2))
assert(real(z1/z2) == 39/61 and imag(z1/z2) == 2/61)
print("Done\n")


