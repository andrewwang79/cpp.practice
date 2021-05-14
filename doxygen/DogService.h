#include <iostream>

using namespace std;

namespace cn::wangyaqi::cpp::practice {
namespace doxygen {
class DogService {
 public:
  /**
   * @brief 简介，比如主要功能
   * @date   建立时间
   * @author 作者
   * @param  :
   * | 名称 | 字段 | 类型 | 必须 | 说明 |
   * | :-: | - | - | - | - |
   * | 类型     | type                 | string        | Y | 类型枚举 |
   * | 序列号   | sn                   | string        | Y | 唯一     |
   * @return
   * | 名称 | 字段 | 类型 | 必须 | 说明 |
   * | :-: | - | - | - | - |
   * | 结果     | result               | string        | Y | 结果情况 |
   * | 区域     | detection/polygon    | array<double> | Y | 2D       |
   * 返回码
   * | 返回码 | 名称 | 说明 |
   * | :-: | - | - |
   * | 1001 | 图像读取错误         |  |
   * | 1002 | 图像不合法           |  |
   * @warning   一些警告
   * @attention 一些注意事项
   * @note 一些备注说明。一般用于接口调整记录
   * | 时间 | 作者 | 说明 |
   * | :-: | - | - |
   * | 20210512 | wangyaqi   | 增加输入参数xxx |
   */
  int eat(string food);
}
}  // namespace doxygen
}  // namespace cn::wangyaqi::cpp::practice
