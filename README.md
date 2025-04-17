# lcomplex
Lua complex numbers support module - based on `lcomplex`  (https://web.tecgraf.puc-rio.br/~lhf/ftp/lua/ar/lcomplex-100.tar.gz) and extended for MSVC compilers with limited `complex.h` support. The numerical precision (double, float, or long double) automatically scales with the settings in `luaconf.h`.

## Creation of Complex Numbers

Complex numbers in Lua are created as `userdata`.

```lua
local c = require("lcomplex")
local z0 = c.new()	-- inits to (0+0i)
local z1 = c.new(2)	-- inits to (2+0i)
local z2 = c.new(2,3)	-- inits to (2+3i)
```

As an extension to the original `lcomplex` module, the function `cmplx()` is introduced, which mimics the corresponding, classical Fortran function with the same name and behaves essentially in the same way as `new()`.

```lua
local c = require("lcomplex")
local cmplx = c.cmplx
local z0 = cmplx(2,3)	-- inits to (2+3i)
```

## Available Operators

```lua
local c = require("lcomplex")
local cmplx = c.cmplx
local i = c.I

local z1 = cmplx(3,4)
local z2 = cmplx(5,6)

-- Addition
local sum = z1 + z2
-- Subtraction
local sub = z1 - z2
-- Multiplication
local mul = z1 * z2
-- Division
local div = z1 / z2
-- Exponentiation
local expo = i^i
-- Negation
local neg = -z1
-- Equality
print(z1 == z2)
-- Print as String
print(tostring(z1))
```

## Available Functions

```lua
atan(z)      log(z)      sqrt(z)     tanh(z)
abs(z)       atanh(z)    pow(z,w)  
acos(z)      conj(z)     proj(z) 
acosh(z)     cos(z)      real(z)  
arg(z)       cosh(z)     sin(z) 
asin(z)      exp(z)      sinh(z)
asinh(z)     imag(z)     tan(z)
```

Example:

```Lua
local c = require("lcomplex")
local z0 = c.cmplx(2,3)
print(tostring(c.cosh(z0)))

-- Result:
-- -3.7245455049153+0.51182256998738i
```

## License

See https://github.com/OneLuaPro/lcomplex/blob/master/LICENSE.
