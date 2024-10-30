import behavior_tree_module as bt

# Create a factory
factory = bt.BehaviorTreeFactory()

# Register all your custom nodes
factory.registerNode("MoveToNurse")
factory.registerNode("ReceiveBarcode")
factory.registerNode("CheckBarcode")
factory.registerNode("ReleaseDrawer")
factory.registerNode("WaitDrawerClose")
factory.registerNode("MoveToLab")
factory.registerNode("MoveToInitial")

# Create the tree from your XML file
tree = factory.createTreeFromFile("./bts/tree.xml")

# Run the tree
tree.tickRootWhileRunning()
