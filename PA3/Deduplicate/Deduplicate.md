# 重名剔除(Deduplicate)

## Description

Mr. Epicure is compiling an encyclopedia of food. He had collected a long list of candidates nominated by several belly-gods. As candidates in list are nominated by several people, duplication of name is inevitable. Mr. Epicure pay you a visit for help. He request you to remove all duplication, which is thought an easy task for you. So please hold this opportunity to be famous to all belly-gods.

## Input

1 integer in fist line to denote the length of nomination list. In following n lines, each nomination is given in each line.

## Output

All the duplicated nomination (only output once if duplication appears more multiple times), which is sorted in the order that duplication appears firstly.

## Example

Input

```markdown
10
brioche
camembert
cappelletti
savarin
cheddar
cappelletti
tortellni
croissant
brioche
mapotoufu
```

Output

```markdown
cappelletti
brioche
```

## Restrictions

$1 < n < 6 * 10^5$

All nominations are only in lowercase. No other character is included. Length of each item is not greater than 40.

Time: 2 sec

Memory: 256 MB

## Hints

Hash

## 描述

Epicure先生正在编撰一本美食百科全书。为此，他已从众多的同好者那里搜集到了一份冗长的美食提名清单。既然源自多人之手，其中自然不乏重复的提名，故必须予以筛除。Epicure先生因此登门求助，并认定此事对你而言不过是“一碟小菜”，相信你不会错过在美食界扬名立万的这一良机

## 输入

第1行为1个整数n，表示提名清单的长度。以下n行各为一项提名

## 输出

所有出现重复的提名（多次重复的仅输出一次），且以其在原清单中首次出现重复（即第二次出现）的位置为序

## 样例

见英文题面

## 限制

$1 < n < 6 * 10^5$

提名均由小写字母组成，不含其它字符，且每项长度不超过40

时间：2 sec

空间：256 MB

## 提示

散列
