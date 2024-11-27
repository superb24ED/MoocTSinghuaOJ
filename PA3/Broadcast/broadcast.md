# 无线广播(Broadcast)

## Description

A broadcaster wants to set up a radio broadcast transmitter in an area. There are n towns in the area, and each town has a transmitter installed and plays its own program.

However, the company only licensed the two bands FM104.2 and FM98.6, and transmitters using the same band would interfere with each other. It is known that the signal coverage of each transmitter is a circular area with a radius of 20km. Therefore, if two towns with a distance of less than 20km use the same band, they will not be able to function properly due to band interference. Listen to the show. Now give a list of towns with distances less than 20km, and try to determine whether the company can make residents of the entire region hear the broadcasts normally.

## Input

The first line is two integers n, m, which are the number of towns and the number of town pairs that are less than 20km. The next m lines, 2 integers per line, indicate that the distance between the two towns is less than 20km (numbering starts at 1).

## Output

 Output 1 if the requirement is met, otherwise -1

### Input sample

```markdown
4 3
1 2
1 3
twenty four
```

### Output sample

```markdown
1
```

## Restrictions

$ 1 ≤ n ≤ 10000 $

$ 1 ≤ m ≤ 30000 $

There is no need to consider the spatial characteristics of a given 20km town list, such as whether triangle inequality is satisfied, whether more information can be derived using transitivity, and so on.

Time: 2 sec

Space: 256MB

## Tips

BFS

## 描述

 某广播公司要在一个地区架设无线广播发射装置。该地区共有n个小镇，每个小镇都要安装一台发射机并播放各自的节目。

不过，该公司只获得了FM104.2和FM98.6两个波段的授权，而使用同一波段的发射机会互相干扰。已知每台发射机的信号覆盖范围是以它为圆心，20km为半径的圆形区域，因此，如果距离小于20km的两个小镇使用同样的波段，那么它们就会由于波段干扰而无法正常收听节目。现在给出这些距离小于20km的小镇列表，试判断该公司能否使得整个地区的居民正常听到广播节目。

## 输入

第一行为两个整数n，m，分别为小镇的个数以及接下来小于20km的小镇对的数目。 接下来的m行，每行2个整数，表示两个小镇的距离小于20km（编号从1开始）。

## 输出

如果能够满足要求，输出1，否则输出-1。

### 输入样例

```markdown
4 3
1 2
1 3
2 4
```

### 输出样例

```markdown
1
```

## 限制

$ 1 ≤ n ≤ 10000 $

$ 1 ≤ m ≤ 30000 $

不需要考虑给定的20km小镇列表的空间特性，比如是否满足三角不等式，是否利用传递性可以推出更多的信息等等。

时间：2 sec

空间：256MB

## 提示

BFS
