# 玩具(Toy)

## Description

ZC God is best at logical reasoning. One day, he talks about his childhood digital toys.

The toy is like a Rubik's cube, but not a Rubik's cube. Specifically, it is not a $3 * 3 * 3 $structure, but a $4 * 2$ structure.

![Toy](https://dsa.cs.tsinghua.edu.cn/oj/attachment/f3ad/f3ad89ed82d8fe9dc52ce2c578e92259856fda89.png "Toy")

According to the play of the toy, we can repeatedly transform it in the following three ways:

A． Exchange the upper and lower lines. For example, the result of the transformation of Figure (a) is shown in Figure (b).

B． Loop right shift (ZC God knows what this means from an early age). For example, the result of the transformation of Figure (b) is shown in Figure (c).

C． The center rotates clockwise. For example, the result of the transformation of Figure (c) is shown in Figure (d).

ZC God is a genius in this respect. He often has a hand that has not dried his nose, the other hand has quickly restored the toy in any state to the initial state shown in Figure (a). In the year when the material was extremely scarce, ZC God had only one such toy; today, the material is extremely rich, and you have many toys in different states. Now, please restore them all.

## Input

The first line is a positive integer, which is the total number of Rubik's cube toys you have.

In each one of the following N lines, 8 positive integers, i.e. an arrangement of 1~8, are included, indicating the current state of the toy.

Here, the representation rule of the cube state is that the first four numbers represent the first line of the cube from left to right, and the last four numbers represent the second line from right to left. For example, the initial state is expressed as "1 2 3 4 5 6 7 8".

## Output

A total of N lines, each line containing an integer, in turn corresponding to the minimum number of transform needs to be performed to restore each toy.

In particular, if a toy is not recoverable, the corresponding line outputs -1.

## Example

Input

```text
2
1 2 3 4 5 6 7 8
8 6 3 5 4 2 7 1
```

Output

```text
0
2
```

## Restrictions

For 60% of the data, $N = 1$

For 100% of the data,$1 <= N \leq 1,000$

Time: 1 sec

Memory: 20 MB

## Hints

State transition diagram and its search

## 描述

ZC神最擅长逻辑推理，一日，他给大家讲述起自己儿时的数字玩具。

该玩具酷似魔方，又不是魔方。具体来说，它不是一个$3 * 3 * 3$的结构，而是$4 * 2$的结构。

![Toy](https://dsa.cs.tsinghua.edu.cn/oj/attachment/f3ad/f3ad89ed82d8fe9dc52ce2c578e92259856fda89.png "Toy")

按照该玩具约定的玩法，我们可反复地以如下三种方式对其做变换：

A． 交换上下两行。比如，图(a)经此变换后结果如图(b)所示。

B． 循环右移（ZC神从小就懂得这是什么意思的）。比如，图(b)经此变换后结果如图(c)所示。

C． 中心顺时针旋转。比如，图(c)经此变换后结果如图(d)所示。

ZC神自小就是这方面的天才，他往往是一只手还没揩干鼻涕，另一只手已经迅速地将处于任意状态的玩具复原至如图(a)所示的初始状态。物质极其匮乏的当年，ZC神只有一个这样的玩具；物质极大丰富的今天，你已拥有多个处于不同状态的玩具。现在，就请将它们全部复原吧。

## 输入

第一行是一个正整数，即你拥有的魔方玩具总数N。

接下来共N行，每行8个正整数，是1~8的排列，表示该玩具的当前状态。

这里，魔方状态的表示规则为：前四个数自左向右给出魔方的第一行，后四个数自右向左给出第二行。比如，初始状态表示为“1 2 3 4 5 6 7 8”。

## 输出

共N行，各含一个整数，依次对应于复原各玩具所需执行变换的最少次数。

特别地，若某个玩具不可复原，则相应行输出-1。

## 样例

见英文题面

## 限制

对于60%的数据，$N = 1$

对于100%的数据，$1 <= N \leq 1,000$

时间：1 sec

空间：20MB

## 提示

状态转换图及其搜索
