# COOL语言链表实现

这是一个使用COOL（Classroom Object Oriented Language）实现的单向链表数据结构。该实现提供了基本的链表操作功能，包括插入、删除、查找和打印等操作。

## 功能特性

- 支持任意类型（Object）数据的存储
- 提供头部插入操作
- 支持按值删除节点
- 支持按值查找节点
- 提供链表判空操作
- 支持链表内容的格式化打印
- 支持多种数据类型的显示（Int、String、Object）

## 类结构

### Node 类
节点类，表示链表中的一个节点。
- `item`：存储节点数据
- `next`：指向下一个节点的引用
- 提供获取和设置节点数据的方法

### LinkedList 类
链表类，实现链表的核心功能。
- `head`：指向链表的第一个节点
- 提供完整的链表操作接口

## 主要方法

### LinkedList类方法
1. `init()`：初始化空链表
2. `insert(item: Object)`：在链表头部插入新节点
3. `delete(item: Object)`：删除第一个匹配的节点
4. `search(item: Object)`：查找指定值的节点
5. `isEmpty()`：检查链表是否为空
6. `print()`：打印链表内容

## 使用示例

```cool
let list : LinkedList <- new LinkedList in {
    (* 初始化链表 *)
    list <- list.init();
    
    (* 插入元素 *)
    list.insert("First");
    list.insert("Second");
    list.insert("Third");
    
    (* 打印链表 *)
    list.print();  (* 输出: Third -> Second -> First -> null *)
    
    (* 删除元素 *)
    list.delete("Second");
    
    (* 查找元素 *)
    if list.search("First") then
        out_string("Found!")
    else
        out_string("Not Found!")
    fi;
}
```

## 编译和运行

1. 编译COOL程序：
```bash
coolc homework.cl
```

2. 运行编译后的程序：
```bash
spim homework.s
```

## 注意事项

- 所有操作都会保持链表的结构完整性
- 删除和查找操作基于对象的相等性比较
- 打印方法会根据对象类型自动选择合适的显示格式