# Learning Lab1

## 步骤1



## 步骤2

### 问题：我需要修改/新增Nachos哪一部分的代码？我不该随意改动哪些代码？（1.7节）

在一些情况下，您将扩展或“填充”已经定义的C++类或方法。很少有必要删除或重写已经存在的代码（这在实验4中确实发生过），或者在每个作业的重点之外的区域添加代码。

此外，在任何情况下，您都不应修改第2.4节所述的machine子目录中的machine simulation software定义的“硬件”行为

### 问题：code文件夹下的子目录都放了啥？输入`make depend` ，`make`和 `./nachos` 都发生了什么？（2.1节）

#### 1. 都放了啥

Nachos代码目录包括多个子目录，其中包含Nachos系统不同部分的源代码。子目录包括makefile，允许您使用make命令自动为特定的赋值构建正确的组件（确保您使用的是GNU make）。

相关的子目录是：

- threads for Labs 1-3
- userprog for Labs 4-5
- vm for Lab 6

#### 2. `make` 和 `./nachos`

如果在其中一个目录中键入make，它将执行一系列命令来编译和链接Nachos，从而在该目录中生成一个名为Nachos的可执行程序。所有的测试都将通过运行这些nachos可执行文件来完成，这些文件是由您修改的nachos代码构建的。

#### 3. `make depend`

依赖关系信息确定在给定的 `.h` 文件更改时重建哪些 `.cc` 文件。每个子目录的 `Makefile`（例如`nachos/threads/Makefile`）中的依赖关系行是使用makedepend工具自动创建的。例如，如果键入 `cd threads；make depend`，这将在 `threads/Makefile` 中重新生成依赖关系信息。保持依赖信息的最新是非常重要的。

一些简单的指导原则将帮助您避免构建问题，这些问题可能会花费数小时令人沮丧的调试时间来跟踪实际上并不存在的bug。首先，务必确保运行make dependency time header文件依赖项更改（例如，您引入了一个新的.cc文件或在现有.cc文件中包含新的头文件），或者在源代码位置发生任何更改时（例如，您可以将源树从一个目录更改为另一个目录）。第二，始终确保您运行的是nachos可执行文件的正确副本，可能是您刚刚构建的一个。如果有疑问，请将目录更改为正确的目录，并使用./Nachos执行Nachos。

### 如何 Debug `Nachos` 代码？（2.2节）

- `printf` 可能不同步；
- 想要 print 调试，需要使用 `threads/utility.h` 中的函数

### 如何在Nachos进行context switch？（2.3节）

当显式运行的线程调用 `thread::Yield` 或其他一些例程以使调度程序切换到另一个线程时，会发生自愿上下文切换。

注意，线程必须在Nachos内核中运行，才能进行自愿的上下文切换。

## 步骤3

### 阅读 `nachos-3.4/code/`目录下的 `Makefile`, `Makefile.dep`,`Makefile.common`，以及 `nachos-3.4/code/threads/Makefile`，初步了解Makefile的构成和相互关系