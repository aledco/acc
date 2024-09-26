#include <memory>
#include <iostream>

class A
{
public:
    virtual void dump() = 0;
};

class B : public A
{
public:
    void dump() override;
};

void B::dump()
{
    std::cout << "B.dump()\n";
}

class C : public B
{
public:
    void dump() override;
};

void C::dump()
{
    std::cout << "C.dump()\n";
}

void dump(A *a)
{
    std::cout << "dump(a)\n";
}

void dump(B *b)
{
    std::cout << "dump(b)\n";
}

void dump(C *c)
{
    std::cout << "dump(c)\n";
}


int main()
{
    std::unique_ptr<B> b = std::make_unique<B>();
    std::unique_ptr<A> b_as_a = std::make_unique<B>();

    std::unique_ptr<C> c = std::make_unique<C>();
    std::unique_ptr<B> c_as_b = std::make_unique<C>();
    std::unique_ptr<A> c_as_a = std::make_unique<C>();

    b->dump();
    b_as_a->dump();
    c->dump();
    c_as_b->dump();
    c_as_a->dump();

    dump(b.get());
    dump(b_as_a.get());
    dump(c.get());
    dump(c_as_b.get());
    dump(c_as_a.get());

    return 0;
}