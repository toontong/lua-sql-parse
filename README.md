# lemon 
C Sql parser from sqlite, the lemon is using in Atlas sharding branch. 
clone from https://github.com/winkyao/lemon

# 来源

- 原工程代码基本上来自于sqlite开源代码，作者winkyao已忘记是来自哪个版本的sqlite。
- 针对扣出来的代码，作者winkyao进行了大量裁减，以便编译上不再依赖sqlite。
- sqlite的sql语法与MySQL语法有着小量的差异，winkyao主要添加了几种原来sqlite不支持语法到parse.y中。
- test/目录下的代码是winkyao自己写的，并未从sqlite上搬运太多。

# 其它
- cstring工程是我(hct) 添加进来的。来自网上大人物-云风的代码。
- lua/目录是编译成lua C 模块的代码-- by hct。

# 已知问题：
- UNION 语句解释不了。

