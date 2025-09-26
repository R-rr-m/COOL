(* A2I类：整数与字符串互转工具类 *)
class A2I inherits Object {
    (* 从字符串转换为整数 *)
    a2i(s: String): Int {
        if s.length() = 0 then 0
        else if s.substr(0, 1) = "-" then ~a2i_aux(s.substr(1, s.length() - 1), 0)
        else a2i_aux(s, 0) fi fi
    };

    (* 辅助递归，逐位处理字符串 *)
    a2i_aux(s: String, n: Int): Int {
        if s.length() = 0 then n
        else a2i_aux(s.substr(1, s.length() - 1), n * 10 + (s.substr(0, 1)).ord() - 48)
        fi
    };

    (* 从整数转换为字符串 *)
    i2a(i: Int): String {
        if i = 0 then "0"
        else if i < 0 then "-" ^ i2a_aux(~i, "")
        else i2a_aux(i, "") fi fi
    };

    (* 辅助递归，将每一位加到字符串前面 *)
    i2a_aux(i: Int, s: String): String {
        if i = 0 then s
        else i2a_aux(i / 10, (((i - (i / 10) * 10) + 48).chr()) ^ s)
        fi
    };
};

(* QueueNode类：队列节点，存储单个元素和下一节点引用 *)
class QueueNode inherits Object {
    item: Object;             (* 节点存储的数据 *)
    next: QueueNode;          (* 指向下一节点的指针 *)

    (* 初始化节点 *)
    init(i: Object, n: QueueNode): QueueNode {
        {
            item <- i;
            next <- n;
            self;
        }
    };
};

(* Queue类：队列操作 *)
class Queue inherits Object {
    front: QueueNode;         (* 队首指针 *)
    rear: QueueNode;          (* 队尾指针 *)
    size: Int;                (* 队列元素个数 *)

    (* 初始化空队列 *)
    init(): Queue {
        {
            front <- void;
            rear <- void;
            size <- 0;
            self;
        }
    };

    (* 入队 *)
    enqueue(item: Object): Object {
        let new_node: QueueNode <- (new QueueNode).init(item, void) in {
            if isvoid front then {
                front <- new_node;
                rear <- new_node;
            } else {
                rear.next <- new_node;
                rear <- new_node;
            } fi;
            size <- size + 1;
            item;
        }
    };

    (* 出队 *)
    dequeue(): Object {
        if isEmpty() then
            void
        else
            let dequeued_item: Object <- front.item,
                new_front: QueueNode <- front.next
            in {
                front <- new_front;
                size <- size - 1;
                if isvoid front then
                    rear <- void
                fi;
                dequeued_item;
            }
        fi
    };

    (* 查看队首元素 *)
    front_item(): Object {
        if isEmpty() then
            void
        else
            front.item
        fi
    };

    (* 判断队列是否为空 *)
    isEmpty(): Bool {
        size = 0
    };

    (* 打印队列（支持Int和String类型） *)
    print(): Object {
        let io: IO <- new IO, current: QueueNode <- front in {
            io.out_string("[");
            while not isvoid current loop
                case current.item of
                    i: Int =>
                        let a2i: A2I <- new A2I in io.out_string(a2i.i2a(i));
                    s: String =>
                        io.out_string(s);
                    o: Object =>
                        io.out_string("Unknown");
                esac;
                if not isvoid current.next then
                    io.out_string(", ")
                fi;
                current <- current.next;
            pool;
            io.out_string("]\n");
            self;
        }
    };
};

(* Main类：测试队列所有功能，程序入口 *)
class Main inherits A2I {
    main(): Object {
        let io: IO <- new IO, queue: Queue <- (new Queue).init() in {
            io.out_string("--- COOL语言队列实现测试 ---\n");

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
            if isvoid queue.front_item() then
                io.out_string("队列为空，无队首元素\n")
            else
                case queue.front_item() of
                    s: String => io.out_string("队首元素: "); io.out_string(s); io.out_string("\n");
                    i: Int => io.out_string("队首元素: "); io.out_string(i2a(i)); io.out_string("\n");
                    o: Object => io.out_string("队首元素未知类型\n");
                esac
            fi;

            (* 测试3：入队5个元素（混合String和Int类型） *)
            io.out_string("\n3. 入队测试:\n");
            queue.enqueue("apple");
            queue.enqueue(123);
            queue.enqueue("banana");
            queue.enqueue(456);
            queue.enqueue("orange");
            io.out_string("入队5个元素后: ");
            queue.print();

            (* 测试4：查看队首元素 *)
            io.out_string("\n4. 查看队首元素:\n");
            io.out_string("队首元素: ");
            if not isvoid queue.front_item() then
                case queue.front_item() of
                    s: String => io.out_string(s); io.out_string("\n");
                    i: Int => io.out_string(i2a(i)); io.out_string("\n");
                    o: Object => io.out_string("未知\n");
                esac
            else
                io.out_string("队列为空\n")
            fi;

            (* 测试5：出队3个元素 *)
            io.out_string("\n5. 出队测试:\n");
            io.out_string("出队元素1: ");
            let d1: Object <- queue.dequeue() in
                if not isvoid d1 then
                    case d1 of
                        s: String => io.out_string(s); io.out_string("\n");
                        i: Int => io.out_string(i2a(i)); io.out_string("\n");
                        o: Object => io.out_string("未知\n");
                    esac
                else
                    io.out_string("队列为空\n")
                fi;

            io.out_string("出队元素2: ");
            let d2: Object <- queue.dequeue() in
                if not isvoid d2 then
                    case d2 of
                        s: String => io.out_string(s); io.out_string("\n");
                        i: Int => io.out_string(i2a(i)); io.out_string("\n");
                        o: Object => io.out_string("未知\n");
                    esac
                else
                    io.out_string("队列为空\n")
                fi;

            io.out_string("出队元素3: ");
            let d3: Object <- queue.dequeue() in
                if not isvoid d3 then
                    case d3 of
                        s: String => io.out_string(s); io.out_string("\n");
                        i: Int => io.out_string(i2a(i)); io.out_string("\n");
                        o: Object => io.out_string("未知\n");
                    esac
                else
                    io.out_string("队列为空\n")
                fi;

            io.out_string("出队3个元素后: ");
            queue.print();

            (* 测试6：再次入队并打印最终状态 *)
            io.out_string("\n6. 再次入队测试:\n");
            queue.enqueue("watermelon");
            queue.enqueue(789);
            io.out_string("再次入队2个元素后: ");
            queue.print();

            (* 测试7：清空队列 *)
            io.out_string("\n7. 清空队列测试:\n");
            while not queue.isEmpty() loop
                let d: Object <- queue.dequeue() in
                    (
                        io.out_string("出队: ");
                        if not isvoid d then
                            case d of
                                s: String => io.out_string(s); io.out_string("\n");
                                i: Int => io.out_string(i2a(i)); io.out_string("\n");
                                o: Object => io.out_string("未知\n");
                            esac
                        else
                            io.out_string("队列为空\n")
                        fi;
                    )
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

            io.out_string("\n--- 队列测试完成 ---");
            self;
        }
    };
};
