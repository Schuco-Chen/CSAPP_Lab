# CSAPP-Lab2-Bomb Lab

这个实验主要考察的是对汇编代码的掌握以及 gdb 的使用。

关于 gdb 的使用可以看这两个：

- [http://csapp.cs.cmu.edu/2e/docs/gdbnotes-x86-64.pdf](http://csapp.cs.cmu.edu/2e/docs/gdbnotes-x86-64.pdf)
- [http://beej.us/guide/bggdb/](http://beej.us/guide/bggdb/)

**常用命令**：

- `set args <file>`: 设置 main 函数的参数
- `run`: 启动
- `break <address>/<function>`: 在指定地址或者指定函数的最开始打断点

**Tips**:

- 可以先使用 `objdump -d bomb > bomb.txt` 来得到完整的汇编代码以便查看

- 使用 `gdbtui` 可以进入带有 tui 界面的 gdb，比原生的方便些。
  - `layout src`: 上边源代码，下边 gdb
  - `layout asm`: 上边汇编代码，下边 gdb
  - `layout split`: 上边源代码，中间汇编代码，下边 gdb

- `x86-64` 架构中，调用函数的形参列表按顺序一般保存在：`%rdi`, `%rsi`, `%rdx`, `%rcx`, `%r8`, `%r9`。

## 概览

查看 `bomb.c`，发现我们缺失了两个头文件，所以不能运行，只能反汇编可执行程序 `bomb` 来获得汇编代码。

查看 `main` 函数，大致逻辑就是首先查看有没有参数，没有就用命令行输入，否则是文件输入。然后每次输入一个字符串，通过一个函数 `phase_<n>()`，如果与它想要的字符串相同，那就成功通过，否则炸弹爆炸，一共 6 个关卡加一个隐藏关卡。

## 第 1 关

```git
0000000000400ee0 <phase_1>:
  400ee0:	48 83 ec 08          	sub    $0x8,%rsp
  400ee4:	be 00 24 40 00       	mov    $0x402400,%esi
  400ee9:	e8 4a 04 00 00       	callq  401338 <strings_not_equal>
  400eee:	85 c0                	test   %eax,%eax
  400ef0:	74 05                	je     400ef7 <phase_1+0x17>
  400ef2:	e8 43 05 00 00       	callq  40143a <explode_bomb>
  400ef7:	48 83 c4 08          	add    $0x8,%rsp
  400efb:	c3                   	retq
```

这关调用了一个 `strings_not_equal()` 函数，字面意思可以知道就是对比输入字符串和它内部的秘密字符串是否相等。然后查看返回值，为 0 通过，否则爆炸，所以我们要使 `strings_not_equal()` 返回值为 0:

```git
0000000000401338 <strings_not_equal>:
  401338:	41 54                	push   %r12
  40133a:	55                   	push   %rbp
  40133b:	53                   	push   %rbx
  40133c:	48 89 fb             	mov    %rdi,%rbx
  40133f:	48 89 f5             	mov    %rsi,%rbp
  401342:	e8 d4 ff ff ff       	callq  40131b <string_length>
  401347:	41 89 c4             	mov    %eax,%r12d
  40134a:	48 89 ef             	mov    %rbp,%rdi
  40134d:	e8 c9 ff ff ff       	callq  40131b <string_length>
  401352:	ba 01 00 00 00       	mov    $0x1,%edx
  401357:	41 39 c4             	cmp    %eax,%r12d
  40135a:	75 3f                	jne    40139b <strings_not_equal+0x63>
  40135c:	0f b6 03             	movzbl (%rbx),%eax
  40135f:	84 c0                	test   %al,%al
  401361:	74 25                	je     401388 <strings_not_equal+0x50>
  401363:	3a 45 00             	cmp    0x0(%rbp),%al
  401366:	74 0a                	je     401372 <strings_not_equal+0x3a>
  401368:	eb 25                	jmp    40138f <strings_not_equal+0x57>
  40136a:	3a 45 00             	cmp    0x0(%rbp),%al
  40136d:	0f 1f 00             	nopl   (%rax)
  401370:	75 24                	jne    401396 <strings_not_equal+0x5e>
  401372:	48 83 c3 01          	add    $0x1,%rbx
  401376:	48 83 c5 01          	add    $0x1,%rbp
  40137a:	0f b6 03             	movzbl (%rbx),%eax
  40137d:	84 c0                	test   %al,%al
  40137f:	75 e9                	jne    40136a <strings_not_equal+0x32>
  401381:	ba 00 00 00 00       	mov    $0x0,%edx
  401386:	eb 13                	jmp    40139b <strings_not_equal+0x63>
  401388:	ba 00 00 00 00       	mov    $0x0,%edx
  40138d:	eb 0c                	jmp    40139b <strings_not_equal+0x63>
  40138f:	ba 01 00 00 00       	mov    $0x1,%edx
  401394:	eb 05                	jmp    40139b <strings_not_equal+0x63>
  401396:	ba 01 00 00 00       	mov    $0x1,%edx
  40139b:	89 d0                	mov    %edx,%eax
  40139d:	5b                   	pop    %rbx
  40139e:	5d                   	pop    %rbp
  40139f:	41 5c                	pop    %r12
  4013a1:	c3                   	retq
```

可以看到，首先用 `%rbx` 和 `%rbp` 保存两个参数（也就是两个字符串的值），我们输入的字符串是第一个参数（因为我们就传给了 `phase_1()` 1 个参数，又没动过 `%rdi`），很容易可以想到这第二个参数就是秘密字符串。实际上在这我们就能明白 `%rsi` 里就是正确结果，我们只要 `x/s $rsi` 就能得到。

当然，为了保险起见也可以继续看代码，发现后面的大致过程就是先对比长度是否相等，再通过一个循环逐个字符对比。

最后结果为：

```text
Border relations with Canada have never been better.
```

## 第 2 关

```git
0000000000400efc <phase_2>:
  400efc:	55                   	push   %rbp
  400efd:	53                   	push   %rbx
  400efe:	48 83 ec 28          	sub    $0x28,%rsp
  400f02:	48 89 e6             	mov    %rsp,%rsi
  400f05:	e8 52 05 00 00       	callq  40145c <read_six_numbers>
  400f0a:	83 3c 24 01          	cmpl   $0x1,(%rsp)
  400f0e:	74 20                	je     400f30 <phase_2+0x34>
  400f10:	e8 25 05 00 00       	callq  40143a <explode_bomb>
  400f15:	eb 19                	jmp    400f30 <phase_2+0x34>
  400f17:	8b 43 fc             	mov    -0x4(%rbx),%eax
  400f1a:	01 c0                	add    %eax,%eax
  400f1c:	39 03                	cmp    %eax,(%rbx)
  400f1e:	74 05                	je     400f25 <phase_2+0x29>
  400f20:	e8 15 05 00 00       	callq  40143a <explode_bomb>
  400f25:	48 83 c3 04          	add    $0x4,%rbx
  400f29:	48 39 eb             	cmp    %rbp,%rbx
  400f2c:	75 e9                	jne    400f17 <phase_2+0x1b>
  400f2e:	eb 0c                	jmp    400f3c <phase_2+0x40>
  400f30:	48 8d 5c 24 04       	lea    0x4(%rsp),%rbx
  400f35:	48 8d 6c 24 18       	lea    0x18(%rsp),%rbp
  400f3a:	eb db                	jmp    400f17 <phase_2+0x1b>
  400f3c:	48 83 c4 28          	add    $0x28,%rsp
  400f40:	5b                   	pop    %rbx
  400f41:	5d                   	pop    %rbp
  400f42:	c3                   	retq
```

这一关一开始把栈顶指针保存到 `%rsi` 中，然后调用了 `read_six_numbers()`。看其字面意思明白这关我们得输入 6 个数字。查看这个函数的汇编代码：

```git
000000000040145c <read_six_numbers>:
  40145c:	48 83 ec 18          	sub    $0x18,%rsp
  401460:	48 89 f2             	mov    %rsi,%rdx
  401463:	48 8d 4e 04          	lea    0x4(%rsi),%rcx
  401467:	48 8d 46 14          	lea    0x14(%rsi),%rax
  40146b:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
  401470:	48 8d 46 10          	lea    0x10(%rsi),%rax
  401474:	48 89 04 24          	mov    %rax,(%rsp)
  401478:	4c 8d 4e 0c          	lea    0xc(%rsi),%r9
  40147c:	4c 8d 46 08          	lea    0x8(%rsi),%r8
  401480:	be c3 25 40 00       	mov    $0x4025c3,%esi
  401485:	b8 00 00 00 00       	mov    $0x0,%eax
  40148a:	e8 61 f7 ff ff       	callq  400bf0 <__isoc99_sscanf@plt>
  40148f:	83 f8 05             	cmp    $0x5,%eax
  401492:	7f 05                	jg     401499 <read_six_numbers+0x3d>
  401494:	e8 a1 ff ff ff       	callq  40143a <explode_bomb>
  401499:	48 83 c4 18          	add    $0x18,%rsp
  40149d:	c3                   	retq
```

我们首先要知道 `sscanf()` 函数是干嘛的，它与 `scanf()` 类似，只不过读取不是标准输入，而是指定的字符串。所以第一个参数就是我们输入的字符串，第二个参数是格式化字符串，后面就是保存到的各个变量的位置。

我们查看第二个参数：

```shell
(gdb) x/s $rsi
"%d %d %d %d %d %d"
```

可以看到是 6 个整型，与我们理解的字面意思相符。

我们记住 `%rsi` 保存了调用函数的栈顶指针，看这个代码中，可以发现，每隔 4 个字节把 `%rsi+n` (n=0, 4, 8, 12, 16, 20) 分别保存到了 `%rdx`, `%rcx`, `%r8`, `%r9`, `%rsp`, `%rsp+8` 中，正好对应了 `sscanf()` 后面的 6 个参数。

也就是说，当我们返回到 `phase_2` 后，从栈顶开始每隔 4 个字节按顺序保存了一个数字。而我们正是需要将这 6 个数字都输入正确。

继续查看代码，可以看到 `0x400f02` 将第一位与 1 对比，要求第一位必须等于 1，然后是一个循环，每一位等于前一位的两倍。

所以正确解答是：

```text
1 2 4 8 16 32
```

## 第 3 关

```git
0000000000400f43 <phase_3>:
  400f43:	48 83 ec 18          	sub    $0x18,%rsp
  400f47:	48 8d 4c 24 0c       	lea    0xc(%rsp),%rcx
  400f4c:	48 8d 54 24 08       	lea    0x8(%rsp),%rdx
  400f51:	be cf 25 40 00       	mov    $0x4025cf,%esi
  400f56:	b8 00 00 00 00       	mov    $0x0,%eax
  400f5b:	e8 90 fc ff ff       	callq  400bf0 <__isoc99_sscanf@plt>
  400f60:	83 f8 01             	cmp    $0x1,%eax
  400f63:	7f 05                	jg     400f6a <phase_3+0x27>
  400f65:	e8 d0 04 00 00       	callq  40143a <explode_bomb>
  400f6a:	83 7c 24 08 07       	cmpl   $0x7,0x8(%rsp)
  400f6f:	77 3c                	ja     400fad <phase_3+0x6a>
  400f71:	8b 44 24 08          	mov    0x8(%rsp),%eax
  400f75:	ff 24 c5 70 24 40 00 	jmpq   *0x402470(,%rax,8)
  400f7c:	b8 cf 00 00 00       	mov    $0xcf,%eax
  400f81:	eb 3b                	jmp    400fbe <phase_3+0x7b>
  400f83:	b8 c3 02 00 00       	mov    $0x2c3,%eax
  400f88:	eb 34                	jmp    400fbe <phase_3+0x7b>
  400f8a:	b8 00 01 00 00       	mov    $0x100,%eax
  400f8f:	eb 2d                	jmp    400fbe <phase_3+0x7b>
  400f91:	b8 85 01 00 00       	mov    $0x185,%eax
  400f96:	eb 26                	jmp    400fbe <phase_3+0x7b>
  400f98:	b8 ce 00 00 00       	mov    $0xce,%eax
  400f9d:	eb 1f                	jmp    400fbe <phase_3+0x7b>
  400f9f:	b8 aa 02 00 00       	mov    $0x2aa,%eax
  400fa4:	eb 18                	jmp    400fbe <phase_3+0x7b>
  400fa6:	b8 47 01 00 00       	mov    $0x147,%eax
  400fab:	eb 11                	jmp    400fbe <phase_3+0x7b>
  400fad:	e8 88 04 00 00       	callq  40143a <explode_bomb>
  400fb2:	b8 00 00 00 00       	mov    $0x0,%eax
  400fb7:	eb 05                	jmp    400fbe <phase_3+0x7b>
  400fb9:	b8 37 01 00 00       	mov    $0x137,%eax
  400fbe:	3b 44 24 0c          	cmp    0xc(%rsp),%eax
  400fc2:	74 05                	je     400fc9 <phase_3+0x86>
  400fc4:	e8 71 04 00 00       	callq  40143a <explode_bomb>
  400fc9:	48 83 c4 18          	add    $0x18,%rsp
  400fcd:	c3                   	retq
```

一步一步看下来，发现和第 2 关一样用到了 `sscanf()`，那么就不再赘述。通过 `0x400f51` 可以判断 `0x4025cf` 处是格式化字符串：

```shell
(gdb) x/s 0x4025cf
"%d %d"
```

可以看到结果应该是两个数字。通过 `0x400f6a` 处判断第一个数是否不大于 7，否则爆炸，并且还能看到是无符号数的比较。那么我们的第一个数取值范围在 0 ~ 6。

继续运行，发现在 `0x400f75` 处依据第一个数的值跳转到不同的位置（`0x402470+n*8` 这个地址所拥有的值），那么我们看一下取每个值的时候分别跳转到哪里：

```shell
(gdb) x/a 0x402470+0*8
0x402470:       0x400f7c <phase_3+57>
(gdb) x/a 0x402470+1*8
0x402478:       0x400fb9 <phase_3+118>
(gdb) x/a 0x402470+2*8
0x402480:       0x400f83 <phase_3+64>
(gdb) x/a 0x402470+3*8
0x402488:       0x400f8a <phase_3+71>
(gdb) x/a 0x402470+4*8
0x402490:       0x400f91 <phase_3+78>
(gdb) x/a 0x402470+5*8
0x402498:       0x400f98 <phase_3+85>
(gdb) x/a 0x402470+6*8
0x4024a0:       0x400f9f <phase_3+92>
```

所以，这一关的解答不只一个：

| num1 | num2 |
| --- | --- |
| 0 | 207(0xcf) |
| 1 | 311(0x137) |
| 2 | 707(0x2c3) |
| 3 | 256(0x100) |
| 4 | 389(0x185) |
| 5 | 206(0xce) |
| 6 | 682(0x2aa) |

## 第 4 关

```git
000000000040100c <phase_4>:
  40100c:	48 83 ec 18          	sub    $0x18,%rsp
  401010:	48 8d 4c 24 0c       	lea    0xc(%rsp),%rcx
  401015:	48 8d 54 24 08       	lea    0x8(%rsp),%rdx
  40101a:	be cf 25 40 00       	mov    $0x4025cf,%esi
  40101f:	b8 00 00 00 00       	mov    $0x0,%eax
  401024:	e8 c7 fb ff ff       	callq  400bf0 <__isoc99_sscanf@plt>
  401029:	83 f8 02             	cmp    $0x2,%eax
  40102c:	75 07                	jne    401035 <phase_4+0x29>
  40102e:	83 7c 24 08 0e       	cmpl   $0xe,0x8(%rsp)
  401033:	76 05                	jbe    40103a <phase_4+0x2e>
  401035:	e8 00 04 00 00       	callq  40143a <explode_bomb>
  40103a:	ba 0e 00 00 00       	mov    $0xe,%edx
  40103f:	be 00 00 00 00       	mov    $0x0,%esi
  401044:	8b 7c 24 08          	mov    0x8(%rsp),%edi
  401048:	e8 81 ff ff ff       	callq  400fce <func4>
  40104d:	85 c0                	test   %eax,%eax
  40104f:	75 07                	jne    401058 <phase_4+0x4c>
  401051:	83 7c 24 0c 00       	cmpl   $0x0,0xc(%rsp)
  401056:	74 05                	je     40105d <phase_4+0x51>
  401058:	e8 dd 03 00 00       	callq  40143a <explode_bomb>
  40105d:	48 83 c4 18          	add    $0x18,%rsp
  401061:	c3                   	retq
```

这关前半部分和前几关差不多，通过 `0x40101a` 判断出 `0x4025cf` 处是格式化字符串：

```shell
(gdb) x/s 0x4025cf
"%d %d"
```

可以看到这关的结果应该是两个数字。

在 `0x40102e` 处能够推断出第一个数字是一个小于 0xe(14) 的数字。在 `0x401051` 处推断出第二个数字必须是 0。

然后看到它调用了一个 `func4` 函数，并要求返回值为 0：

```git
0000000000400fce <func4>:
  400fce:	48 83 ec 08          	sub    $0x8,%rsp
  400fd2:	89 d0                	mov    %edx,%eax
  400fd4:	29 f0                	sub    %esi,%eax
  400fd6:	89 c1                	mov    %eax,%ecx
  400fd8:	c1 e9 1f             	shr    $0x1f,%ecx
  400fdb:	01 c8                	add    %ecx,%eax
  400fdd:	d1 f8                	sar    %eax
  400fdf:	8d 0c 30             	lea    (%rax,%rsi,1),%ecx
  400fe2:	39 f9                	cmp    %edi,%ecx
  400fe4:	7e 0c                	jle    400ff2 <func4+0x24>
  400fe6:	8d 51 ff             	lea    -0x1(%rcx),%edx
  400fe9:	e8 e0 ff ff ff       	callq  400fce <func4>
  400fee:	01 c0                	add    %eax,%eax
  400ff0:	eb 15                	jmp    401007 <func4+0x39>
  400ff2:	b8 00 00 00 00       	mov    $0x0,%eax
  400ff7:	39 f9                	cmp    %edi,%ecx
  400ff9:	7d 0c                	jge    401007 <func4+0x39>
  400ffb:	8d 71 01             	lea    0x1(%rcx),%esi
  400ffe:	e8 cb ff ff ff       	callq  400fce <func4>
  401003:	8d 44 00 01          	lea    0x1(%rax,%rax,1),%eax
  401007:	48 83 c4 08          	add    $0x8,%rsp
  40100b:	c3                   	retq
```

这个函数的代码是一个递归调用的函数，直接分析很不好分析，我们进行逆向工程将它还原成 C 语言代码，这里是我还原出来的代码：

```c
#include<stdio.h>

int fun(int x, int y, int z)
{
    int val, u;
    val = z-y;
    u = (val >> 31) & (~1);
    val += u;
    val >>= 1;
    u = val + y;
    if(u == x)
        return 0;
    if(u > x)
    {
        z = u-1;
        val = fun(x, y, z);
        val *= 2;
    }
    else
    {
        y = u+1;
        val = fun(x, y, z);
        val = val*2+1;
    }
    return val;
}

int main()
{
    int res;
    for(int i = 0;i <= 14; ++i)
    {
        res = fun(i, 0, 14);
        if(res == 0)
            printf("%d ", i);
    }
    printf("\n");
    return 0;
}
```

我们在 `main` 函数中将 0~14 都试一遍得出可以令 res 为 0 的参数，结果为：

```shell
0 1 3 7
```

那么，最终的密码字符串有四种：

```text
0 0
1 0
3 0
7 0
```

## 第 5 关

先看总体代码：

```git
0000000000401062 <phase_5>:
  401062:	53                   	push   %rbx
  401063:	48 83 ec 20          	sub    $0x20,%rsp
  401067:	48 89 fb             	mov    %rdi,%rbx
  40106a:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  401071:	00 00
  401073:	48 89 44 24 18       	mov    %rax,0x18(%rsp)
  401078:	31 c0                	xor    %eax,%eax
  40107a:	e8 9c 02 00 00       	callq  40131b <string_length>
  40107f:	83 f8 06             	cmp    $0x6,%eax
  401082:	74 4e                	je     4010d2 <phase_5+0x70>
  401084:	e8 b1 03 00 00       	callq  40143a <explode_bomb>
  401089:	eb 47                	jmp    4010d2 <phase_5+0x70>
  40108b:	0f b6 0c 03          	movzbl (%rbx,%rax,1),%ecx
  40108f:	88 0c 24             	mov    %cl,(%rsp)
  401092:	48 8b 14 24          	mov    (%rsp),%rdx
  401096:	83 e2 0f             	and    $0xf,%edx
  401099:	0f b6 92 b0 24 40 00 	movzbl 0x4024b0(%rdx),%edx
  4010a0:	88 54 04 10          	mov    %dl,0x10(%rsp,%rax,1)
  4010a4:	48 83 c0 01          	add    $0x1,%rax
  4010a8:	48 83 f8 06          	cmp    $0x6,%rax
  4010ac:	75 dd                	jne    40108b <phase_5+0x29>
  4010ae:	c6 44 24 16 00       	movb   $0x0,0x16(%rsp)
  4010b3:	be 5e 24 40 00       	mov    $0x40245e,%esi
  4010b8:	48 8d 7c 24 10       	lea    0x10(%rsp),%rdi
  4010bd:	e8 76 02 00 00       	callq  401338 <strings_not_equal>
  4010c2:	85 c0                	test   %eax,%eax
  4010c4:	74 13                	je     4010d9 <phase_5+0x77>
  4010c6:	e8 6f 03 00 00       	callq  40143a <explode_bomb>
  4010cb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
  4010d0:	eb 07                	jmp    4010d9 <phase_5+0x77>
  4010d2:	b8 00 00 00 00       	mov    $0x0,%eax
  4010d7:	eb b2                	jmp    40108b <phase_5+0x29>
  4010d9:	48 8b 44 24 18       	mov    0x18(%rsp),%rax
  4010de:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
  4010e5:	00 00
  4010e7:	74 05                	je     4010ee <phase_5+0x8c>
  4010e9:	e8 42 fa ff ff       	callq  400b30 <__stack_chk_fail@plt>
  4010ee:	48 83 c4 20          	add    $0x20,%rsp
  4010f2:	5b                   	pop    %rbx
  4010f3:	c3                   	retq
```

这关一开始把输入的字符串保存到了 `%rbx`，然后在 `%rsp+0x18` 处设置了金丝雀值。然后将 `%eax` 置 0，调用 `string_length` 函数得到我们输入的字符串长度，可以看到，要求是 6 位。

如果长度正确，接下来又将 `%eax` 置 0，然后跳转到了一个循环：

```git{.line-numbers}
  40108b:	0f b6 0c 03          	movzbl (%rbx,%rax,1),%ecx
  40108f:	88 0c 24             	mov    %cl,(%rsp)
  401092:	48 8b 14 24          	mov    (%rsp),%rdx
  401096:	83 e2 0f             	and    $0xf,%edx
  401099:	0f b6 92 b0 24 40 00 	movzbl 0x4024b0(%rdx),%edx
  4010a0:	88 54 04 10          	mov    %dl,0x10(%rsp,%rax,1)
  4010a4:	48 83 c0 01          	add    $0x1,%rax
  4010a8:	48 83 f8 06          	cmp    $0x6,%rax
  4010ac:	75 dd                	jne    40108b <phase_5+0x29>
```

这个循环以 `%rax` 为标志，从 0 开始每次加 1 直到 5（6 就退出了），第 1~4 行将我们输入的字符串的第 `%rax` 位取出来，并截取其位表示的低 4 位作为偏移量 p，然后第 5 行取出在 `0x4024b0` 处保存的一个字符串的第 p 位，保存在 `%rsp+10` 对于相应 `%rax` 的偏移位置。

`0x4024b0` 处的字符串为：

```shell
(gdb) x/s 0x4024b0
0x4024b0 <array.3449>:  "maduiersnfotvbylSo you think you can stop the bomb with ctrl-c, do you?"
```

也就是说，经过这个循环后，它将我们输入的字符串变成了另一个字符串，保存在 `%rsp+10~15` 的空间中。

再继续看总代码，退出循环之后会调用 `strings_not_equal` 函数来与 `0x40245e` 处保存的字符串对比，相同则通过，否则爆炸。

`0x40245e` 处的字符串为：

```shell
(gdb) x/s 0x40245e
0x40245e:       "flyers"
```

与上面的字符串对比，由于我们的偏移量 p 是 4 位的，范围是 0~15，所以我们要在前 16 位找出我们需要的字符，得到对应的偏移量为：0x9, 0xf, 0xe, 0x5, 0x6, 0x7。

查询 ASCII 码表，我们输入的字符只要后 4 位是上面的数字就行，这里是其中一种答案：

```text
9on567
```
