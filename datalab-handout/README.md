# Lab 1-DataLab Answer

[toc]

## bitXor

```c
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
```

可以通过画卡诺图解出异或的与非表达式

## tmin

```c
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
```

## isTmax

```c
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
```

思路就在于把 x 用一种方法转化，在其为 Tmax 时为 0，否则为 1。我们可以想到， (Tmax + Tmax + 1) 就是全 1 串，此时取反就能得到 0。但要注意一下 -1 的情况，它也能得到 0，所以要排除一下。

## allOddBits

```c
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
```

用一个 0xAAAAAAAA 的掩码来做就行。由于题目限制只能定义 0x00 - 0xFF 的数，所以要构建一下。

## negate

```c
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
```

阿贝尔群中的逆元，求补即可。

## isAsciiDigit

```c
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
```

定义一个数 num_max，加上大于 39 的数符号就会变，一个数 num_max，加上小于 30 的数符号就会变。

## conditional

```c
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
	x = ~x + 1; // 全 0 或全 1
	return (x & y) | (~x & z);
}
```

x 全 1 时，y 生效， x 全 0 时，z 生效。

## isLessOrEqual

```c
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
```

就通过 y-x 来比大小即可。

## logicalNeg

```c
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
```

除了 0 之外，所有数的补码都符号相反。

## howManyBits

```c
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
```

思路就是正数就找第一位 0，负数就找第一位 1，然后位数 + 1（符号位）就是结果了。还可以负数取反来找 0 更加方便。

## floatFloat2Int

```c
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
```

先排除无穷和 NaN 是溢出值，以及指数小于 0 时一定是 0。然后通过 (阶码 - Bias) 得到要位移的值进行位移就可以，记得负数要返回补码。

## floatPower2

```c
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
```
