
# 循环移位(Cycle)

## Description

Cycle shifting refers to following operation on the sting. Moving first letter to the end and keeping rest part of the string. For example, apply cycle shifting on ABCD will generate BCDA. Given any two strings, to judge if arbitrary times of cycle shifting on one string can generate the other one.

## Input

There m lines in the input, while each one consists of two strings separated by space. Each string only contains uppercase letter 'A'~'Z'.

## Output

For each line in input, output YES in case one string can be transformed into the other by cycle shifting, otherwise output NO.

## Example

Input

```markdown
AACD CDAA
ABCDEFG EFGABCD
ABCD ACBD
ABCDEFEG ABCDEE
```

Output

```markdown
YES
YES
NO
NO
```

## Restrictions

$0 \leq m \leq 5000$

$1 \leq |S1|, |S2| \leq 10^5$

Time: 2 sec

Memory: 256 MB

## 描述

所谓循环移位是指。一个字符串的首字母移到末尾, 其他字符的次序保持不变。比如ABCD经过一次循环移位后变成BCDA

给定两个字符串，判断它们是不是可以通过若干次循环移位得到彼此

## 输入

由m行组成，每行包含两个由大写字母'A'~'Z'组成的字符串，中间由空格隔开

## 输出

对于每行输入，输出这两个字符串是否可以通过循环移位得到彼此：YES表示是，NO表示否

## 样例

见英文题面

## 限制

$0 ≤ m ≤ 5000$

$1 ≤ |S1|, |S2| ≤ 10^5$

时间：2 sec

内存：256 MB
