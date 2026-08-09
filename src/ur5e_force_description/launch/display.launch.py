from launch import LaunchDescription
from launch.substitutions import Command
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

import os


def generate_launch_description():

    pkg_path = get_package_share_directory("ur5e_force_description")

    robot_description = Command([
        "xacro ",
        os.path.join(pkg_path, "urdf", "ur5e_force.urdf.xacro")
    ])

    return LaunchDescription([

        Node(
            package="robot_state_publisher",
            executable="robot_state_publisher",
            parameters=[{
                "robot_description": robot_description
            }]
        ),

        Node(
            package="joint_state_publisher_gui",
            executable="joint_state_publisher_gui"
        ),

        Node(
            package="rviz2",
            executable="rviz2"
        )

    ])