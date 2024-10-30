#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/chrono.h>

#include "behaviortree_cpp_v3/bt_factory.h"
#include "behaviortree_cpp_v3/blackboard.h"

#include "nodes.cpp"

namespace py = pybind11;
using namespace BT;

void registerNode(BehaviorTreeFactory& factory, const std::string& name) {
    if (name == "MoveToNurse") {
        factory.registerNodeType<MoveToNurse>(name);
    }
    else if (name == "ReceiveBarcode") {
        factory.registerNodeType<ReceiveBarcode>(name);
    }
    else if (name == "CheckBarcode") {
        factory.registerNodeType<CheckBarcode>(name);
    }
    else if (name == "ReleaseDrawer") {
        factory.registerNodeType<ReleaseDrawer>(name);
    }
    else if (name == "WaitDrawerClose") {
        factory.registerNodeType<WaitDrawerClose>(name);
    }
    else if (name == "MoveToLab") {
        factory.registerNodeType<MoveToLab>(name);
    }
    else if (name == "MoveToInitial") {
        factory.registerNodeType<MoveToInitial>(name);
    }
}

PYBIND11_MODULE(behavior_tree_module, m) {
    // Blackboard
    py::class_<Blackboard, std::shared_ptr<Blackboard>>(m, "Blackboard")
        .def_static("create", &Blackboard::create, py::arg("parent") = nullptr)
        // Expose templated get/set methods for common types
        .def("get", static_cast<bool (Blackboard::*)(const std::string&, std::string&) const>(&Blackboard::get<std::string>))
        .def("get", static_cast<std::string (Blackboard::*)(const std::string&) const>(&Blackboard::get<std::string>))
        .def("get", static_cast<bool (Blackboard::*)(const std::string&, double&) const>(&Blackboard::get<double>))
        .def("get", static_cast<double (Blackboard::*)(const std::string&) const>(&Blackboard::get<double>))
        .def("get", static_cast<bool (Blackboard::*)(const std::string&, int&) const>(&Blackboard::get<int>))
        .def("get", static_cast<int (Blackboard::*)(const std::string&) const>(&Blackboard::get<int>))
        .def("set", &Blackboard::set<std::string>)
        .def("set", &Blackboard::set<double>)
        .def("set", &Blackboard::set<int>);

    // Expose NodeStatus enum
    py::enum_<NodeStatus>(m, "NodeStatus")
        .value("SUCCESS", NodeStatus::SUCCESS)
        .value("FAILURE", NodeStatus::FAILURE)
        .value("RUNNING", NodeStatus::RUNNING)
        .export_values();

    // Expose base classes
    py::class_<TreeNode>(m, "TreeNode");
    py::class_<SyncActionNode, TreeNode>(m, "SyncActionNode");
    py::class_<ConditionNode, TreeNode>(m, "ConditionNode");

    // Expose BehaviorTreeFactory
    py::class_<BehaviorTreeFactory>(m, "BehaviorTreeFactory")
        .def(py::init<>())
	.def("registerNode", &registerNode)
        .def("createTreeFromFile", &BehaviorTreeFactory::createTreeFromFile, py::arg("file_path"), py::arg("blackboard") = Blackboard::create())
        .def("createTreeFromText", &BehaviorTreeFactory::createTreeFromText);

    // Expose Tree
    py::class_<Tree>(m, "Tree")
        .def("tickRoot", &Tree::tickRoot)
        .def("tickRootWhileRunning", &Tree::tickRootWhileRunning, py::arg("sleep_ms") = std::chrono::milliseconds(0));

    // Expose your custom nodes
    py::class_<MoveToNurse, SyncActionNode>(m, "MoveToNurse")
        .def(py::init<const std::string &>());

    py::class_<ReceiveBarcode, SyncActionNode>(m, "ReceiveBarcode")
        .def(py::init<const std::string &, const NodeConfiguration &>());

    py::class_<CheckBarcode, ConditionNode>(m, "CheckBarcode")
        .def(py::init<const std::string &, const NodeConfiguration &>())
        .def_static("providedPorts", &CheckBarcode::providedPorts);

    py::class_<ReleaseDrawer, SyncActionNode>(m, "ReleaseDrawer")
        .def(py::init<const std::string &>());

    py::class_<WaitDrawerClose, ConditionNode>(m, "WaitDrawerClose")
        .def(py::init<const std::string &>());

    py::class_<MoveToLab, SyncActionNode>(m, "MoveToLab")
        .def(py::init<const std::string &>());

    py::class_<MoveToInitial, SyncActionNode>(m, "MoveToInitial")
        .def(py::init<const std::string &>());
}
