(* 
 * Node 类
 * 表示链表中的一个节点，每个节点包含一个数据项(item)和指向下一个节点的引用(next)。
 * 继承自 IO 类以支持输入输出操作。
 *)
class Node inherits IO {
    item : Object;  (* 节点存储的数据 *)
    next : Node;    (* 指向下一个节点的引用 *)
    
    (* init(i : Object) : Node
     * 初始化节点方法
     * @param i - 要存储在节点中的数据项
     * @return 返回初始化后的节点自身
     * 说明：创建一个新节点，设置其数据项，并初始化next引用
     *)
    init(i : Object) : Node {
        {
            item <- i;
            next <- new Node;  (* 初始化为新节点 *)
            self;
        }
    };

    (* 获取节点中存储的数据 *)
    getItem() : Object {
        item
    };

    (* 获取下一个节点 *)
    getNext() : Node {
        next
    };

    (* 设置下一个节点 *)
    setNext(n : Node) : Node {
        {
            next <- n;
            self;
        }
    };
};

(* 
 * LinkedList 类
 * 实现单向链表数据结构，提供基本的链表操作功能。
 * 包括：插入、删除、查找、判空和打印等操作。
 * 继承自 IO 类以支持输入输出操作。
 *)
class LinkedList inherits IO {
    head : Node;    (* 链表头节点 *)
    
    (* 初始化方法 *)
    init() : LinkedList {
        {
            head <- new Node;  (* 初始化为新节点 *)
            self;
        }
    };

    (* insert(item : Object) : LinkedList
     * 在链表头部插入新节点
     * @param item - 要插入的数据项
     * @return 返回链表自身
     * 说明：创建新节点并将其插入到链表头部，原头节点成为新节点的下一个节点
     *)
    insert(item : Object) : LinkedList {
        let new_node : Node <- new Node in
        {
            new_node <- new_node.init(item);
            if not isvoid head then
                new_node.setNext(head)
            else
                new_node.setNext(new Node)
            fi;
            head <- new_node;
            self;
        }
    };

    (* delete(item : Object) : Bool
     * 删除链表中第一个匹配的项
     * @param item - 要删除的数据项
     * @return 如果找到并删除了项返回true，否则返回false
     * 说明：遍历链表查找匹配项，找到后调整相应的next引用以删除该节点
     *)
    delete(item : Object) : Bool {
        if isvoid head then
            false
        else 
            if head.getItem() = item then {
                head <- head.getNext();
                true;
            } else 
                let current : Node <- head,
                    found : Bool <- false
                in {
                    while not isvoid current.getNext() loop
                        if current.getNext().getItem() = item then {
                            current.setNext(current.getNext().getNext());
                            found <- true;
                        } else
                            current <- current.getNext()
                        fi
                    pool;
                    found;
                }
            fi
        fi
    };

    (* print() : Object
     * 打印链表内容
     * @return 返回self对象
     * 说明：遍历链表并打印每个节点的内容，支持Int、String和一般Object类型
     * 格式：item1 -> item2 -> ... -> null
     *)
      print() : Object {
        let current : Node <- head in {
            while not isvoid current loop {
                let item : Object <- current.getItem() in
                    if isvoid item then
                        out_string("void")
                    else
                        case item of
                            i : Int => out_int(i);
                            s : String => out_string(s);
                            o : Object => out_string("Object");
                        esac
                    fi;
                out_string(" -> ");
                current <- current.getNext();
            } pool;
            out_string("null\n");
            self;
        }
    };

    (* 检查链表是否为空 *)
    isEmpty() : Bool {
        isvoid head
    };

    (* search(item : Object) : Bool
     * 查找元素是否存在于链表中
     * @param item - 要查找的数据项
     * @return 如果找到项返回true，否则返回false
     * 说明：遍历链表查找是否存在匹配的数据项
     *)
    search(item : Object) : Bool {
        let current : Node <- head,
            found : Bool <- false
        in {
            while not isvoid current loop {
                if current.getItem() = item then {
                    found <- true;
                    current <- new Node;
                } else
                    current <- current.getNext()
                fi;
            } pool;
            found;
        }
    };
};

(* 
 * Main 类
 * 程序的主类，用于测试LinkedList类的各项功能
 * 包括：插入、删除、查找、判空等操作的测试用例
 *)
class Main inherits IO {
    main() : Object {
        let list : LinkedList <- new LinkedList in
        {
            list <- list.init();
            
            (* 测试插入操作 *)
            list.insert("Third");
            list.insert("Second");
            list.insert("First");
            
            out_string("Initial list: \n");
            list.print();
            
            (* 测试删除操作 *)
            out_string("\nDeleting 'Second'...\n");
            list.delete("Second");
            out_string("List after deletion: \n");
            list.print();
            
            (* 测试空链表检查 *)
            out_string("\nIs list empty? ");
            if list.isEmpty() then
                out_string("Yes\n")
            else
                out_string("No\n")
            fi;

            (* 测试查找操作 *)
            out_string("\nTesting search operation:\n");
            out_string("Searching for 'First': ");
            if list.search("First") then
                out_string("Found!\n")
            else
                out_string("Not Found!\n")
            fi;
            out_string("Searching for 'Second' (deleted item): ");
            if list.search("Second") then
                out_string("Found!\n")
            else
                out_string("Not Found!\n")
            fi;
            out_string("Searching for 'Fourth' (non-existent item): ");
            if list.search("Fourth") then
                out_string("Found!\n")
            else
                out_string("Not Found!\n")
            fi;
            self;
        }
    };
};