#include <Arduino.h>
#include <micro_ros_platformio.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

// 添加接口
#include <example_interfaces/srv/add_two_ints.h>

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

// 定义服务
rcl_service_t service;

// 服务请求和返回消息定义
example_interfaces__srv__AddTwoInts_Request res;
example_interfaces__srv__AddTwoInts_Response req;

// 服务回调函数
void service_callback(const void *req, void *res)
{
  example_interfaces__srv__AddTwoInts_Request *req_in = (example_interfaces__srv__AddTwoInts_Request *)req;
  example_interfaces__srv__AddTwoInts_Response *res_in = (example_interfaces__srv__AddTwoInts_Response *)res;
  // 计算sum
  res_in->sum = req_in->a + req_in->b;
}

void setup()
{
  Serial.begin(115200);
  // 设置通过串口进行MicroROS通信
  set_microros_serial_transports(Serial);
  // 延时时一段时间，等待设置完成
  delay(2000);
  // 初始化内存分配器
  allocator = rcl_get_default_allocator();
  // 创建初始化选项
  rclc_support_init(&support, 0, NULL, &allocator);
  // 创建节点 hello_microros
  rclc_node_init_default(&node, "service_test", "", &support);
  // 使用默认配置创建服务
  rclc_service_init_default(&service, &node, ROSIDL_GET_SRV_TYPE_SUPPORT(example_interfaces, srv, AddTwoInts), "/addtwoints");
  // 创建执行器
  rclc_executor_init(&executor, &support.context, 1, &allocator);
  // 执行器添加服务
  rclc_executor_add_service(&executor, &service, &req, &res, service_callback);
}

void loop()
{
  delay(100);
  // 循环处理数据
  rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
}
