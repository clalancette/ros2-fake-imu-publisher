#include <chrono>
#include <cstdio>
#include <functional>
#include <memory>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/imu.hpp>

class FakeImu final : public rclcpp::Node
{
public:
  FakeImu() : rclcpp::Node("fake_imu")
  {
    fprintf(stderr, "Fake IMU constructor\n");
    imu_pub_ = this->create_publisher<sensor_msgs::msg::Imu>("imu", 10);
    timer_ = this->create_wall_timer(std::chrono::milliseconds(10), std::bind(&FakeImu::timer_cb, this));
  }

  ~FakeImu()
  {
    fprintf(stderr, "Fake IMU destructor\n");
  }

private:
  void timer_cb()
  {
    auto msg = std::make_unique<sensor_msgs::msg::Imu>();
    msg->linear_acceleration.x = count_;
    count_ += 1.0;

    imu_pub_->publish(std::move(msg));
  }

  rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr imu_pub_;
  rclcpp::TimerBase::SharedPtr timer_;
  double count_{0};
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);

  // Force flush of the stdout buffer, which ensures a sync of all prints
  // even from a launch file.
  setvbuf(stdout, nullptr, _IONBF, BUFSIZ);

  auto fake_imu = std::make_shared<FakeImu>();

  rclcpp::spin(fake_imu);

  rclcpp::shutdown();

  return 0;
}
