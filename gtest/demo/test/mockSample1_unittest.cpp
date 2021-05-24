#include "../src/mockSample1.h"

#include <gmock/gmock.h>

class MockParent : public Parent {
 public:
  //! MOCK_[CONST_]METHODx(方法名, 返回类型(参数类型列表));
  MOCK_CONST_METHOD0(getNum,
                     int());  //! 由于 getNum() 是 const 成员函数，所以要使用
                              //! MOCK_CONST_METHODx
  MOCK_METHOD1(setResult, void(int));
  MOCK_METHOD1(print, void(const string &));
  MOCK_METHOD2(calc, int(int, double));
};

using ::testing::_;
using ::testing::Return;

TEST(mockDemo, 1) {
  MockParent p;
  Target t(&p);

  //! 设置 p.getNum() 方法的形为
  EXPECT_CALL(p, getNum())
      .Times(2)               //! 期望被调两次
      .WillOnce(Return(2))    //! 第一次返回值为2
      .WillOnce(Return(10));  //! 第二次返回值为10

  //! 设置 p.setResult(), 参数为2的调用形为
  EXPECT_CALL(p, setResult(2)).Times(1);

  EXPECT_CALL(
      p,
      print(_))  //! 表示任意的参数，其中 _ 就是 ::testing::_ ，如果冲突要注意
      .Times(2);

  EXPECT_EQ(t.doThis(), 10);
}

TEST(mockDemo, 2) {
  MockParent p;
  Target t(&p);

  EXPECT_CALL(p, calc(1, 2.2)).Times(1).WillOnce(Return(3));

  EXPECT_EQ(t.doThat(), 3);
}
