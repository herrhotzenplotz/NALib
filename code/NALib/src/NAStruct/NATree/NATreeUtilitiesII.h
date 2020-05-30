
NA_IDEF NATree* naInitTree(NATree* tree, NATreeConfiguration* config){
  tree->config = naRetainTreeConfiguration(config);

  #ifndef NDEBUG
    if(((tree->config->flags & NA_TREE_QUADTREE) || (tree->config->flags & NA_TREE_OCTTREE)) && !tree->config->configdata)
      naError("Quadtree or Octtree configuration need more information. Use naSetTreeConfigurationBaseLeafExponent");
  #endif

  // If the config has a callback for constructing a tree, call it.
  if(tree->config->treeConstructor){
    tree->config->treeConstructor(tree->config->userdata);
  }

  // Init the tree root.
  tree->root = NA_NULL;
  tree->flags = 0;
  #ifndef NDEBUG
    tree->iterCount = 0;
  #endif

  return tree;
}



NA_IDEF void naEmptyTree(NATree* tree){
  #ifndef NDEBUG
    if(tree->iterCount != 0)
      naError("There are still iterators running on this tree. Did you miss a naClearTreeIterator call?");
  #endif
  if(tree->root){
    if(naIsTreeRootLeaf(tree)){
      naDestructTreeLeaf(tree->config, (NATreeLeaf*)tree->root);
    }else{
      naDestructTreeNode(tree->config, (NATreeNode*)tree->root, NA_TRUE);
    }
  }
  tree->root = NA_NULL;
}



NA_IDEF void naClearTree(NATree* tree){
  naEmptyTree(tree);
  // If the config has a callback function for deleting a tree, call it.
  if(tree->config->treeDestructor){
    tree->config->treeDestructor(tree->config->userdata);
  }
  naReleaseTreeConfiguration(tree->config);
}



NA_IDEF NABool naIsTreeEmpty(const NATree* tree){
  return (tree->root == NA_NULL);
}



NA_IDEF NABool naAddTreeFirstConst(NATree* tree, const void* content){
  NATreeIterator iter;
  #ifndef NDEBUG
    if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
      naError("This function should not be called on trees with keys");
  #endif
  iter = naMakeTreeModifier(tree);
  naAddTreeNextConst(&iter, content, NA_FALSE);
  naClearTreeIterator(&iter);
  return NA_TRUE;
}



NA_IDEF NABool naAddTreeFirstMutable(NATree* tree, void* content){
  NATreeIterator iter;
  #ifndef NDEBUG
    if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
      naError("This function should not be called on trees with keys");
  #endif
  iter = naMakeTreeModifier(tree);
  naAddTreeNextMutable(&iter, content, NA_FALSE);
  naClearTreeIterator(&iter);
  return NA_TRUE;
}



NA_IDEF NABool naAddTreeLastConst(NATree* tree, const void* content){
  NATreeIterator iter;
  #ifndef NDEBUG
    if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
      naError("This function should not be called on trees with keys");
  #endif
  iter = naMakeTreeModifier(tree);
  naAddTreePrevConst(&iter, content, NA_FALSE);
  naClearTreeIterator(&iter);
  return NA_TRUE;
}



NA_IDEF NABool naAddTreeLastMutable(NATree* tree, void* content){
  NATreeIterator iter;
  #ifndef NDEBUG
    if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
      naError("This function should not be called on trees with keys");
  #endif
  iter = naMakeTreeModifier(tree);
  naAddTreePrevMutable(&iter, content, NA_FALSE);
  naClearTreeIterator(&iter);
  return NA_TRUE;
}



NA_IDEF const void* naGetTreeFirstConst(const NATree* tree){
  const void* retvalue;
  NATreeIterator iter = naMakeTreeAccessor(tree);
  naLocateTreeFirst(&iter);
  retvalue = naGetTreeCurLeafConst(&iter);
  naClearTreeIterator(&iter);
  return retvalue;
}



NA_IDEF void* naGetTreeFirstMutable(const NATree* tree){
  void* retvalue;
  NATreeIterator iter = naMakeTreeAccessor(tree);
  naLocateTreeFirst(&iter);
  retvalue = naGetTreeCurLeafMutable(&iter);
  naClearTreeIterator(&iter);
  return retvalue;
}



NA_IDEF const void* naGetTreeLastConst(const NATree* tree){
  const void* retvalue;
  NATreeIterator iter = naMakeTreeAccessor(tree);
  naLocateTreeLast(&iter);
  retvalue = naGetTreeCurLeafConst(&iter);
  naClearTreeIterator(&iter);
  return retvalue;
}



NA_IDEF void* naGetTreeLastMutable(const NATree* tree){
  void* retvalue;
  NATreeIterator iter = naMakeTreeAccessor(tree);
  naLocateTreeLast(&iter);
  retvalue = naGetTreeCurLeafMutable(&iter);
  naClearTreeIterator(&iter);
  return retvalue;
}



NA_IDEF void naUpdateTree(NATree* tree){
  #ifndef NDEBUG
    if(!tree->config->nodeUpdater)
      naError("tree is configured without nodeUpdater callback");
  #endif
  if(tree->root && !naIsTreeRootLeaf(tree)){
    naUpdateTreeNodeCapturing(tree, (NATreeNode*)tree->root);
  }
}



NA_IDEF NAPtr naGetRootNodeContent(NATree* tree)
{
  NAPtr retdata;
  if(tree->root){
    #ifndef NDEBUG
      if(naIsTreeRootLeaf(tree))
        naError("Root of the tree is not a node");
    #endif
    retdata = naGetTreeNodeData(tree->config, (NATreeNode*)(tree->root));
  }else{
    retdata = naMakePtrNull();
  }
  return retdata;
}



NA_HIDEF void naSetTreeRoot(NATree* tree, NATreeItem* newroot, NABool isLeaf){
  #ifndef NDEBUG
    if(!newroot)
      naCrash("Do not send null as new root. Use naClearTreeRoot for that.");
  #endif
  tree->root = newroot;
  naMarkTreeRootLeaf(tree, isLeaf);
  newroot->parent = NA_NULL;
}



NA_HIDEF void naClearTreeRoot(NATree* tree){
  tree->root = NA_NULL;
}



NA_HIDEF NABool naIsTreeRootLeaf(const NATree* tree){
  return (NABool)((tree->flags & NA_TREE_FLAG_ROOT_IS_LEAF) == NA_TREE_FLAG_ROOT_IS_LEAF);
}



NA_HIDEF void naMarkTreeRootLeaf(NATree* tree, NABool isleaf){
  tree->flags &= ~NA_TREE_FLAG_ROOT_IS_LEAF;
  tree->flags |= (isleaf * NA_TREE_FLAG_ROOT_IS_LEAF);
}


// todo: If this shows up in performance, adding rootparent again? Or adding childindx as flag in every item?
NA_HIDEF NABool naIsTreeItemLeaf(const NATree* tree, NATreeItem* item){
  NABool retvalue;
  if(naIsTreeItemRoot(item)){
    retvalue = naIsTreeRootLeaf(tree);
  }else{
    NATreeNode* parent = naGetTreeItemParent(item);
    NAInt childindx = naGetTreeNodeChildIndex(tree->config, parent, item);
    retvalue = naIsNodeChildLeaf(parent, childindx);
  }
  return retvalue;
}



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
