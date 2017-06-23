#include <iostream>
#include <string>
#include <memory>

#pragma execution_character_set("utf-8")

using namespace std;

class Beverage
{
public:
    virtual string description() const
    {
        return "unknown beverage";
    }

    virtual double cost() const  = 0;
};

// 具体组件
class Espresso : public Beverage
{
public:
    string description() const override
    {
        return "Espresso";
    }

    double cost() const override
    {
        return 1.99;
    }
};

class HouseBlend : public Beverage
{
public:
    string description() const override
    {
        return "House Blend Coffee";
    }

    double cost() const
    {
        return 0.89;
    }
};

class DarkRoast : public Beverage
{
public:
    string description() const override
    {
        return "Dark Roast Coffee";
    }

    double cost() const
    {
        return 0.99;
    }
};


// 调料装饰者基类, 目的是让所有装饰者重新实现description函数
class CondimentDecorator: public Beverage
{
public:
    explicit CondimentDecorator(shared_ptr<Beverage> beverage)
        : m_beverage(beverage)
    {
        if(!beverage)
        {
            throw ;
        }
    }

    virtual ~CondimentDecorator()
    {
    }

    string description() const override = 0 ;

protected:
    const shared_ptr<Beverage> component() const
    {
        return m_beverage;
    }

private:
    shared_ptr<Beverage> m_beverage;
};

// 装饰者
class Mocha : public CondimentDecorator
{
public:
    using CondimentDecorator::CondimentDecorator;

    string description() const override
    {
        return component()->description() + ", Mocha";
    }

    double cost() const override
    {
        return 0.20 + component()->cost();
    }
};

class Whip : public CondimentDecorator
{
public:
    using CondimentDecorator::CondimentDecorator;

    string description() const override
    {
        return component()->description() + ", Whip";
    }

    double cost() const override
    {
        return 0.10 + component()->cost();
    }
};

class Soy : public CondimentDecorator
{
public:
    using CondimentDecorator::CondimentDecorator;

    string description() const override
    {
        return component()->description() + ", Soy";
    }

    double cost() const override
    {
        return 0.15 + component()->cost();
    }
};


int main()
{
    auto print = [](const shared_ptr<Beverage> &beverage)
    {
        if(!beverage) throw;
        cout << beverage->description() << " $ " << beverage->cost() << endl;
    };

    shared_ptr<Beverage> b1 = make_shared<DarkRoast>();
    b1 = make_shared<Mocha>(b1);
    b1 = make_shared<Mocha>(b1);
    b1 = make_shared<Whip>(b1);

    print(b1);

    shared_ptr<Beverage> b2 = make_shared<HouseBlend>();
    b2 = make_shared<Soy>(b2);
    b2 = make_shared<Mocha>(b2);
    b2 = make_shared<Whip>(b2);

    print(b2);

    return 0;
}
