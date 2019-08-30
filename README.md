<!--
 * @Copyright: CS of BIT
 * @Author: 王占坤
 * @File name: 
 * @Version: 
 * @Date: 2019-08-30 15:14:54 +0800
 * @LastEditTime: 2019-08-30 15:21:54 +0800
 * @LastEditors: 
 * @Description: 
 -->
# chat_application

## 代码文件结构
1. 每一个分支仅仅允许有以下文件结构。实现文件统一放在src目录下，头文件统一放在include目录下。
2. 在include目录下必须有head.h头文件，在里面include所有需要的库文件，不允许include自己创建的头文件。
3. 在编写过程中，慎用extern关键字，严格注意thread的生命周期。
```
.
├── build
├── CMakeLists.txt
├── include
├── README.md
└── src
```

## debug与运行
由于某些位置原因```crtl+shift+b```不能够调用自定义的task进行编译，因此将debug功能调整成了debug和build两个功能。
<br/>
直接按```F5```或者用鼠标点击vscode上方菜单栏的调试。在侧边栏可以选择调试的模式，一个叫debug，一个叫build，build就是只编译运行，debug就是debug模式。

## cmake说明
第一次使用前需要安装
```
sudo apt update
sudo apt install cmake make
```
cmake具体使用：
```
cd build
cmake ..
make
./chat_application
```
如果出现错误，无论是编译还是运行，只要出现错误，必须将build目录下的所有文件删除。
更改完错误再次编译运行。

如果出现链接库等的错误找王占坤处理。


## 代码规范
参阅：
* [代码整洁之道（一）：命名篇](https://www.cnblogs.com/harrypotterjackson/p/11431812.html)
* [代码整洁之道（二）：函数篇](https://www.cnblogs.com/harrypotterjackson/p/11431816.html)

### 命名规范
关于命名的一些细节描述可以见上述网址。
<br/>
- 命名必须要体现其含义
- 命名要尽量详细
- 命名必须剔除废话

#### 文件命名

文件名前缀表示改文件的内容或作用。一般以模块或者功能命名，采用名词短语命名，统一使用驼峰命名法，第一个单词的首字母大写。

#### 函数命名

函数命名采用动宾结构，表示某个动作。采用驼峰命名法。第一个单词的首字母小写。一般而言第一个单词必须是动词。

函数命名应当包括作用域，命名组成顺序：

作用域\_函数名

其中作用域必须小写，与函数名以下划线相连。

#### 变量命名

变量命名采用名词短语结构。采用下划线命名法，全部单词小写。

变量命名组成顺序：

含义\_模块名\_作用域\_数据类型

#### typedef命名

除统一商量外，一律采用全大写。

### 作用域前缀

| 标识符类型               | 作用域前缀 |
| ------------------------ | ---------- |
| Global Variable          | w          |
| File Static Variale      | n          |
| Function Static Variable | fn         |
| Auto Variable            | a          |
| Global Function          | w          |
| Static Function          | n          |

普通变量不加作用域。

### 函数规范

- 短小，尽可能短小，控制在八十行内。一个函数仅仅实现一个功能
- 命名规范，体现函数动作含义

### 注释

- 必须加注释
- 注释尽可能详尽
- 代码即注释，使得注释尽可能少。良好的命名规范和文件结构编排

### 文件结构

每个较大的文件都必须有一个能够体现流程的主调函数。如果主调函数还是比较长就进一步再增加一级，使得一个文件的函数调用与排布具有典型的流水线形式。多级的文件结构、多级的代码结构可以使得代码逐渐抽象，更容易使调用者清晰明了。

## 头文件
所有头文件的生成格式都要如下，不要弄乱顺序。在```ifndef __HEAD_H__```这里应当注意，前后两个下划线，
中间是一个下划线。
<br/>
所有的配置都放在config.h中
<br/>
格式如下：如创建head.h文件：
```
/*
 * @Copyright: CS of BIT
 * @Author: 王占坤
 * @File name: 
 * @Version: 
 * @Date: 2019-08-30 15:08:30 +0800
 * @LastEditTime: 2019-08-30 15:11:53 +0800
 * @LastEditors: 
 * @Description: 这里是主头文件，包含了所有的库文件，全局变量声明，结构体声明等
 */
#ifndef __HEAD_H__
#define __HEAD_H__
///////////////include库文件区/////////////
#include <stdio.h>
#include <gtk/gtk.h>
#include <pthread.h>
/////////////define区////////////////////

//////////////typedef区/////////////////
typedef int socketfd;

/////////////全局结构体定义区//////////

///////////////全局变量声明区/////////////

#endif
```

