# 实验 1 带键盘事件的计算器

郭毅川 2024414290411 24软件工程4班

## 构建运行

使用 Qt Creator 打开 `src/calculator.pro`。界面文件可在 Designer 中编辑。

```sh
mkdir -p src/build
cd src/build
qmake ../calculator.pro
make -j2
./calculator
```

实际验证环境：Linux、Qt 5.15.3、C++17。
测试在 `src/tests`，日志和截图在 `evidence`。
