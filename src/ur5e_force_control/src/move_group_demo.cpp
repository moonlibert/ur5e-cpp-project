#include <iostream>
#include <memory>
#include <vector>

#include <rclcpp/rclcpp.hpp>

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<rclcpp::Node>(
        "move_group_demo",
        rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true));

    // 为 MoveGroupInterface 开一个 executor
    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);

    std::thread spinner([&executor]() {
        executor.spin();
    });

    static const std::string PLANNING_GROUP = "ur5e_force_manipulator";

    moveit::planning_interface::MoveGroupInterface move_group(node, PLANNING_GROUP);

    std::cout << "\n==========================" << std::endl;
    std::cout << "Planning Group : " << PLANNING_GROUP << std::endl;
    std::cout << "Planning Frame : " << move_group.getPlanningFrame() << std::endl;
    std::cout << "End Effector   : " << move_group.getEndEffectorLink() << std::endl;
    std::cout << "==========================\n" << std::endl;

    // 当前关节角
    std::vector<double> joints = move_group.getCurrentJointValues();

    std::cout << "Current joint values:" << std::endl;
    for (size_t i = 0; i < joints.size(); ++i)
    {
        std::cout << "Joint[" << i << "] = " << joints[i] << std::endl;
    }

    // 第一关节旋转 0.2rad
    joints[0] += 0.2;

    move_group.setJointValueTarget(joints);

    moveit::planning_interface::MoveGroupInterface::Plan plan;

    bool success =
        (move_group.plan(plan) == moveit::core::MoveItErrorCode::SUCCESS);

    if (success)
    {
        std::cout << "\nPlanning SUCCESS!\n" << std::endl;

        auto result = move_group.execute(plan);

        if (result == moveit::core::MoveItErrorCode::SUCCESS)
            std::cout << "Execution SUCCESS!" << std::endl;
        else
            std::cout << "Execution FAILED!" << std::endl;
    }
    else
    {
        std::cout << "\nPlanning FAILED!" << std::endl;
    }

    executor.cancel();
    spinner.join();

    rclcpp::shutdown();
    return 0;
}