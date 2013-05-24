#include "stdafx.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <sstream>

#include "CppLinq.h"

class InvalidResult{};
struct TestClass1
{
    TestClass1()
        : iVal(0)
        , dVal(0)
    {}
    virtual ~TestClass1(){}
    int iVal;
    double dVal;
    std::string sVal;

    bool operator<(const TestClass1 &rh) const
    {
        return (iVal < rh.iVal) || (dVal < rh.dVal) || (sVal < rh.sVal);
    }

    bool operator==(const TestClass1 &rh) const
    {
        return (iVal == rh.iVal) && (dVal == rh.dVal) && (sVal == rh.sVal);
    }

    bool operator!=(const TestClass1 &rh) const
    {
        return !(*this == rh);
    }
};

struct TestClass2: public TestClass1
{
    int iVal2;
    double dVal2;
    std::string sVal2;
};

class TestLinq{
public:
    void runMain();

    void aggregate();
    void all();
    void any();
    void contains();
    void count();
    void concat();
    void defaultIfEmpty();
    void distinct();
    void elementAt();
    void empty();
    void except();
    void where();
    void orderBy();
    void select();
    void selectMany();
    void single();
    void skip();
    void take();
    void last();
    void repeat();
    void reverse();
    void sum();
    void constSource();

    void containers();

private:
    const std::vector<TestClass1> testData1();
    const std::vector<TestClass2> testData2();
};

void TestLinq::runMain()
{
    aggregate();
    all();
    any();
    contains();
    count();
    concat();
    defaultIfEmpty();
    distinct();
    elementAt();
    empty();
    except();
    where();
    orderBy();
    select();
    selectMany();
    single();
    skip();
    take();
    last();
    repeat();
    reverse();
    sum();
    constSource();

    containers();
}

void TestLinq::aggregate()
{
    auto src = testData1();

    auto t = Linq::from(src)
        .aggregate(
        std::string(),
        [](const std::string &acc, const TestClass1 &a){ return acc + a.sVal; },
        [](const std::string &a) { return a + "!"; }
    );

    auto t2 = Linq::from(src)
        .aggregate(
        0,
        [](int acc, const TestClass1 &a){ return acc + a.iVal; }
    );

    auto t3 = Linq::from(src)
        .select([](const TestClass1 &a){ return a.dVal; })
        .aggregate(
        [](double acc, double a){ return acc + a; }
    );

    if (t2 != 45 || t3 != 450)
        throw new InvalidResult();
}

void TestLinq::all()
{
    auto src = testData1();
    auto t = Linq::from(src).
        all([](const TestClass1 &a){ return a.iVal >= 0; });
    auto f = Linq::from(src).
        all([](const TestClass1 &a){ return a.iVal > 0; });
    if (!t || f)
        throw new InvalidResult();
}

void TestLinq::any()
{
    auto src = testData1();
    auto t = Linq::from(src).
        any([](const TestClass1 &a){ return a.iVal >= 0; });
    auto f = Linq::from(src).
        all([](const TestClass1 &a){ return a.iVal < 0; });
    if (!t || f)
        throw new InvalidResult();
}

void TestLinq::contains()
{
    auto src = testData1();
    TestClass1 v;
    v.iVal = 5;
    auto f1 = Linq::from(src).
        contains(v);
    auto t2 = Linq::from(src).
        contains(v, [](const TestClass1 &a, const TestClass1 &b){ return a.iVal == b.iVal; });
    if (f1 || !t2)
        throw new InvalidResult();
}

void TestLinq::count()
{
    auto src = testData1();
    auto t1 = Linq::from(src).
        count() == 10;

    auto t2 = Linq::from(src).
        count([](const TestClass1 &a){ return a.iVal >= 5; }) == 5;
    if (!t1 || !t2)
        throw new InvalidResult();
}

void TestLinq::concat()
{
    auto src1 = testData1();
    auto src2 = testData1();

    auto t1 = Linq::from(src1).concat(Linq::from(src2)).count() == 20;

    if (!t1)
        throw new InvalidResult();
}

void TestLinq::defaultIfEmpty()
{
    auto src1 = testData1();
    auto t1 = Linq::from(src1).defaultIfEmpty().count() == 10;
    src1.clear();
    auto t2 = Linq::from(src1).defaultIfEmpty().count() == 1;
    TestClass1 defValue;
    auto t3 = Linq::from(src1).defaultIfEmpty(defValue).first() == defValue;

    if (!t1 || !t2 || !t3)
        throw new InvalidResult();
}

void TestLinq::distinct()
{
    auto src1 = testData1();
    auto src2 = testData1();
    src1.insert(src1.end(), src2.begin(), src2.end());

    auto t1 = Linq::from(src1).distinct().count() == 10;
    auto t2 = Linq::from(src1).distinct([](const TestClass1 &a){return a.iVal; }).count() == 10;
    if (!t1 || !t2)
        throw new InvalidResult();
}

void TestLinq::elementAt()
{
    auto src1 = testData1();
    auto t1 = Linq::from(src1)
        .elementAt(5);
    auto t2 = Linq::from(src1)
        .elementAtOrDefault(15);
    if (t1.iVal != 5 || t2.iVal != 0)
        throw new InvalidResult();
}

void TestLinq::empty()
{
    auto t1 = Linq::empty<int>()
        .count();
    if (t1 != 0)
        throw new InvalidResult();
}

void TestLinq::except()
{
    auto src1 = testData1();
    auto src2 = testData1();
    auto e = Linq::from(src2)
        .where([](const TestClass1 &a){return a.iVal >= 5; });
    auto t1 = Linq::from(src1)
        .except(e)
        .count();

    if (t1 != 5)
        throw new InvalidResult();
}

void TestLinq::where()
{
    auto src1 = testData1();

    auto t1 = Linq::from(src1)
        .where([](const TestClass1 &a){return a.iVal >= 5; })
        .count() == 5;

    if (!t1)
        throw new InvalidResult();
}

void TestLinq::orderBy()
{
    auto src1 = testData1();
    std::random_shuffle(src1.begin(), src1.end());

    auto t1 = Linq::from(src1).orderBy([](const TestClass1 &a){return a.iVal; }).toVector();
    auto t2 = Linq::from(src1).orderByDesc([](const TestClass1 &a){return a.iVal; }).toVector();
    auto t3 = Linq::from(src1)
        .orderBy(
        [](const TestClass1 &a){ return a.sVal; },
        [](std::string lh, std::string rh) { return lh > rh; }
    )
        .thenBy([](const TestClass1 &a){return a.iVal; })
        .toVector();
    auto t4 = Linq::from(src1)
        .orderByDesc([](const TestClass1 &a){return a.sVal; })
        .thenBy([](const TestClass1 &a){return a.iVal; })
        .toVector();
    auto t5 = Linq::from(src1)
        .orderByDesc([](const TestClass1 &a){return a.sVal; })
        .thenByDesc([](const TestClass1 &a){return a.iVal; })
        .toVector();
    //if (!t1)
    //	throw new InvalidResult();
}

void TestLinq::select()
{
    auto src1 = testData1();

    auto t1 = Linq::from(src1)
        .select([](const TestClass1 &a){return a.iVal; })
        .toVector();

    //if (!t1)
    //	throw new InvalidResult();
}

void TestLinq::selectMany()
{
    auto src1 = testData1();

    auto t1 = Linq::from(src1)
        .selectMany([](const TestClass1 &a){ return Linq::repeat(a.iVal, 3); })
        .orderByDesc()
        .toVector();

    if (t1.size() != 30)
        throw new InvalidResult();
}

void TestLinq::single()
{
    auto src1 = testData1();

    auto t1 = Linq::from(src1)
        .skip(1)
        .take(1)
        .single();

    TestClass1 defValue;
    auto t2 = Linq::from(src1)
        .skip(100)
        .take(1)
        .singleOrDefault();

    auto t3 = Linq::from(src1)
        .skip(100)
        .take(1)
        .singleOrDefault(defValue);

    if (t1.iVal != 1 || t2 != defValue || t3 != defValue )
        throw new InvalidResult();
}

void TestLinq::skip()
{
    auto src1 = testData1();

    auto t1 = Linq::from(src1)
        .skip(5)
        .count() == 5;

    auto t2 = Linq::from(src1)
        .skipWhile([](const TestClass1 &a){ return a.iVal < 5; })
        .count() == 5;

    if (!t1 || !t2)
        throw new InvalidResult();
}

void TestLinq::take()
{
    auto src1 = testData1();

    auto t1 = Linq::from(src1)
        .take(5)
        .count() == 5;

    auto t2 = Linq::from(src1)
        .takeWhile([](const TestClass1 &a){ return a.iVal < 5; })
        .count() == 5;

    if (!t1 || !t2)
        throw new InvalidResult();
}

void TestLinq::last()
{
    auto src1 = testData1();

    auto t1 = Linq::from(src1)
        .last();

    std::vector<int> empty;
    auto t2 = Linq::from(empty)
        .lastOrDefault(10);

    if (t1.iVal != 9 || t2 != 10)
        throw new InvalidResult();
}

void TestLinq::containers()
{
    std::set<int> set1;
    for(int i = 0; i < 10; i++)
        set1.insert(i);
    auto l1 =
        Linq::from(set1)
        .where([](const int &a){ return a < 5; })
        .orderBy([](const int &a){ return a; });
    auto v1 = l1.toVector();
    auto s1 = l1.toSet();
    auto list1 = l1.toList();
    auto m1 = l1.toMap(
        [](decltype(l1.const_reference()) &a){ return a; },
        [](decltype(l1.const_reference()) &a){ return a  + 10; }
    );

    double data1[] = {0 , 1, 2};
    const double data2[] = {0 , 1, 2};
    auto s2 = Linq::from(data1).sum();
    auto s3 = Linq::from(data2).sum();
}

void TestLinq::repeat()
{
    auto src1 = Linq::repeat(10, 5);
    auto v1 = src1.toVector();
    auto t1 = src1.count();

    if (t1 != 5)
        throw new InvalidResult();
}

void TestLinq::reverse()
{
    auto src1 = testData1();

    auto t1 = Linq::from(src1)
        .reverse();
    //.count();
    auto t2 = t1.reverse().toVector();

    if (t1.first().iVal != 9)
        throw new InvalidResult();
}

void TestLinq::sum()
{
    std::vector<int> vec1;
    for(int i = 0; i < 10; i++)
        vec1.push_back(i);

    auto t1 = Linq::from(vec1)
        .sum();

    auto src1 = testData1();
    auto t2 = Linq::from(src1)
        .sum([](const TestClass1 &a){return a.dVal; });
    if (t1 != 45 || t2 != 450 )
        throw new InvalidResult();
}

void TestLinq::constSource()
{
    std::vector<double> vec1;
    for(int i = 0; i < 10; i++)
        vec1.push_back(i);

    const auto &vec2 = vec1;

    auto t1 = Linq::from(vec2)
        .sum();

    auto t2 = Linq::from(vec2)
        .where([](const double &a){ return a < 5; })
        .orderBy([](const double &a){ return a; })
        .sum();

    auto t3 = Linq::from(vec2)
        .where([](const double &a){ return a < 5; })
        .orderBy([](const double &a){ return a; });
    auto el1 = t3.range->front();
    el1 = 1;

    if (t1 != 45)
        throw new InvalidResult();
}

const std::vector<TestClass1> TestLinq::testData1()
{
    std::vector<TestClass1> result;
    for (int i = 0; i < 10; i++)
    {
        TestClass1 tmp;
        tmp.iVal = i;
        tmp.dVal = i * 10;
        std::stringstream s;
        s << "str " << i / 3;
        tmp.sVal = s.str();;
        result.push_back(tmp);
    }
    return result;
}

const std::vector<TestClass2> TestLinq::testData2()
{
    std::vector<TestClass2> result;
    for (int i = 0; i < 10; i++)
    {
        TestClass2 tmp;
        tmp.iVal2 = i * 2;
        tmp.dVal2 = i * 20;
        std::stringstream s;
        s << "str " << i * 2;
        tmp.sVal2 = s.str();;
        result.push_back(tmp);
    }
    return result;
}

int _tmain(int argc, _TCHAR* argv[])
{
    TestLinq testLinq;
    testLinq.runMain();
    /*
    {
    int sum = 0;
    std::list<std::pair<int, double> > v1;
    std::vector<std::pair<int, int> > v2;
    for(int i = 0 ; i < 10; i++)
    {
    v1.push_back(std::pair<int, double>(i, rand()));
    v2.push_back(std::pair<int, int>(i, rand()));
    v2.push_back(std::pair<int, int>(i * 2, rand()));
    }
    auto src1 = from(v1.begin(), v1.end());
    src1 = from(v1);
    auto src2 = from(v2.begin(), v2.end());
    src2 = from(v2);
    auto src3 = src2;
    auto dst1 = src3
    .where([](decltype(src3.const_reference()) a) {return a.first < 7;} )
    .where([](decltype(src3.const_reference()) a) {return a.first < 50;} );
    auto dst2 = dst1
    .where([](decltype(src3.const_reference()) a) {return a.first > 0;} )
    .orderBy([](decltype(src3.const_reference()) a) {return -a.first;} )
    //.thenBy([](decltype(src3.const_reference()) a) {return -a.second;} )
    ;
    auto dst3 = dst2
    .thenBy([](decltype(src3.const_reference()) a) {return a.second;} )
    .select([](decltype(src3.const_reference()) a) {return a.second;} )
    .distinct()
    .skip(0)
    .take(10)
    ;

    auto res1 = dst1.toVector();
    auto res2 = dst2.toVector();
    auto res3 = dst3.toVector();
    auto res4 = dst3.first();
    auto res5 = dst3.skip(1).firstOrDefault();
    auto res6 = dst3.min();
    auto res7 = dst3.max();
    auto is1 = dst3.contains(5705);
    auto res8 = src1.join(
    src2,
    [](decltype(src1.const_reference()) a){ return a.first; },
    [](decltype(src2.const_reference()) a){ return a.first; },
    [](decltype(src1.const_reference()) lh, decltype(src1.const_reference()) rh) {return std::pair<int, double>(lh.first, rh.second); }
    //,[](const int &a, const int &b) -> bool{ return a > b; }
    )
    .toVector();
    auto res9 = dst3.take(1).single();
    auto cnt = dst3.count();
    auto re10 = src1.selectMany(
    [&]
    (decltype(src1.const_reference()) a)
    {
    return from(v2).where(
    [=]
    (decltype(from(v2).const_reference()) b)
    {return b.first == a.first ;}
    );
    }
    )
    .toVector();
    auto res11 = src2.toVector();
    auto res12 = src2.distinct(
    [](decltype(src2.const_reference()) a){ return a.first; }
    ).toVector();

    auto res13 = src2.groupBy(
    [](decltype(src2.const_reference()) a){ return a.first; }//,
    //[](decltype(src2.const_reference()) a){ return a.second; }
    ).toVector();
    for (auto ii = res13.begin(); ii != res13.end(); ++ii)
    {
    auto sub = ii->second;
    }

    auto isAll = src2.all([](decltype(src2.const_reference()) a){ return a.first >= 0; });
    auto isAny = src2.any([](decltype(src2.const_reference()) a){ return a.first >= 10; });
    }
    std::cout << Count << std::endl;
    return 0;
    8/
    /*
    auto dst2 = src1
    .where([](decltype(src1.const_reference()) a) {return a.first < 7;} )
    .where([](decltype(src1.const_reference()) a) {return a.first > 3;} )
    .innerJoin(
    src2,
    [](decltype(src1.const_reference()) lh, decltype(src2.const_reference()) rh) { return lh.first == rh.first; }
    //[](decltype(src1.const_reference()) lh) { return lh.first; },
    //[](decltype(src2.const_reference()) rh) { return rh.first; }
    );
    auto dst4 = dst2
    .select([](decltype(dst2.reference()) a){return a.second.second;});
    auto dst5 = dst4
    //.elect([](decltype(dst4.const_reference()) lh, decltype(dst4.const_reference()) rh) { return lh > rh; });
    //.min();
    .max();
    //.toVector();

    auto dst3 = dst2
    .orderBy([](decltype(dst2.const_reference()) a){return a.second.second;})
    .toVector();

    v1.push_back(std::pair<int, double>(4, rand()));
    v2.push_back(std::pair<int, int>(4, rand()));
    auto dst6 = dst2
    .orderBy([](decltype(dst2.const_reference()) a){return a.second.second;})
    .toVector();
    auto dst7 = dst2
    .skip(30)
    .toVector();

    auto f = dst2.first();
    auto fd = dst2
    .where([](decltype(dst2.const_reference()) a){ return false; })
    .firstOrDefault();
    */
    return 0;
}