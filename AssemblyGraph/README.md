==============================================

# This is the module handling assembly graph

==============================================

# classes:

GraphNodeType:  the definition of the assembly graph node
GraphEdgeType:  the definition of the assembly graph edge
AssemblyGraph:  the definition of the assembly graph fundamentals (construction/destruction, IO interface, and basic operations). This class should include GraphNodeType and GraphEdge Type.
GraphPrune: a set of supported algorithms to prune or alter the topology of the graph. Inherit from the AssemblyGraph class.
GraphTraversal: a set of supported algorithms to traverse the graph. Inherit from the AssemblyGraph class.

# Folder structure:

doc: the folder where the document of the assembly graph object
reference: the folder where the materials of designing the object.
obsolute_doc: the obsolete design documents
obsolete_src: the source code from DRAGoM, some code might be reused.
