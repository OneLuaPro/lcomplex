/*
  --------------------------------------------------------------------------------
  MIT License

  lcomplex - Copyright (c) 2018 Luiz Henrique de Figueiredo.
                       (c) 2025 Kritzel Kratzel for OneLuaPro.

  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in 
  the Software without restriction, including without limitation the rights to 
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all 
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
  --------------------------------------------------------------------------------

  Original Copyright Notice
  -------------------------
  lcomplex.c
  C99 complex numbers for Lua 5.3
  Luiz Henrique de Figueiredo <lhf@tecgraf.puc-rio.br>
  26 Jul 2018 17:57:06
  This code is hereby placed in the public domain and also under the MIT license

  --------------------------------------------------------------------------------
  https://github.com/microsoft/STL/issues/3280
  https://github.com/OpenMathLib/OpenBLAS/issues/4073
  https://learn.microsoft.com/en-us/cpp/c-runtime-library/complex-math-support
*/

#ifdef _MSC_VER
#define _CRT_USE_C_COMPLEX_H
#endif
#include <complex.h>

#include "lua.h"
#include "lauxlib.h"

#ifdef _MSC_VER
#if LUA_FLOAT_TYPE == LUA_FLOAT_FLOAT		/* { single float */
#define Complex		_Fcomplex
#undef  l_mathop
#define l_mathop(op)	op##f
#define ZERO		0.0f
#define ONE		1.0f
#elif LUA_FLOAT_TYPE == LUA_FLOAT_LONGDOUBLE	/* }{ long double */
#define Complex		_Lcomplex
#undef  l_mathop
#define l_mathop(op)	op##l
#define ZERO		0.0l
#define ONE		1.0l
#elif LUA_FLOAT_TYPE == LUA_FLOAT_DOUBLE	/* }{ double */
#define Complex		_Dcomplex
#undef  l_mathop
#define l_mathop(op)	op
#define ZERO		0.0
#define ONE		1.0
#else
#error "numeric float type not defined"
#endif
#define Double		LUA_NUMBER
#else
#define Complex		LUA_NUMBER complex
#endif
#define MYNAME		"complex"
#define MYTYPE		MYNAME " number"
#define MYVERSION	"l" MYNAME " 1.0"

#define Z(i)		Pget(L,i)
#define O(i)		luaL_optnumber(L,i,0)

#ifdef _MSC_VER
#define radd(z,w)	(l_mathop(creal)(z)+l_mathop(creal)(w))
#define iadd(z,w)	(l_mathop(cimag)(z)+l_mathop(cimag)(w))
#define rsub(z,w)	(l_mathop(creal)(z)-l_mathop(creal)(w))
#define isub(z,w)	(l_mathop(cimag)(z)-l_mathop(cimag)(w))
#define rmul(z,w)	(l_mathop(creal)(z)*l_mathop(creal)(w)-l_mathop(cimag)(z)*l_mathop(cimag)(w))
#define imul(z,w)	(l_mathop(creal)(z)*l_mathop(cimag)(w)+l_mathop(cimag)(z)*l_mathop(creal)(w))
#define rneg(z)		(-(l_mathop(creal)(z)))
#define ineg(z)		(-(l_mathop(cimag)(z)))
#define rdiv(z,w)	((l_mathop(creal)(z)*l_mathop(creal)(w)+l_mathop(cimag)(z)*l_mathop(cimag)(w))/ \
			 (l_mathop(creal)(w)*l_mathop(creal)(w)+l_mathop(cimag)(w)*l_mathop(cimag)(w)))
#define idiv(z,w)	((l_mathop(cimag)(z)*l_mathop(creal)(w)-l_mathop(creal)(z)*l_mathop(cimag)(w))/ \
			 (l_mathop(creal)(w)*l_mathop(creal)(w)+l_mathop(cimag)(w)*l_mathop(cimag)(w)))
#else
#define cadd(z,w)	((z)+(w))
#define csub(z,w)	((z)-(w))
#define cneg(z)		(-(z))
#define cmul(z,w)	((z)*(w))
#define cdiv(z,w)	((z)/(w))
#endif
#define cconj		l_mathop(conj)

static Complex Pget(lua_State *L, int i){
  switch (lua_type(L,i)){
  case LUA_TNUMBER:
#ifdef _MSC_VER
    Complex cnum;
    cnum._Val[0] = luaL_checknumber(L,i);
    cnum._Val[1] = ZERO;
    return cnum;
#else
    return luaL_checknumber(L,i);
#endif
  case LUA_TSTRING:
    luaL_error(L,"Argument cannot be a string.");
  default:
    return *((Complex*)luaL_checkudata(L,i,MYTYPE));
  }
}

#ifdef _MSC_VER
static int Pnew(lua_State *L, Double re, Double im){
  Complex *p = lua_newuserdata(L,sizeof(Complex));
  luaL_setmetatable(L,MYTYPE);
  p->_Val[0] = re;
  p->_Val[1] = im;
  return 1;
}

static int Lcmplx(lua_State *L){
  // Extension to original lcomplex.c
  // Mimics Fortran cmplx() function
  Double re, im;
  // Check and act according to number of arguments
  if (lua_gettop(L) == 0) {
    // signature 1 - init to (0.0,0.0)
    re = ZERO;
    im = ZERO;
  }
  else if (lua_gettop(L) == 1) {
    // signature 2 - init to (arg,0.0)
    re = luaL_checknumber(L,1);
    im = ZERO;
  }
  else if (lua_gettop(L) == 2) {
    // signature 3 - init to (arg1,arg2)
    re = luaL_checknumber(L,1);
    im = luaL_checknumber(L,2);
  }
  else {
    // bailing out
    return luaL_error(L,"Wrong number of arguments.");
  }
  return Pnew(L, re, im);
}
#else
static int Pnew(lua_State *L, Complex z){
  Complex *p=lua_newuserdata(L,sizeof(Complex));
  luaL_setmetatable(L,MYTYPE);
  *p=z;
  return 1;
}
#endif

static int Leq(lua_State *L){			/** __eq(z,w) */
#ifdef _MSC_VER
  lua_pushboolean(L,l_mathop(creal)(Z(1))==l_mathop(creal)(Z(2)) &&
		  l_mathop(cimag)(Z(1))==l_mathop(cimag)(Z(2)));
#else
  lua_pushboolean(L,Z(1)==Z(2));
#endif
  return 1;
}

static int Ltostring(lua_State *L){		/** tostring(z) */
  Complex z=Z(1);
  LUA_NUMBER x = l_mathop(creal)(z);
  LUA_NUMBER y = l_mathop(cimag)(z);
  lua_settop(L,0);
  if (x!=0 || y==0) lua_pushnumber(L,x);
  if (y!=0){
    if (y==1){
      if (x!=0) lua_pushliteral(L,"+");
    }
    else if (y==-1)
      lua_pushliteral(L,"-");
    else{
      if (y>0 && x!=0) lua_pushliteral(L,"+");
      lua_pushnumber(L,y);
    }
    lua_pushliteral(L,"i");
  }
  lua_concat(L,lua_gettop(L));
  return 1;
}

#ifdef _MSC_VER
#define A(f,re,im)	static int L##f(lua_State *L) { return Pnew(L,re,im); }
#define B(f)	A(f,l_mathop(r##f)(Z(1),Z(2)),l_mathop(i##f)(Z(1),Z(2)))
#define F(f)	A(f,l_mathop(creal)(l_mathop(c##f)(Z(1))),l_mathop(cimag)(l_mathop(c##f)(Z(1))))
#define H(f)	A(f,l_mathop(r##f)(Z(1)),l_mathop(i##f)(Z(1)))
#define J(f)	A(f,l_mathop(creal)(l_mathop(c##f)(Z(1),Z(2))),l_mathop(cimag)(l_mathop(c##f)(Z(1),Z(2))))
#else
#define A(f,e)	static int L##f(lua_State *L) { return Pnew(L,e); }
#define B(f)	A(f,l_mathop(c##f)(Z(1),Z(2)))
#define F(f)	A(f,l_mathop(c##f)(Z(1)))
#endif
#define G(f)	static int L##f(lua_State *L) { lua_pushnumber(L,l_mathop(c##f)(Z(1))); return 1; }

#ifdef _MSC_VER
A(new,O(1),O(2))	/** new(x,y) */
#else
A(new,O(1)+O(2)*I)	/** new(x,y) */
#endif
B(add)			/** __add(z,w) */
B(div)			/** __div(z,w) */
B(mul)			/** __mul(z,w) */
B(sub)			/** __sub(z,w) */
#ifdef _MSC_VER
H(neg)			/** __unm(z) */
#else
F(neg)			/** __unm(z) */
#endif
G(abs)			/** abs(z) */
F(acos)			/** acos(z) */
F(acosh)		/** acosh(z) */
G(arg)			/** arg(z) */
F(asin)			/** asin(z) */
F(asinh)		/** asinh(z) */
F(atan)			/** atan(z) */
F(atanh)		/** atanh(z) */
F(conj)			/** conj(z) */
F(cos)			/** cos(z) */
F(cosh)			/** cosh(z) */
F(exp)			/** exp(z) */
G(imag)			/** imag(z) */
F(log)			/** log(z) */
#ifdef _MSC_VER
J(pow)			/** pow(z,w) */
#else
B(pow)			/** pow(z,w) */
#endif
F(proj)			/** proj(z) */
G(real)			/** real(z) */
F(sin)			/** sin(z) */
F(sinh)			/** sinh(z) */
F(sqrt)			/** sqrt(z) */
F(tan)			/** tan(z) */
F(tanh)			/** tanh(z) */

static const luaL_Reg R[] = {
  { "__add",	Ladd	},
  { "__div",	Ldiv	},
  { "__eq",	Leq	},
  { "__mul",	Lmul	},
  { "__sub",	Lsub	},
  { "__unm",	Lneg	},
  { "abs",	Labs	},
  { "acos",	Lacos	},
  { "acosh",	Lacosh	},
  { "arg",	Larg	},
  { "asin",	Lasin	},
  { "asinh",	Lasinh	},
  { "atan",	Latan	},
  { "atanh",	Latanh	},
  { "conj",	Lconj	},
  { "cos",	Lcos	},
  { "cosh",	Lcosh	},
  { "exp",	Lexp	},
  { "imag",	Limag	},
  { "log",	Llog	},
  { "new",	Lnew	},
  { "pow",	Lpow	},
  { "proj",	Lproj	},
  { "real",	Lreal	},
  { "sin",	Lsin	},
  { "sinh",	Lsinh	},
  { "sqrt",	Lsqrt	},
  { "tan",	Ltan	},
  { "tanh",	Ltanh	},
  { "tostring",	Ltostring},
  { "cmplx",	Lcmplx  },
  { NULL,	NULL	}
};

LUALIB_API int luaopen_lcomplex(lua_State *L){
  luaL_newmetatable(L,MYTYPE);
  luaL_setfuncs(L,R,0);
  lua_pushliteral(L,"_VERSION");			/** version */
  lua_pushliteral(L,MYVERSION);
  lua_settable(L,-3);
  lua_pushliteral(L,"__index");
  lua_pushvalue(L,-2);
  lua_settable(L,-3);
  lua_pushliteral(L,"I");			/** I */
#ifdef _MSC_VER
  Pnew(L,ZERO,ONE);
#else
  Pnew(L,I);
#endif
  lua_settable(L,-3);
  lua_pushliteral(L,"__pow");			/** __pow(z,w) */
  lua_pushliteral(L,"pow");
  lua_gettable(L,-3);
  lua_settable(L,-3);
  lua_pushliteral(L,"__tostring");		/** __tostring(z) */
  lua_pushliteral(L,"tostring");
  lua_gettable(L,-3);
  lua_settable(L,-3);
  lua_pushliteral(L,"__cmplx");			/** __cmplx(re,im) */
  lua_pushliteral(L,"cmplx");
  lua_gettable(L,-3);
  lua_settable(L,-3);
  return 1;
}
