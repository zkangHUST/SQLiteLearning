# SQLite 学习笔记

## 0. 概述

首发 2000.5.29

特点：

- 零配置
- 内嵌的
- 事务性
- 线程安全
- 轻量级
- 可定制
- 跨平台

## 1.sqlite 架构

|前端模块|作用|
|--|--|
|tokenizer|将sql语句切分成token|
|parse|根据token生成parse tree|
|code generator|生成字节码程序|

|后端模块|作用|
|--|--|
|vm|执行字节码程序，完成对数据库的操作|
|B/B+- tree|把数据组织成树的结构，方便vm增删改查|
|pager:|缓存管理，实现事务ACID属性|
|The operating system interface|对不同的操作系统提供统一的接口。|

sqlite可以分为前端和后端

前端包括：tokenizer, Parser, Code generator
后端包括：Virtual machine, B+ tree, Pager, OS Interface

前端对application输入的SQL语句进行预处理，解析，优化，然后生成字节码程序，后端可以执行。

tokenizer:

tokenizer把SQL语句切分成一个个token

parser:

parser分析产生的token，产生一棵经过优化的解析树

code generator:

遍历解析树，生成等价的字节码程序

前端实现了sqlite3_prepare函数


后端引擎解释字节码程序，做实际的数据库操作，包含四个部分

虚拟机：

虚拟机是内部字节码语句的解释器，它执行字节码程序，实现对数据库的最终操作。

B/B+- tree
B/B+- tree模块把数据组织成树的结构，表和索引分别在不同的B+-和B-树中。这个模块的主要作用是管理数据，帮助虚拟机进行查询，插入和删除，还有创建新的树，删除旧的树。


pager:
pager模块在本机文件的顶部实现了面向页面的数据库文件抽象。它管理(数据库页面的)内存缓存(B/B+-tree模块使用的缓存)，此外，还管理文件锁定和日志记录，以实现事务ACID属性。

The operating system interface

操作系统接口模块对不同的操作系统提供了一个统一的接口。

sqlite3_bind_*, sqlite3_step, sqlite3_column_*, sqlite3_reset and sqlite3_finalize 等函数是由后端实现的。

## 2. sqlite的弊端

sqlite的设计目标就是尽量简单，为了简单，因此导致了一些低效的实现。sqlite的弊端主要包括以下几点：

1. 不支持某些SQL-92特性(很多企业级的数据库都是支持的)
2. 低并发，允许多个读事务并发，但是仅支持一个写事务
3. 应用程序限制，只支持小尺寸的事务，也是因为低并发导致的
4. NFS(网络文件系统)的两个问题，1. 网络文件系统locking logic有bug(unix 和windows都有) 2.因为网络延时可能导致性能下降，这个情况使用实现了客户端-服务器端模型的数据库比较好。
5. 数据库尺寸限制，因为可能导致内存开销过大。
6. 对象类型和数量限制
