这里用于记录一些MySQL的使用知识

mysql是一种关系型数据库，关系型数据库顾名思义，就是数据之间是有关系相连，创建一个数据库，库内可包含多个关系表，数据就是以一张张表的形式存储

### mysql的注册
我是使用docker的容器运行的mysql：8,这里讲述下过程
1. 先使用docker pull拉取要使用的mysql的版本
2. 然后docker run，注意这里，如果是第一次启动要完成以下配置

    docker run -d \
    --name mysql8 \
    -e MYSQL_ROOT_PASSWORD=123456 \
    -p 3306:3306 \
    -v ~/mysql-data:/var/lib/mysql \
    mysql:8

3. 此时可以docker ps查看当前运行的容器，确保启动成功
4. 使用docker exec -it name bash 进入容器
5. 此时使用命令mysql -u root -p，输入密码登陆mysql服务器，就算正式进入mysql了

### mysql中的数据类型
+ 数值类型
1. 整数类型
tinyint 1字节  
smallint 2字节  
mediumint 3字节  
int 4字节  
bigint 8字节  
以上所用都可以使用unsigned修饰，表示无符号数  

2. 小数类型
float(M,D) 单精度浮点数  
double(M,D) 双精度浮点数  
decimal(M,D) 精确值  
M表示总位数，D表示小数位数 

+ 字符串类型
char(n) 定长字符串，最大 255 字节，空白会被填充  
varchar 可变长度字符串，最大 65535 字节（受字符集影响） 
tinytext 最大 255 字节的文本  
text 最大 65,535 字节的文本  
mediumtext 最大 16,777,215 字节  
longtext 最大 4,294,967,295 字节  
binary(n) 定长二进制数据（类似 char）
varbinary(n) 可变长度二进制数据（类似 varchar）

+ 日期时间类型
date 日期，格式为YYYY-MM-DD  
time 时间，格式为HH:MM:SS  
datetime 日期和时间，格式为YYYY-MM-DD HH:MM:SS
timestamp 时间戳，默认当前时间
year 年份，格式为YYYY

### 常用命令
接下来讲解常用指令，mysql使用的是sql语言，所以需要重新学习，这里注意sql语言是不分大小写的，也就是说大小写均可，但为了更好的区分，对于固定的指令，参数会使用大写来区分，由于作者是新手，就先一律使用小写便于记忆
>sql指令均以;结尾

+ show databases;
展示当前的数据库，初始化是这样的  
+--------------------+  
| Database           |  
+--------------------+  
| information_schema |  
| mysql              |  
| performance_schema |  
| sys                |  
+--------------------+  

+ create database name;
创建一个新的数据库，可以使用上面的命令查看

+ drop database name;
删除一个数据库，可以使用上面的命令查看

+ use name；
name是数据库的名字，将该数据库绑定为当前使用的数据库，便于后面的创建表

+ create table name (具体参数名称 类型，...);
在当前使用的数据库中创建一张表，名字为name,内部包含具体的参数

+ desc name;
查看表的具体内容  
+-------+---------------+------+-----+---------+-------+  
| Field | Type          | Null | Key | Default | Extra |  
+-------+---------------+------+-----+---------+-------+  
| id    | int           | YES  |     | NULL    |       |  
| name  | varchar(100)  | YES  |     | NULL    |       |  
| level | int           | YES  |     | NULL    |       |  
| gold  | decimal(10,2) | YES  |     | NULL    |       |  
+-------+---------------+------+-----+---------+-------+  

+ alter table name modify colunm 原参数名称 要修改的类型；
修改表参数类型

+ alter table name add column 原参数名称 to 新参数名称；
修改参数名称

+ alter table name add column 新参数名称 类型;
添加新参数到表中

+ alter table name drop column 要删除的参数名称；
从表中删除参数

+ drop table name;
从数据库中删除表

+ insert into name (要填写的参数) valus (具体的参数)；
添加一个表的具体用例，注意参数一一对应

+ select - from name;
查看表的用例，-代表要查看的参数，当-为*时，列出表的所用用例的全部参数
select id from player;  
+----+  
| id |  
+----+  
|  1 |  
+----+  
select * from player;  
+----+------+-------+--------+  
| id | name | level | gold   |  
+----+------+-------+--------+  
|  1 | haha |    10 | 100.00 |  
+----+------+-------+--------+  

+ insert table name modify 参数名称 参数类型 default 具体的参数;
设置一个参数的默认初始化值，当创建一个具体用例但该参数未设置时，就会设为默认值

+ update name set 具体参数 = 要设置的数 where 具体的用例
修改具体用例的参数，同时where后面的可以不加，表示把所有的用例的具体参数都修改

+ delete from name where 具体的用例
与update类似，是删除某一具体用例，当不加where后面的筛选条件时，删除所有的具体用例

+ select - from name where 参数条件 筛选
1. -同上，筛选这里可以使用 > < and or(优先级and>or) between+and not,这里也有优先级，也可以使用()来设置优先级
2. 同时也可以使用like实现模糊查询，这里主要用到%和\_，%代表任意个字符，\_代表任意一个字符，与想要查询的字符组合在一起就可以实现模糊查询，例如'1%',就表示所有以1开头的参数，'
%1\_'就表示所有倒数第二个为1的参数
3. 也可以使用正则表达式regexp实现查询，以下是正则表达式的语法  
. 表示匹配任意单个字符  
^ 匹配行首  
$ 匹配行尾  
A|B A或者B  
[...] 匹配括号中任意一个字符    
[^...] 匹配非括号中的任意字符   
\* 匹配前一个字符重复0次或多次   
\+ 匹配前一个字符重复1次或多次
{n} 精确匹配前一个字符n次  
4. 注意想要查找某个值为NULL的情况下，使用is null进行判断，这里注意null与' '的区别       
5. 想要输出的结果排好序，可以使用order by 具体的条件，这样就会按照顺序升序排列，也可以加上desc,实现降序排列，同时还可以加上其他的条件，在已排好的顺序上继续排序     

+ select 聚合函数 from name；
常用的聚合函数有
avg() 返回集合的平均值
count() 返回集合中的项目数
max() 返回最大值
min() 返回最小值
sum() 求和

+ 

