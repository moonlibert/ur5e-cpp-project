from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource

from launch.substitutions import PathJoinSubstitution

from launch_ros.substitutions import FindPackageShare


def generate_launch_description():

    ur_driver = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution(
                [
                    FindPackageShare("ur_robot_driver"),
                    "launch",
                    "ur_control.launch.py",
                ]
            )
        ),
        launch_arguments={
            "ur_type": "ur5e",
            "robot_ip": "192.168.56.101",          # 改成你的机器人IP
            "use_fake_hardware": "true",
            "description_package": "ur5e_force_description",
            "description_file": "ur5e_force.urdf.xacro",
            "launch_rviz": "false",
        }.items(),
    )

    return LaunchDescription([
        ur_driver,
    ])