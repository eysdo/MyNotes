# MongoDb 快速入门教程

## 简介

MongoDb 是一个介于关系数据库和非关系数据库之间的产品，是非关系数据库当中功能最丰富，最像关系数据库的。

它是可扩展的高性能数据存储解决方案，经常被用于非关系型数据的存储，能存储海量的数据。

常见的非关系型数据库还有：[Redis](https://redis.io/)，但 MongoDb 比 redis 更具有数据库的特性。

![img](images\595137-20170905225124538-596840123.png)



## MongoDb的基本数据结构

与 MySQL 等关系型数据库一样，MongoDb 也有类似的概念，但是其称呼不同。下表列出了 MongoDb 的基本数据结构以及与 MySQL 的对比。

| **SQL术语/概念** | **MongoDB术语/概念** |            **解释/说明**            |
| :--------------: | :------------------: | :---------------------------------: |
|     database     |       database       |               数据库                |
|      table       |      collection      |            数据库表/集合            |
|       row        |       document       |           数据记录行/文档           |
|      column      |        field         |             数据字段/域             |
|      index       |        index         |                索引                 |
|   table joins    |                      |        表连接,MongoDb不支持         |
|   primary key    |     primary key      | 主键,MongoDb自动将_id字段设置为主键 |

可以看到 MongoDb 与 SQL 的数据库概念都是一致的，而 MongoDb 中数据库表（Table）则称之为集合（Collection），行（Row）则称为文档（Document），列（Column）则称为字段（Field）。



## 安装MongoDb服务器

MongoDB 提供了 OSX 平台上 64 位的安装包，你可以在官网下载安装包。

下载地址：[MongoDb Download Center | MongoDB](https://www.mongodb.com/download-center#community)

![img](images\595137-20170905225138944-1625936615.png)

下载后解压文件并做好PATH路径配置：

```
# 解压文件
tar -zxvf mongodb-osx-x86_64-3.4.7.tgz
# 配置${mongodb_home}/bin到PATH路径
export PATH=<mongodb-install-directory>/bin:$PATH
```

创建数据库存储目录：

```
mkdir -p /data/db
```

创建完目录之后，直接运行`mongod`命令即可启动MongoDb服务器。`mongod`命令默认使用`/data/db`为 MongoDb 数据库的数据文件目录。如果需要改变数据文件存储目录，需要指定`--dbpath`参数，例如：

```
mongod --dbpath /Users/yurongchan/mongodb_data_file
```

类似的启动配置参数还有：

|       **参数**       |                           **描述**                           |
| :------------------: | :----------------------------------------------------------: |
|      --bind_ip       | 绑定服务IP，若绑定127.0.0.1，则只能本机访问，不指定默认本地所有IP |
|      --logpath       |          定MongoDB日志文件，注意是指定文件不是目录           |
|     --logappend      |                     使用追加的方式写日志                     |
|       --dbpath       |                        指定数据库路径                        |
|        --port        |                指定服务端口号，默认端口27017                 |
|    --serviceName     |                         指定服务名称                         |
| --serviceDisplayName |           指定服务名称，有多个mongodb服务时执行。            |
|      --install       |                指定作为一个Windows服务安装。                 |

运行`mongod`命令后控制台输出一系列初始化信息，包括使用的数据库文件存储目录等。如果没有什么异常信息，那就说明 MongoDb 服务器启动成功。

```
2017-08-27T15:23:39.680+0800 I CONTROL  [initandlisten] MongoDB starting : pid=27207 port=27017 dbpath=/data/db 64-bit host=YuRongChandeMacBook-Pro.local
2017-08-27T15:23:39.681+0800 I CONTROL  [initandlisten] db version v3.4.7
2017-08-27T15:23:39.681+0800 I CONTROL  [initandlisten] git version: cf38c1b8a0a8dca4a11737581beafef4fe120bcd
2017-08-27T15:23:39.681+0800 I CONTROL  [initandlisten] allocator: system
2017-08-27T15:23:39.681+0800 I CONTROL  [initandlisten] modules: none
2017-08-27T15:23:39.681+0800 I CONTROL  [initandlisten] build environment:
2017-08-27T15:23:39.681+0800 I CONTROL  [initandlisten]     distarch: x86_64
2017-08-27T15:23:39.681+0800 I CONTROL  [initandlisten]     target_arch: x86_64
2017-08-27T15:23:39.681+0800 I CONTROL  [initandlisten] options: {}
2017-08-27T15:23:39.682+0800 I STORAGE  [initandlisten] wiredtiger_open config: create,cache_size=3584M,session_max=20000,eviction=(threads_min=4,threads_max=4),config_base=false,statistics=(fast),log=(enabled=true,archive=true,path=journal,compressor=snappy),file_manager=(close_idle_time=100000),checkpoint=(wait=60,log_size=2GB),statistics_log=(wait=0),
```

**如果你的操作系统是Windows系统，你可以通过查阅这里获取安装的详细方法。**



## 连接MongoDb服务器

服务器启动之后，我们启动一个终端连接到 MongoDb 服务器，这样我们就可以运行进行数据库CURD操作。连接MongoDb服务器命令的语法如下：

```
mongo server_ip:port/dbname -u user -p password
```

我们这里直接连接本地服务器，因此直接运行`mongo`命令即可。此时控制台会打印出连接的 MongoDb 服务器地址以及端口等信息。

```
YuRongChandeMacBook-Pro:~ yurongchan$ mongo
MongoDB shell version v3.4.7
connecting to: mongodb://127.0.0.1:27017
MongoDB server version: 3.4.7
Welcome to the MongoDB shell.
For interactive help, type "help".
For more comprehensive documentation, see
    http://docs.mongodb.org/
Questions? Try the support group
    http://groups.google.com/group/mongodb-user
Server has startup warnings:
2017-08-27T15:23:40.570+0800 I CONTROL  [initandlisten]
2017-08-27T15:23:40.570+0800 I CONTROL  [initandlisten] ** WARNING: Access control is not enabled for the database.
2017-08-27T15:23:40.570+0800 I CONTROL  [initandlisten] **          Read and write access to data and configuration is unrestricted.
2017-08-27T15:23:40.570+0800 I CONTROL  [initandlisten] ** WARNING: You are running this process as the root user, which is not recommended.
2017-08-27T15:23:40.570+0800 I CONTROL  [initandlisten]
2017-08-27T15:23:40.570+0800 I CONTROL  [initandlisten]
2017-08-27T15:23:40.570+0800 I CONTROL  [initandlisten] ** WARNING: soft rlimits too low. Number of files is 256, should be at least 1000
```

而 MongoDb 服务器控制台也会输出相关信息，提示有客户端连接到本服务器上。

```
2017-09-03T15:05:52.799+0800 I NETWORK  [thread1] connection accepted from 127.0.0.1:55283 #1 (1 connection now open)
2017-09-03T15:05:52.804+0800 I NETWORK  [conn1] received client metadata from 127.0.0.1:55283 conn1: { application: { name: "MongoDB Shell" }, driver: { name: "MongoDB Internal Client", version: "3.4.7" }, os: { type: "Darwin", name: "Mac OS X", architecture: "x86_64", version: "14.1.0" } }
```



## MongoDb创建数据库

MongoDb 创建和切换数据库的语法格式为：

```
use database_name
```

如果数据库不存在，则创建数据库，否则切换到指定数据库。

```
> use chenyurong
switched to db chenyurong
> db
chenyurong
> show dbs
admin  0.000GB
local  0.000GB
```

上面创建了名为chenyurong的数据库，但是我们使用`show dbs`命令时并没有看到该数据库存在，这是因为该数据库中还没有数据。要显示它，我们需要向 chenyurong 数据库插入一些数据。关于插入数据操作，将在下面提及。



## MongoDb创建表

MongoDb 中并没有直接创建表的命令，表的数据结构在你往表插入数据时确定。因此在 MongoDb 中，你创建完数据库之后就可以直接往表中插入数据，表名在插入数据时指定。



## MongoDb插入数据

MongoDB 使用 insert() 或 save() 方法向集合中插入文档，语法如下：

```
db.collection.insert(document)
```

下面我们向`user`表插入一条数据：

```
> db.user.insert({
...     "name": "chenyurong",
...     "age": 25,
...     "addr": "ShenZhen"
... })
WriteResult({ "nInserted" : 1 })
> db.user.find()
{ "_id" : ObjectId("59a2782f6eb4c099dbb718a1"), "name" : "chenyurong", "age" : 25, "addr" : "ShenZhen" }
> show dbs
admin       0.000GB
chenyurong  0.000GB
local       0.000GB
```

可以看到我们成功向`user`表插入了一条数据，此时我们使用`show dbs`也能看到对应chenyurong数据库。

上面使用`db.user.find()`查询的结果没有格式化，不便于阅读。我们可以使用`pretty()`函数使其输出格式化。

```
> db.user.find().pretty()
{
    "_id" : ObjectId("59abb034dca9453471d67f13"),
    "name" : "chenyurong",
    "age" : 25,
    "addr" : "ShenZhen"
}
```



## MongoDb查询数据

为了方便演示接下来的查询功能，我们这里运行以下命令清空数据库表user，并向其中插入一些测试数据。

```
//清空当前数据库下user表的所有数据
> db.user.remove({})
WriteResult({ "nRemoved" : 2 })
//查看user表的所有数据
> db.user.find()
//批量插入数据
> db.user.insert([
... {
...     "name": "ChenYuRong",
...     "age": 25,
...     "addr": "ShenZhen"
... },
... {
...     "name": "LiQiLiang",
...     "age": 27,
...     "addr": "GuangZhou"
... },
... {
...     "name": "XiaoPai",
...     "age": 30,
...     "addr": "BeiJing"
... },
... {
...     "name": "YuChangHui",
...     "age": 32,
...     "addr": "FuJian"
... },
... {
...     "name": "XiaoHei",
...     "age": 28,
...     "addr": "ShenZhen"
... },
... {
...     "name": "AnQi",
...     "age": 20,
...     "addr": "JieYang"
... }]
... )
BulkWriteResult({
    "writeErrors" : [ ],
    "writeConcernErrors" : [ ],
    "nInserted" : 6,
    "nUpserted" : 0,
    "nMatched" : 0,
    "nModified" : 0,
    "nRemoved" : 0,
    "upserted" : [ ]
})
```

此时 user 表中的数据列表如下：

|  **name**  | **age** | **addr**  |
| :--------: | :-----: | :-------: |
| ChenYuRong |   25    | ShenZhen  |
| LiQiLiang  |   27    | GuangZhou |
|  XiaoPai   |   30    |  BeiJing  |
| YuChangHui |   32    |  FuJian   |
|  XiaoHei   |   28    | ShenZhen  |
|    AnQi    |   20    |  JieYang  |

MongoDB 查询数据的语法格式如下：

```
db.collection.find(query, projection)
```

- **query（可选）**：使用查询操作符指定查询条件。该参数是一个JSON对象，key 一般为查询的列名，value 为查询匹配的值。
- **projection（可选）**：使用投影操作符指定返回的键。如果省略该参数，那么查询时返回文档中所有键值。该参数是一个JSON对象，key 为需要显示的列名，value 为 1（显示） 或 0（不显示）。

下面的查询语句将user表中地址（addr）为ShenZhen，年龄（age）为25的数据筛选出来，并且在结果中不显示ID列：

```
db.user.find({"addr":"ShenZhen","age":25},{"_id":0}).pretty()
```

查询结果为：

```
{ "name" : "ChenYuRong", "age" : 25, "addr" : "ShenZhen" }
```



### 范围操作符

范围操作符指的是：大于、大于等于、等于、不等于、小于、小于等于操作符，在 MongoDb 中它们的表示以及使用如下面表格所示：

|  **操作**  |  **格式**  |                 **范例**                  | **RDBMS中的类似语句** |
| :--------: | :--------: | :---------------------------------------: | :-------------------: |
|    等于    |    {:}     |  db.col.find({"by":"MongoDb"}).pretty()   | where by = 'MongoDb'  |
|    小于    | {:{$lt:}}  | db.col.find({"likes":{$lt:50}}).pretty()  |   where likes < 50    |
| 小于或等于 | {:{$lte:}} | db.col.find({"likes":{$lte:50}}).pretty() |   where likes <= 50   |
|    大于    | {:{$gt:}}  | db.col.find({"likes":{$gt:50}}).pretty()  |   where likes > 50    |
| 大于或等于 | {:{$gte:}} | db.col.find({"likes":{$gte:50}}).pretty() |   where likes >= 50   |
|   不等于   | {:{$ne:}}  | db.col.find({"likes":{$ne:50}}).pretty()  |   where likes != 50   |

例如我要查询用户表中所有年龄大于等于25岁的用户，那么查询语句为：

```
db.user.find({"age": {$gte:25}},{"_id":0}).pretty()
```

查询结果为：

```
{ "name" : "ChenYuRong", "age" : 25, "addr" : "ShenZhen" }
{ "name" : "LiQiLiang", "age" : 27, "addr" : "GuangZhou" }
{ "name" : "XiaoPai", "age" : 30, "addr" : "BeiJing" }
{ "name" : "YuChangHui", "age" : 32, "addr" : "FuJian" }
{ "name" : "XiaoHei", "age" : 28, "addr" : "ShenZhen" }
```



### AND操作符

MongoDB 的 find() 方法可以传入多个键（key），每个键（key）以逗号隔开。每个键（key）之间是与的逻辑关系。

例如我要查询用户表（user）中地址为ShenZhen且年龄大于等于25岁的用户，那么查询语句为：

```
db.user.find({"addr": "ShenZhen","age": {$gte:25}},{"_id":0}).pretty()
```

查询结果为：

```
{ "name" : "ChenYuRong", "age" : 25, "addr" : "ShenZhen" }
{ "name" : "XiaoHei", "age" : 28, "addr" : "ShenZhen" }
```



### OR操作符

MongoDB 中关键字`$or`表示或逻辑关系，其语法格式如下：

```
db.col.find(
   {
      $or: [
         {key1: value1}, {key2:value2}
      ]
   }
).pretty()
```

例如我要查询用户表（user）中地址为ShenZhen或者年龄大于等于30岁的用户，那么查询语句为：

```
db.user.find({$or:[{"addr":"ShenZhen"},{"age":{$gte:30}}]}).pretty()
```

查询结果为：

```
{
    "_id" : ObjectId("59ac1c57dca9453471d67f15"),
    "name" : "ChenYuRong",
    "age" : 25,
    "addr" : "ShenZhen"
}
{
    "_id" : ObjectId("59ac1c57dca9453471d67f17"),
    "name" : "XiaoPai",
    "age" : 30,
    "addr" : "BeiJing"
}
{
    "_id" : ObjectId("59ac1c57dca9453471d67f18"),
    "name" : "YuChangHui",
    "age" : 32,
    "addr" : "FuJian"
}
{
    "_id" : ObjectId("59ac1c57dca9453471d67f19"),
    "name" : "XiaoHei",
    "age" : 28,
    "addr" : "ShenZhen"
}
```

AND操作符和OR操作符可以混合使用，例如要实现以下SQL查询：

```
select * from user
where name = "ChenYuRong" or (age <= 25 and addr == "JieYang")
```

那么该 MongoDb 查询语句应该这样写：

```
db.user.find({$or:[{"name":"ChenYuRong"}, {"age": {$lte:25}, "addr": "JieYang"}]}).pretty()
```

查询结果为：

```
{
    "_id" : ObjectId("59ac1c57dca9453471d67f15"),
    "name" : "ChenYuRong",
    "age" : 25,
    "addr" : "ShenZhen"
}
{
    "_id" : ObjectId("59ac1c57dca9453471d67f1a"),
    "name" : "AnQi",
    "age" : 20,
    "addr" : "JieYang"
}
```



### 排序

在 MongoDB 中使用使用 sort() 方法对数据进行排序，sort() 方法可以通过参数指定排序的字段，并使用 1 和 -1 来指定排序的方式，其中 1 为升序排列，而-1是用于降序排列。

sort()方法基本语法如下所示：

```
db.collection.find().sort({KEY:1})
```

其中`KEY`表示要进行排序的字段。

例如我们将所有年龄小于30岁的用户查询出来并将其按照年龄升序排列：

```
db.user.find({"age":{$lt:30}}).sort({age:1}).pretty()
```

查询结果为：

```
{
    "_id" : ObjectId("59ac1c57dca9453471d67f1a"),
    "name" : "AnQi",
    "age" : 20,
    "addr" : "JieYang"
}
{
    "_id" : ObjectId("59ac1c57dca9453471d67f15"),
    "name" : "ChenYuRong",
    "age" : 25,
    "addr" : "ShenZhen"
}
{
    "_id" : ObjectId("59ac1c57dca9453471d67f16"),
    "name" : "LiQiLiang",
    "age" : 27,
    "addr" : "GuangZhou"
}
{
    "_id" : ObjectId("59ac1c57dca9453471d67f19"),
    "name" : "XiaoHei",
    "age" : 28,
    "addr" : "ShenZhen"
}
```



### 聚合

MongoDB中聚合的方法使用`aggregate()`，其基本的语法格式如下：

```
db.collection.aggregate(AGGREGATE_OPERATION)
```

其中`AGGREGATE_OPERATION`的格式为：

```
[
    {
        $group: {
            _id: {
                addr: '$addr'
            },
            totalCount: {
                $sum: 1
            }
        }
    }
]
```

- `$group`是固定的，表示这里一个分组聚合操作。
- `_id`表示需要根据哪一些列进行聚合，其实一个JSON对象，其key/value对分别表示结果列的别名以及需要聚合的的数据库列。
- `totaoCount`表示聚合列的列名。
- `$sum`表示要进行的聚合操作，后面的1表示每次加1。

例如要根据地区统计用户人数，那么查询语句为：

```
db.user.aggregate([{$group:{_id:{userAddr:'$addr'},totalCount:{$sum:1}}}])
```

查询结果为：

```
{ "_id" : { "userAddr" : "FuJian" }, "totalCount" : 1 }
{ "_id" : { "userAddr" : "JieYang" }, "totalCount" : 1 }
{ "_id" : { "userAddr" : "BeiJing" }, "totalCount" : 1 }
{ "_id" : { "userAddr" : "GuangZhou" }, "totalCount" : 1 }
{ "_id" : { "userAddr" : "ShenZhen" }, "totalCount" : 2 }
```



## MongoDb更新数据

update() 方法用于更新已存在的文档。语法格式如下：

```
db.collection.update(
   <query>,
   <update>,
   {
     upsert: <boolean>,
     multi: <boolean>,
     writeConcern: <document>
   }
)
```

- **query**：对哪些数据进行更新操作。
- **update**：对这些数据做什么操作。
- **upsert（可选）**：如果不存在update的记录，是否将其作为记录插入。true为插入，默认是false，不插入。
- **multi（可选）**：是否更新多条记录。MongoDb 默认是false，只更新找到的第一条记录。如果这个参数为true,就把按条件查出来多条记录全部更新。
- **writeConcern（可选）**：表示抛出异常的级别。

例如我们更新user表名为chenyurong的记录，将其年龄更改为25岁。

```
db.user.update({'name':'chenyurong'},{$set:{'age':25}})
```

其中`$set`表示进行赋值操作。



## MongoDb删除数据

MongoDB中聚合的方法使用`remove()`，其基本的语法格式如下：

```
db.collection.remove(
   <query>,
   {
     justOne: <boolean>,
     writeConcern: <document>
   }
)
```

- **query（可选）：**删除的文档的条件。
- **justOne（可选）：**如果设为 true 或 1，则只删除一个文档。
- **writeConcern（可选）：**抛出异常的级别。

例如我们想删除名字（name）为LiQiLiang的用户，那么该删除语句为：

```
> db.user.remove({"name":"LiQiLiang"})
WriteResult({ "nRemoved" : 1 })
> db.user.find({"name":"LiQiLiang"}).pretty()
> 
```

如果你想删除所有数据，可以使用以下方式（类似常规 SQL 的 truncate 命令）：

```
>db.col.remove({})
>db.col.find()
>
```

## 常用的DDL命令

- 查看当前数据库：`db`
- 查看所有数据库：`show dbs`
- 查看当前数据库所有集合（表格）：`show collections`

## MongoDb图形化工具

如果你是企业版的用户，可以尝试使用：[企业版用户：MongoDb Compass](https://www.mongodb.com/products/compass)。

如果你跟我一样是个人用户，而且也一样使用 JetBrain 编辑器，那么你可以试试 JetBrain 的一款插件：[JetBrain Plugin：mongo4idea](https://github.com/dboissier/mongo4idea)。

另外的一款 GUI 图形化工具可以作为备用工具使用：[NoSQL for MongoDb（Windows使用）](https://www.mongodbmanager.com/)。

## 总结

这次我们从服务器的安装到连接，再介绍了如 MongoDb 最基本的 CURD 操作。

**看到这里是不是觉得 MongoDb 还是蛮简单的呢，来动手做做几道题看是否真的完全掌握了吧。**

- 查询出user表中用户年龄小于25岁的用户，并按照年龄升序排列。
- 查询出user表中用户年龄大于25岁并且地址在深圳的用户。
- 查询出user表中用户年龄大于25岁并且地址在深圳，或者地址在北京的用户。
- 统计出user表中各个城市的用户人数。

## 参考资料

- [MongoDB Tutorials — MongoDB Manual 3.4](https://docs.mongodb.com/manual/tutorial/)