import rclpy
import serial, time, sys
from rclpy.node import Node
from std_msgs.msg import Float64

cad = 0

arduino = serial.Serial(port='/dev/ttyUSB0', baudrate=115200, timeout=1, write_timeout=1)
class MinimalSubscriber(Node):

    def __init__(self):
        super().__init__('py_sub_data_node')
        self.subscriber_ = self.create_subscription(Float64, 'sd_dm/cmd_pos', self.subscribe_message, 1)
        self.subscriber_  # prevent unused variable warning

    def subscribe_message(self, msg):
        self.get_logger().info('Recieved - Angular Position : %f' % (msg.data))
        cad = int(((msg.data)*100000))
        cad = 'v'+ str(cad)
        
        arduino.write(cad.encode('ascii'))
        time.sleep(1)
        # Print serial message
        try:
            print(cad)        
        except:
            print("Exception occurred, somthing wrong...")
        #arduino.close()


def main(args=None):
    rclpy.init(args=args)
    py_sub_data_node = MinimalSubscriber()
    rclpy.spin(py_sub_data_node)
    py_sub_data_node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
