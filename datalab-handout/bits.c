/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */
#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */

#endif
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y)
{
	return ~((~(x & ~y)) & (~(~x & y)));
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void)
{
	return 1 << 31;
}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x)
{
	int y = x + 1;
	int a = ~(x + y); // 如果 x 等于 Tmax 或 -1，那么 a 为 0
	// 排除 -1
	int b = !y;
	int c = a + b;
	return !c;
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x)
{
	// 构建掩码
	int mask = 0xAA;
	mask += (mask << 8);
	mask += (mask << 16);
	return !((x & mask) ^ mask);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x)
{
	return (~x + 1);
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x)
{
	int sign = 0x1 << 31;
	int num_max = ~(sign | 0x39);
	int num_min = ~0x30 + 1;
	int smaller_than_39 = !((num_max + x) >> 31);
	int bigger_than_30 = !((num_min + x) >> 31);
	return smaller_than_39 & bigger_than_30;
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z)
{
	x = !!x;
	x = ~x + 1;
	return (x & y) | (~x & z);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y)
{
	int sign_x = (x >> 31) & 0x1;
	int sign_y = (y >> 31) & 0x1;
	int cond_a = sign_x & ~sign_y;	  // 如果 x < 0，y > 0，则必成立
	int cond_c = !(~sign_x & sign_y); // 如果 x > 0，y <0，则一定不成立

	int neg_x = ~x + 1;
	int y_sub_x = y + neg_x;
	int cond_b = !((y_sub_x >> 31) & 0x1); // 如果 y - x > 0，则成立

	return (cond_a | cond_b) & cond_c;
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x)
{
	int y = ~x + 1;
	int sign_x = (x >> 31);
	int sign_y = (y >> 31);
	int cond_a = (sign_x | sign_y) + 1;
	return cond_a;
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x)
{
	int sign;
	int b0, b1, b2, b4, b8, b16;

	// 正数不变，负数取反
	sign = x >> 31;
	sign = (sign << 31) >> 31;
	x = x ^ sign;

	// 找到第一位 1
	b16 = !!(x >> 16) << 4; // 看 x 高 16 位是否有 1
	x = x >> b16;			// 如果有，那么 x 往右移 16 位
	b8 = !!(x >> 8) << 3;	// 同理
	x = x >> b8;
	b4 = !!(x >> 4) << 2;
	x = x >> b4;
	b2 = !!(x >> 2) << 1;
	x = x >> b2;
	b1 = x >> 1;
	x = x >> b1;
	b0 = !!x;
	return b16 + b8 + b4 + b2 + b1 + b0 + 1;
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf)
{
	int tail, order_code, sign;
	tail = uf & 0x7FFFFF;		  // 尾数
	order_code = uf & 0x7F800000; // 阶码
	sign = uf & 0x80000000;

	// 整个思路就是：
	// 要乘以 2，那么要不尾数乘 2，阶码不变，要不阶码加 1，尾数不变

	// 无穷和 NaN 直接返回原数
	if(order_code == 0x7F800000)
	{
		return uf;
	}
	// 阶码是 0 的话，直接左移一位加上符号位就行了
	// 如果左移一位尾数没越界，那么也就是阶码不变，尾数乘以 2
	// 如果左移一位尾数越界，那么正好阶码多了 1，由于阶码 0 和 1 表示的数相同，所以结果正确
	if (order_code == 0)
	{
		return (uf << 1) + sign;
	}
	order_code += 0x800000;
	return sign + order_code + tail;
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf)
{
	int tail, order_code, sign, power, res;
	sign = uf & (1 << 31);
	tail = uf & 0x7FFFFF;
	order_code = (uf & 0x7F800000) >> 23;

	// 阶码全 1 或大于 31 返回溢出值
	if(order_code == 255 || (order_code-127) > 31)
	{
		return 0x80000000;
	}
	// 阶码小于 0 一定返回 0
	if((order_code-127) < 0)
	{
		return 0;
	}

	// 通过 (阶码 - Bias) 判断移动的位数
	if(order_code == 0)
	{
		power = -126;
	}
	else
	{
		tail = tail + 0x800000;
		power = order_code - 127;
	}

	// 看尾数要移动多少位
	if (power <= 23)
	{
		res = tail >> (23 - power);
	}
	else
	{
		res = tail << (power - 23);
	}

	// 如果越界了（占领了符号位），就返回溢出值
	// 注意 == 的优先级大于 &，括号不能省
	if((res & (1 << 31)) == (1 << 31))
	{
		return 0x80000000;
	}
	else
	{
		res =  res | sign;
		// 负数的话要返回补码
		if(res >> 31)
		{
			return ~(res&0x7FFFFFFF)+1;
		}
		else
		{
			return res;
		}
		
	}
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x)
{
	// 知道浮点数的结构很容易就写出来了
	int INF;
	INF = 0x7f800000;

	if(x < -126) return 0;
	if(x > 127) return INF;

	x += 127;
	return (x << 23);
}
