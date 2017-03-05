# tree
在Windows下实现tree命令

使用方法

下载tree.c
## gcc编译
```
gcc tree.c -o tree
```

## vs2015
直接编译运行会出错，为了排错，请这样做：

C/C++ --> 预处理器 --> 预处理器定义 --> 编辑 -->`_CRT_SECURE_NO_DEPRECATE`
