from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
   return LaunchDescription([
       Node(
            package='tl_perception',
            namespace='tl_perception1',
            executable='model_inference',
            name='sim'
        ),
       Node(
            package='tl_perception',
            namespace='tl_perception2',
            executable='depth_estimation',
            name='sim'
        ),
       Node(
            package='tl_perception',
            namespace='tl_perception3',
            executable='tl_decision_making',
            name='sim'
        )
   ])
