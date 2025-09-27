(*
 * 一个用COOL语言实现的队列(Queue)数据结构。
 * 队列是一种先进先出(FIFO)的数据结构。
 * 主要操作包括：入队、出队、查看队首元素、判断队列是否为空。
 *)

(*
 * QueueNode 类
 * 代表队列中的一个节点，包含一个数据项(item)和指向下一个节点的引用(next)。
 *)
class QueueNode inherits Object {
    item : Object;  (* 节点存储的数据 *)
    next : QueueNode;  (* 指向下一节点的引用 *)
    
    (* 初始化节点 *)
    init(i : Object, n : QueueNode) : QueueNode {
        {
            item <- i;
            next <- n;
            self;
        }
    };
    
    (* 返回节点的数据 *)
    getItem() : Object {
        item
    };
    
    (* 返回下一个节点 *)
    getNext() : QueueNode {
        next
    };
};

(*
 * Queue 类
 * 实现队列数据结构，包含队首(front)和队尾(rear)指针。
 *)
class Queue inherits Object {
    front : QueueNode;  (* 队首指针 *)
    rear : QueueNode;   (* 队尾指针 *)
    
    (* 初始化空队列 *)
    init() : Queue {
        {
            front <- void;
            rear <- void;
            self;
        }
    };
    
    (* 入队操作：将元素添加到队列尾部 *)
    enqueue(item : Object) : Object {
        let new_node : QueueNode <- (new QueueNode).init(item, void) in
        {
            if isvoid front then
                (* 如果队列为空，新节点同时是队首和队尾 *)
                {
                    front <- new_node;
                    rear <- new_node;
                }
            else
                (* 否则，将新节点添加到队尾，并更新队尾指针 *)
                {
                    rear.next <- new_node;
                    rear <- new_node;
                }
            fi;
            item;
        }
    };
    
    (* 出队操作：移除并返回队首元素 *)
    dequeue() : Object {
        if isEmpty() then
            (* 如果队列为空，返回void *)
            void
        else
            let item : Object <- front.getItem() in
            {
                (* 更新队首指针 *)
                front <- front.getNext();
                
                (* 如果队列变为空，也要更新队尾指针 *)
                if isvoid front then
                    rear <- void
                else
                    0  (* COOL语言要求else分支必须有表达式 *)
                fi;
                
                item;  (* 返回被移除的元素 *)
            }
        fi
    };
    
    (* 返回队首元素但不移除 *)
    front() : Object {
        if isEmpty() then
            void
        else
            front.getItem()
        fi
    };
    
    (* 检查队列是否为空 *)
    isEmpty() : Bool {
        isvoid front
    };
    
    (* 打印队列中的所有元素 *)
    print() : Object {
        let io : IO <- new IO,
            current : QueueNode <- front in
        {
            io.out_string("Queue: [");
            
            while not isvoid current loop
            {
                case current.getItem() of
                    i : Int => io.out_int(i);
                    s : String => io.out_string(s);
                    o : Object => io.out_string("Object");
                esac;
                
                current <- current.getNext();
                
                if not isvoid current then
                    io.out_string(", ")
                else
                    0  (* COOL语言要求else分支必须有表达式 *)
                fi;
            }
            pool;
            
            io.out_string("]\n");
            self;
        }
    };
};

(* 
 * Main类：测试队列功能
 *)
class Main inherits IO {
    main() : Object {
        let queue : Queue <- (new Queue).init() in
        {
            out_string("=== 队列(Queue)测试 ===\n");
            
            (* 测试1：检查空队列 *)
            out_string("1. 空队列检查: ");
            if queue.isEmpty() then
                out_string("队列为空\n")
            else
                out_string("队列不为空\n")
            fi;
            
            (* 测试2：入队操作 *)
            out_string("\n2. 入队操作:\n");
            queue.enqueue("苹果");
            out_string("入队: 苹果\n");
            queue.enqueue(123);
            out_string("入队: 123\n");
            queue.enqueue("香蕉");
            out_string("入队: 香蕉\n");
            
            out_string("当前队列: ");
            queue.print();
            
            (* 测试3：查看队首元素 *)
            out_string("\n3. 查看队首元素:\n");
            let front_item : Object <- queue.front() in
            {
                out_string("队首元素: ");
                if not isvoid front_item then
                    case front_item of
                        i : Int => out_int(i);
                        s : String => out_string(s);
                        o : Object => out_string("Object");
                    esac
                    out_string("\n");
                else
                    out_string("队列为空\n");
                fi;
            };
            
            (* 测试4：出队操作 *)
            out_string("\n4. 出队操作:\n");
            let item1 : Object <- queue.dequeue() in
            {
                out_string("出队元素: ");
                if not isvoid item1 then
                    case item1 of
                        i : Int => out_int(i);
                        s : String => out_string(s);
                        o : Object => out_string("Object");
                    esac
                    out_string("\n");
                else
                    out_string("队列为空\n");
                fi;
            };
            
            out_string("出队后队列: ");
            queue.print();
            
            (* 测试5：多次出队直到队列为空 *)
            out_string("\n5. 清空队列:\n");
            
            let item2 : Object <- queue.dequeue() in
            {
                out_string("出队元素: ");
                if not isvoid item2 then
                    case item2 of
                        i : Int => out_int(i);
                        s : String => out_string(s);
                        o : Object => out_string("Object");
                    esac
                    out_string("\n");
                else
                    out_string("队列为空\n");
                fi;
            };
            
            let item3 : Object <- queue.dequeue() in
            {
                out_string("出队元素: ");
                if not isvoid item3 then
                    case item3 of
                        i : Int => out_int(i);
                        s : String => out_string(s);
                        o : Object => out_string("Object");
                    esac
                    out_string("\n");
                else
                    out_string("队列为空\n");
                fi;
            };
            
            (* 测试6：检查队列是否为空 *)
            out_string("\n6. 最终队列状态:\n");
            out_string("队列为空? ");
            if queue.isEmpty() then
                out_string("是\n")
            else
                out_string("否\n")
            fi;
            
            out_string("\n=== 测试完成 ===\n");
            
            self;
        }
    };
};
    };
};
