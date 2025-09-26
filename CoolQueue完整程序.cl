(* 队列(Queue)数据结构完整实现 *)
(* ========== 1. A2I类：提供整数和字符串转换功能 ========== *)
class A2I {
    (* 从字符串转换为整数 *)
    a2i(s: String): Int {
        if s.length() = 0 then 0 else
        if s.substr(0,1) = "-" then ~a2i_aux(s.substr(1,s.length()-1), 0) else
        a2i_aux(s, 0) fi fi
    };

    a2i_aux(s: String, n: Int): Int {
        if s.length() = 0 then n else
        a2i_aux(s.substr(1,s.length()-1), 
               n * 10 + (s.substr(0,1)).ord() - 48)
        fi
    };

    (* 从整数转换为字符串 *)
    i2a(i: Int): String {
        if i = 0 then "0" else
        if i < 0 then "-" ^ i2a_aux(~i, "") else
        i2a_aux(i, "") fi fi
    };

    i2a_aux(i: Int, s: String): String {
        if i = 0 then s else
        i2a_aux(i / 10, 
               (let c: String <- "a" in
                c.substr(0,1).ord()-("a".substr(0,1).ord()-i//10) 
               ).chr() ^ s)
        fi
    };
};


(* ========== 2. QueueNode类：队列节点，存储单个元素和下一节点引用 ========== *)
class QueueNode inherits Object {
    item: Object;    (* 节点存储的数据，Object类型支持任意数据 *)
    next: QueueNode; (* 指向下一节点的指针，空节点为void *)

    (* 初始化节点：绑定数据item和下一节点next，返回当前节点 *)
    init(i: Object, n: QueueNode): QueueNode {
        {
            item <- i;
            next <- n;
            self;
        }
    };
};


(* ========== 3. Queue类：队列核心操作，管理队首、队尾指针及元素个数 ========== *)
class Queue inherits Object {
    front: QueueNode; (* 队首指针（最早入队元素） *)
    rear: QueueNode;  (* 队尾指针（最新入队元素） *)
    size: Int;        (* 队列元素个数，用于快速判断空队列 *)

    (* 初始化空队列：front、rear设为void，size设为0 *)
    init(): Queue {
        {
            front <- void;
            rear <- void;
            size <- 0;
            self;
        }
    };

    (* 入队操作：将元素item加入队尾 *)
    enqueue(item: Object): Object {
        let new_node: QueueNode <- (new QueueNode).init(item, void) (* 创建新节点 *)
        in {
            if isvoid front then  (* 空队列：新节点既是队首也是队尾 *)
                {
                    front <- new_node;
                    rear <- new_node;
                }
            else  (* 非空队列：新节点接在队尾，更新rear *)
                {
                    rear.next <- new_node;
                    rear <- new_node;
                }
            fi;
            size <- size + 1;  (* 队列长度+1 *)
            item;  (* 返回入队元素，便于跟踪 *)
        }
    };

    (* 出队操作：移除并返回队首元素，空队列时返回void *)
    dequeue(): Object {
        if isEmpty() then
            void  (* 空队列返回void *)
        else
            let dequeued_item: Object <- front.item,  (* 保存待出队元素 *)
                new_front: QueueNode <- front.next    (* 新队首为原队首的下一节点 *)
            in {
                front <- new_front;  (* 更新队首指针 *)
                size <- size - 1;    (* 队列长度-1 *)
                if isvoid front then  (* 出队后为空，同步更新rear *)
                    rear <- void
                fi;
                dequeued_item;  (* 返回出队元素 *)
            }
        fi
    };

    (* 查看队首元素：仅返回数据，不移除节点，空队列返回void *)
    front(): Object {
        if isEmpty() then
            void  (* 空队列返回void *)
        else
            front.item
        fi
    };

    (* 判断队列是否为空：size为0即空 *)
    isEmpty(): Bool {
        size = 0
    };

    (* 打印队列：从队首到队尾格式化输出，支持Int和String类型 *)
    print(): Object {
        let io: IO <- new IO,
            current: QueueNode <- front  (* 从队首开始遍历 *)
        in {
            io.out_string("[");  (* 输出队列起始符 *)
            while not isvoid current loop  (* 遍历所有节点 *)
                {
                    (* 区分元素类型，Int转字符串，String直接输出 *)
                    if current.item.isInt() then
                        {
                            let a2i: A2I <- new A2I;
                            io.out_string(a2i.i2a(current.item));
                        }
                    else if current.item.isString() then
                        io.out_string(current.item)
                    else
                        io.out_string("Unknown")  (* 其他类型显示占位符 *)
                    fi fi;
                    (* 非最后一个节点，输出分隔符 *)
                    if not isvoid current.next then
                        io.out_string(", ")
                    fi;
                    current <- current.next;  (* 移动到下一节点 *)
                }
            pool;
            io.out_string("]\n");  (* 输出队列结束符和换行 *)
            self;  (* 支持链式调用，如queue.enqueue(1).print() *)
        }
    };
};


(* ========== 4. Main类：测试队列所有功能，程序入口 ========== *)
class Main inherits A2I {
    main(): Object {
        let io: IO <- new IO,
            queue: Queue <- (new Queue).init()  (* 初始化空队列 *)
        in {
            io.out_string("=== COOL语言队列实现测试 ===\n\n");

            (* 测试1：空队列判断 *)
            io.out_string("1. 空队列检查:\n");
            io.out_string("队列为空吗？ ");
            if queue.isEmpty() then
                io.out_string("是\n")
            else
                io.out_string("否\n")
            fi;

            (* 测试2：查看空队列的队首元素 *)
            io.out_string("2. 查看空队首元素:\n");
            if isvoid queue.front() then
                io.out_string("队列为空，无队首元素\n")
            else
                io.out_string("队首元素: ");
                if queue.front().isString() then
                    io.out_string(queue.front())
                else
                    io.out_string(i2a(queue.front()))
                fi;
                io.out_string("\n")
            fi;

            (* 测试3：入队5个元素（混合String和Int类型） *)
            io.out_string("\n3. 入队测试:\n");
            queue.enqueue("apple");
            queue.enqueue(123);
            queue.enqueue("banana");
            queue.enqueue(456);
            queue.enqueue("orange");
            io.out_string("入队5个元素后: ");
            queue.print();  (* 预期输出：[apple, 123, banana, 456, orange] *)

            (* 测试4：查看队首元素 *)
            io.out_string("\n4. 查看队首元素:\n");
            io.out_string("队首元素: ");
            if not isvoid queue.front() then
                if queue.front().isString() then
                    io.out_string(queue.front())
                else
                    io.out_string(i2a(queue.front()))
                fi;
                io.out_string("\n")
            else
                io.out_string("队列为空\n")
            fi;

            (* 测试5：出队3个元素 *)
            io.out_string("\n5. 出队测试:\n");
            io.out_string("出队元素1: ");
            let d1: Object <- queue.dequeue() in
                if not isvoid d1 then
                    if d1.isString() then io.out_string(d1) else io.out_string(i2a(d1)) fi;
                    io.out_string("\n")
                else
                    io.out_string("队列为空\n")
                fi;

            io.out_string("出队元素2: ");
            let d2: Object <- queue.dequeue() in
                if not isvoid d2 then
                    if d2.isString() then io.out_string(d2) else io.out_string(i2a(d2)) fi;
                    io.out_string("\n")
                else
                    io.out_string("队列为空\n")
                fi;

            io.out_string("出队元素3: ");
            let d3: Object <- queue.dequeue() in
                if not isvoid d3 then
                    if d3.isString() then io.out_string(d3) else io.out_string(i2a(d3)) fi;
                    io.out_string("\n")
                else
                    io.out_string("队列为空\n")
                fi;

            io.out_string("出队3个元素后: ");
            queue.print();  (* 预期输出：[456, orange] *)

            (* 测试6：再次入队并打印最终状态 *)
            io.out_string("\n6. 再次入队测试:\n");
            queue.enqueue("watermelon");
            queue.enqueue(789);
            io.out_string("再次入队2个元素后: ");
            queue.print();  (* 预期输出：[456, orange, watermelon, 789] *)

            (* 测试7：清空队列 *)
            io.out_string("\n7. 清空队列测试:\n");
            while not queue.isEmpty() loop
                {
                    let d: Object <- queue.dequeue();
                    io.out_string("出队: ");
                    if not isvoid d then
                        if d.isString() then io.out_string(d) else io.out_string(i2a(d)) fi;
                        io.out_string("\n")
                    else
                        io.out_string("队列为空\n")
                    fi;
                }
            pool;
            io.out_string("清空后，队列为空吗？ ");
            if queue.isEmpty() then
                io.out_string("是\n")
            else
                io.out_string("否\n")
            fi;

            (* 测试8：空队列的出队操作 *)
            io.out_string("\n8. 空队列的出队操作:\n");
            let empty_deq: Object <- queue.dequeue() in
                if isvoid empty_deq then
                    io.out_string("空队列出队操作返回void，处理得当\n")
                else
                    io.out_string("错误：空队列出队不应返回非void值\n")
                fi;

            io.out_string("\n=== 队列测试完成 ===\n");
            self;
        }
    };
};

(* 编译命令：coolc CoolQueue完整程序.cl *)
(* 运行命令：spim a.out *)