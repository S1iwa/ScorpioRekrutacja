#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int8.hpp"
#include "std_srvs/srv/set_bool.hpp"

namespace example_package {

/**
 * @brief Example node with simple publisher, subscriber, and a service.
 * 
 * Subscribes to a Int8 message and stores the value,
 * Publishes an Int8 message with the stored value every second,
 * Provides a service that accepts a boolean and sets a flag.
 */
class ExampleNode : public rclcpp::Node {
public:
  explicit ExampleNode(const rclcpp::NodeOptions& options)
    : Node("example_node", options) {
    _publisher = this->create_publisher<std_msgs::msg::Int8>("~/publisher_topic", 10);

    _subscriber = this->create_subscription<std_msgs::msg::Int8>("~/subscriber_topic", 10,
      std::bind(&ExampleNode::subscriber_callback, this, std::placeholders::_1));

    _service = this->create_service<std_srvs::srv::SetBool>("~/example_service",
      std::bind(
        &ExampleNode::service_callback,
        this, std::placeholders::_1, std::placeholders::_2));

    _timer = this->create_wall_timer(std::chrono::seconds(1),
      std::bind(&ExampleNode::timer_callback, this));
  }

private:
  rclcpp::Publisher<std_msgs::msg::Int8>::SharedPtr _publisher;
  rclcpp::Subscription<std_msgs::msg::Int8>::SharedPtr _subscriber;
  rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr _service;

  rclcpp::TimerBase::SharedPtr _timer;

  int _stored_value = 0;
  bool _flag = false;

  void subscriber_callback(const std_msgs::msg::Int8::SharedPtr msg) {
    _stored_value = msg->data;
    RCLCPP_INFO(this->get_logger(), "Received: %d", msg->data);
  }

  void timer_callback() {
    auto msg = std_msgs::msg::Int8();
    msg.data = _stored_value;
    _publisher->publish(msg);
  }

  void service_callback(const std_srvs::srv::SetBool::Request::SharedPtr request,
                        std_srvs::srv::SetBool::Response::SharedPtr response) {
    RCLCPP_INFO(this->get_logger(),
      "Received request with data: %s", request->data ? "true" : "false");
    _flag = request->data;
    response->success = true;
    response->message = "Flag set to " + std::string(_flag ? "true" : "false");
  }
};

}  // namespace example_package

// Register the component
#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(example_package::ExampleNode)
