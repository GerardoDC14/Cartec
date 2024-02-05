import rclpy
import serial, time, json, sys
from rclpy.node import Node
from std_msgs.msg import Float64

class MinimalPublisher(Node):

    def __init__(self):
        super().__init__('py_pub_data_node')
        self.publisher_ = self.create_publisher(Float64, 'sd_dm/cmd_pos', 1)
        timer_period = 0.5  # seconds
        self.i = 0.0
        self.timer_ = self.create_timer(timer_period, self.publish_message)
        
    def publish_message(self):
        message = Float64()
        cad = 0.0
        message.data = cad + self.i
        
        self.get_logger().info('Sending - Angular Position : %f' % (message.data))
        self.publisher_.publish(message)
        self.i += float(sys.argv[1])

def main(args=None):
    rclpy.init(args=args)
    py_pub_data_node = MinimalPublisher()
    rclpy.spin(py_pub_data_node)
    py_pub_data_node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
