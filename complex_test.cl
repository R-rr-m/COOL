class Main inherits IO {
    main() : Object {
        {
            out_string("Factorial of 5 is: ");
            out_int(fact(5));
            out_string("\n");
            
            out_string("Polymorphism test:\n");
            let a : A <- new B in {
                a.identify(); -- Should print "I am B"
                out_string("Case test:\n");
                test_case(a); -- Should match B
                test_case(new A); -- Should match A
            };
        }
    };

    fact(n : Int) : Int {
        if n = 0 then 1 else n * fact(n - 1) fi
    };

    test_case(x : Object) : Object {
        case x of
            b : B => out_string("Matched B\n");
            a : A => out_string("Matched A\n");
            o : Object => out_string("Matched Object\n");
        esac
    };
};

class A inherits IO {
    identify() : Object {
        out_string("I am A\n")
    };
};

class B inherits A {
    identify() : Object {
        out_string("I am B\n")
    };
};
