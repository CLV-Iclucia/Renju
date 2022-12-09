# Renju

## What is this & Some Thoughts 

这是C程序设计的五子棋大作业项目。

### 这个小程序的定位:

学Graphics的，对AI和算法的研究兴趣只能说很有限，花在AI上的时间有限，肯定卷不过大家的肝帝，大家不用慌。 

Graphics人的关注重点是如何写好的UI而不是写一个能吊锤所有人的AI（确信）

毕竟Graphics是不一定用得上AI的，但是图形系统早晚是得造的（

大家学AI的这么多，我何必抢你们饭碗呢（

以及真真正正写一个**能work的要素齐全的游戏**。

不过这一次强迫自己不用C++，自己用C从底层开始把轮子造好，也不是什么难事儿，就是没泛型有点蛋疼，然后性能跟STL容器和算法都肯定没法比就是了。

## How to build

目前只在Arch Linux和Windows下进行了简单的测试。

### Linux:

在项目根目录下使用如下命令:

```commandline
mkdir build
cd build
cmake ..
make
```

在build目录下找到Renju文件，运行即可

### Windows:

在项目根目录下使用如下命令:

```commandline
mkdir build
cd build
cmake .. -G "Unix Makefiles"
make
```

其中cmake命令与Linux下不同，避免CMake默认生成Visual Studio项目。

在build目录下找到Renju文件，运行即可。

#### 可能的问题:

如果发现界面全角字符显示错误，修改为UTF-8编码再运行。