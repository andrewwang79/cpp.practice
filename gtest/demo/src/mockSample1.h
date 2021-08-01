#include <string>
using namespace std;

#ifdef MACRO_ENABLE_UT
  #define UNIT_TEST_PRIVATE public
#else
  #define UNIT_TEST_PRIVATE private
#endif

class Parent {
 public:
  virtual ~Parent() {}

  virtual int getNum() const = 0;
  virtual void setResult(int value) = 0;
  virtual void print(const string &str) = 0;
  virtual int calc(int a, double b) = 0;
};

class Target {
 public:
  Target(Parent *parent) : parent_(parent) {}

  int doThis() {
    int v = parent_->getNum();
    parent_->setResult(v);
    while (v-- > 0) {
      parent_->print(to_string(v));
    }
    return parent_->getNum();
  }

  int doThat() { return parent_->calc(1, 2.2); }

 private:
  Parent *parent_;

 UNIT_TEST_PRIVATE:
  int pp(int x) { return x + 1; }
};
