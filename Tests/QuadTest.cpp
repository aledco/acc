#include <iostream>
#include <vector>
#include <gtest/gtest.h>
#include "test_util.hpp"
#include "Quad.hpp"

TEST(Quad, Test_QuadList)
{
    auto enter = Quad::MakeEnterOp(nullptr);
    enter->next = Quad::MakeParamOp(nullptr);
    auto ret = enter->next->next = Quad::MakeReturnOp(nullptr);

    QuadList list1;
    EXPECT_TRUE(list1.empty());
    list1.push_back(enter);
    EXPECT_FALSE(list1.empty());

    QuadList list2(enter->next, ret);
    EXPECT_FALSE(list2.empty());

    QuadList list3 = QuadList::concat(list1, list2);
    EXPECT_FALSE(list3.empty());

    int i = 0;
    for (auto curr = list3.get_head(); curr != nullptr; curr = curr->next)
    {
        i++;
    }

    EXPECT_EQ(i, 3);
}
