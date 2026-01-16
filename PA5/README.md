# PA5: COOL语言代码生成器 (Code Generator)

本项目是斯坦福 CS143 编译原理课程 PA5 阶段（代码生成）的实现。以下步骤基于实际开发环境的操作流程，涵盖了编译、运行及与标准编译器结果对比的完整过程。

## 📂 修改的文件列表 (Modified Files)

本项目主要在以下文件中实现了代码生成的逻辑：

1.  **`cgen.cc`**: 核心代码生成逻辑，实现了 `CgenClassTable` 和 `code()` 方法。
2.  **`cgen.h`**: 增加了代码生成所需的成员变量和辅助函数声明。
3.  **`cool-tree.handcode.h`**: 引入了 `Environment` 类用于作用域和变量管理。
4.  **`cool-tree.h`**: 配合代码生成需求调整了部分 AST 节点定义。

---

## 🚀 编译步骤 (Build Steps)

在项目根目录下，按照以下顺序清理并重新编译项目：

```bash
# 1. 清理旧的编译文件
make clean

# 2. 编译基础组件
make

# 3. 编译代码生成器 (cgen)
make cgen
```

> **注意**: 编译 `cgen` 时可能会出现 `invalid suffix on literal` 的警告，这是由于 C++ 标准差异导致的，不影响生成结果。

---

## 🧪 测试与运行步骤 (Test & Run)

我们使用 `complex.cl` 作为测试用例，该文件涵盖了继承、多态、IO、循环等核心特性。

### 步骤 1：生成汇编代码 (Generate Assembly)

使用手动构建的编译管道，将 `complex.cl` 编译为 `complex_my.s`。这一步串联了词法分析(lexer)、语法分析(parser)、语义分析(semant)和我们生成的代码生成器(cgen)。

```bash
./lexer complex.cl | ./parser complex.cl 2>&1 | ./semant complex.cl 2>&1 | ./cgen -o complex_my.s complex.cl
```

*   **预期输出**: 终端应显示如下构建过程信息：
    ```text
    Building CgenClassTable...
    Installed basic classes.
    Installed user classes.
    Built inheritance tree.
    Assigning tags...
    Tags assigned. Max tag: 8
    Generating code...
    coding global data
    choosing gc
    ```

### 步骤 2：运行汇编代码 (Run with SPIM)

使用 MIPS 模拟器 `spim` 运行生成的汇编文件 `complex_my.s`：

```bash
/usr/class/bin/spim -file complex_my.s
```

*   **预期输出**:
    ```text
    SPIM Version 6.5 of January 4, 2003
    ...
    --- COOL Comprehensive Syntax Test Start ---

    1. Testing Class, Let, Dispatch, and Overriding...
    Shape<Circle> at (10, 20)
      - My dynamic type is: Circle
      - My static type (as Shape) is: Shape

    2. Testing While loop, If-Else, and Arithmetic...
      - Factorial of 5 is: 120
      - Factorial test PASSED.
    ...
    --- Test End ---
    COOL program successfully executed
    ```

---

## ✅ 验证正确性 (Validation)

为了确保代码生成的完全正确性，我们将自己的输出与官方标准编译器的输出进行对比 (`diff`)。

### 步骤 1：使用官方编译器生成基准

使用官方的 `coolc` 编译同一个文件，生成标准的 `complex.s`：

```bash
/usr/class/bin/coolc complex.cl
```

### 步骤 2：生成输出文件并对比

分别运行官方版本和我们的版本，将输出重定向到文本文件，然后使用 `diff` 命令比较。

```bash
# 1. 运行官方汇编代码，保存输出到 official.txt
/usr/class/bin/spim -file complex.s > official.txt 2>&1

# 2. 运行我们的汇编代码，保存输出到 my.txt
/usr/class/bin/spim -file complex_my.s > my.txt 2>&1

# 3. 对比两个输出文件
diff official.txt my.txt
```

*   **验证结果**: 如果 `diff` 命令**没有输出任何内容**，则说明你的实现与官方标准编译器完全一致，测试通过。
