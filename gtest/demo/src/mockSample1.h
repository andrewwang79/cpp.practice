#include <string>
using namespace std;

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
};
