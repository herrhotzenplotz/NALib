
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../NAQuadTree.h"
#include "../NAMemory.h"



struct NAQuadTreeNode{
  NAInt childexponent;
  NAInt segmentinparent;
  NAQuadTreeNode* parentnode;
  void* child[4];
  NAPos childorigin[4];
  void* nodedata;
  #ifndef NDEBUG
    NAInt itercount;
  #endif
};



// ////////////////////////////
// Node
// ////////////////////////////


NA_HIDEF NARect naGetQuadTreeNodeRect(NAQuadTreeNode* node){
  return naMakeRect(node->childorigin[0], naGetQuadTreeSizeWithExponent(node->childexponent + 1));
}



NA_HDEF void naFillQuadTreeNodeChildData(const void* childdata[4], NAQuadTreeNode* node){
  childdata[0] = (node->child[0]) ? (((NAQuadTreeNode*)(node->child[0]))->nodedata) : NA_NULL;
  childdata[1] = (node->child[1]) ? (((NAQuadTreeNode*)(node->child[1]))->nodedata) : NA_NULL;
  childdata[2] = (node->child[2]) ? (((NAQuadTreeNode*)(node->child[2]))->nodedata) : NA_NULL;
  childdata[3] = (node->child[3]) ? (((NAQuadTreeNode*)(node->child[3]))->nodedata) : NA_NULL;
}



// Creates an inner node.
NA_HDEF NAQuadTreeNode* naAllocQuadTreeNode(NAInt childexponent, NAInt segmentinparent, NAQuadTreeNode* parentnode, NAPos origin, NAQuadTreeNode* dummynode, NAQuadTreeNodeAllocator allocator){
  NAQuadTreeNode* node = naAlloc(NAQuadTreeNode);
  node->childexponent = childexponent;
  node->segmentinparent = segmentinparent;
  node->parentnode = parentnode;
  node->child[0] = dummynode;
  node->child[1] = dummynode;
  node->child[2] = dummynode;
  node->child[3] = dummynode;
  NASize childsize = naGetQuadTreeSizeWithExponent(node->childexponent);
  node->childorigin[0] = naMakePos(origin.x                  , origin.y);
  node->childorigin[1] = naMakePos(origin.x + childsize.width, origin.y);
  node->childorigin[2] = naMakePos(origin.x                  , origin.y + childsize.height);
  node->childorigin[3] = naMakePos(origin.x + childsize.width, origin.y + childsize.height);
  if(allocator){
    node->nodedata = allocator(origin, childsize);
  }else{
    node->nodedata = NA_NULL;
  }
  #ifndef NDEBUG
    node->itercount = 0;
  #endif
  return node;
}



// Deallocates one node. Deallocates all subnodes and leafes in the process.
NA_HDEF void naDeallocQuadTreeNode(NAQuadTree* tree, NAQuadTreeNode* node){
  #ifndef NDEBUG
    if(node->itercount)
      naError("naDeallocQuadTreeNode", "Iterators still running on this node. Did you forget a call to NAClearQuadTreeIterator?");
  #endif

  if(node->childexponent == naGetQuadTreeMinLeafExponent(tree)){
    if(node->child[0]){tree->configuration.leafdeallocator(node->child[0], tree->configuration.userdata);}
    if(node->child[1]){tree->configuration.leafdeallocator(node->child[1], tree->configuration.userdata);}
    if(node->child[2]){tree->configuration.leafdeallocator(node->child[2], tree->configuration.userdata);}
    if(node->child[3]){tree->configuration.leafdeallocator(node->child[3], tree->configuration.userdata);}
  }else{
    if(node->child[0]){naDeallocQuadTreeNode(tree, node->child[0]);}
    if(node->child[1]){naDeallocQuadTreeNode(tree, node->child[1]);}
    if(node->child[2]){naDeallocQuadTreeNode(tree, node->child[2]);}
    if(node->child[3]){naDeallocQuadTreeNode(tree, node->child[3]);}
  }

  if(node->nodedata && tree->configuration.nodedeallocator){
    tree->configuration.nodedeallocator(node->nodedata);
  }
  naFree(node);
}



// Allocates and initializes a new and empty node which is a child of
// an existing node and denotes the given segment.
NA_HDEF NAQuadTreeNode* naAddQuadTreeNodeChild(NAQuadTree* tree, NAQuadTreeNode* parentnode, NAInt segmentinparent){
  NAQuadTreeNode* node;
  #ifndef NDEBUG
    if(parentnode->child[segmentinparent])
      naError("naAddQuadTreeNodeChild", "A child already exists at that segment of the parent node.");
  #endif

  node = naAllocQuadTreeNode(
          parentnode->childexponent - 1,
          segmentinparent,
          parentnode,
          parentnode->childorigin[segmentinparent],
          NA_NULL,
          tree->configuration.nodeallocator);
  
  parentnode->child[segmentinparent] = node;
  return node;
}



// Returns the segment index [0-3] in which the given pos can be found.
// Warning: The position is expected to be inside this node.
NA_HIDEF NAInt naGetQuadTreeNodeSegment(NAQuadTreeNode* node, NAPos pos){
  NAInt segment = 0;
  if(pos.x >= node->childorigin[3].x){segment |= 1;}
  if(pos.y >= node->childorigin[3].y){segment |= 2;}
  #ifndef NDEBUG
    if(!naContainsRectPosE(naMakeRect(node->childorigin[segment], naGetQuadTreeSizeWithExponent(node->childexponent)), pos))
      naError("naGetQuadTreeNodeSegment", "node does not actually contains pos");
  #endif
  return segment;
}



NA_HDEF NAPos naGetQuadTreeNodeParentOrigin(NAInt childexponent, NAPos childorigin){
  // In order to achieve a full coverage of the whole space
  // (negative and positive in all dimensions), we align parent nodes
  // in a cyclic way.
  //
  // Implementation note: To be sure that the childorigin stays aligned, we
  // simply take that origin als the alignment origin and work from there.

  NARect alignrect = naMakeRect(childorigin, naGetQuadTreeSizeWithExponent(childexponent + 1));
  NASize childsize = naGetQuadTreeSizeWithExponent(childexponent);
  NAInt cycle = childexponent % 4;
  if(cycle & 1){alignrect.pos.x -= childsize.width;}
  if(cycle & 2){alignrect.pos.y -= childsize.height;}

  return naMakePosWithAlignment(childorigin, alignrect);
}



// Expects the parent node of a child which has changed. The segment indicates
// which segment caused the trouble. If -1 is given, there is no particular node
// (for example because it had been removed).
NA_HDEF void naUpdateQuadTreeNodeBubbling(NAQuadTree* tree, NAQuadTreeNode* parentnode, NAInt segment){
  NABool bubble = NA_TRUE;
  #ifndef NDEBUG
    if(parentnode == NA_NULL)
      naError("naUpdateQuadTreeNodeBubbling", "Parent is null.");
  #endif
  
  NASize childsize = naGetQuadTreeSizeWithExponent(parentnode->childexponent);
  
  // We call the update callbacks depending whether this is a leaf node or
  // an inner node.
  if(parentnode->childexponent == naGetQuadTreeMinLeafExponent(tree)){
    if(tree->configuration.leafchanged){
      bubble = tree->configuration.leafchanged(parentnode->nodedata, segment, (const void* const) parentnode->child, childsize);
    }
  }else{
    if(tree->configuration.childchanged){
      const void* childdata[4];
      naFillQuadTreeNodeChildData(childdata, parentnode);
      bubble = tree->configuration.childchanged(parentnode->nodedata, segment, childdata, childsize);
    }
  }
  
  // Then we propagate the message towards the root.
  if(bubble && parentnode->parentnode){
    #ifndef NDEBUG
      if(parentnode->segmentinparent == -1)
        naError("naUpdateQuadTreeNode", "Internal inconsistency detected");
    #endif
    naUpdateQuadTreeNodeBubbling(tree, parentnode->parentnode, parentnode->segmentinparent);
  }
}



// Propagates a capturing update event from the root to the leafes.
// All leaf nodes will be called with the leafchanged callback with -1
// denoting all leafes shall be updated. These callbacks can send back a
// bubbling request which after capturing all leafes will be propagated
// upwards again so that every node in the path receives a childchanged
// message which again can define if the message shall be bubbled even
// further.
NA_HDEF NABool naUpdateQuadTreeNodeCapturing(NAQuadTree* tree, NAQuadTreeNode* node){
  NABool bubble = NA_FALSE;
  
  NASize childsize = naGetQuadTreeSizeWithExponent(node->childexponent);

  if(node->childexponent == naGetQuadTreeMinLeafExponent(tree)){
    // This node stores leafs
    if(tree->configuration.leafchanged){
      bubble = tree->configuration.leafchanged(node->nodedata, -1, (const void* const) node->child, childsize);
    }
  }else{
    // this node stores subnodes
    if(node->child[0]){bubble |= naUpdateQuadTreeNodeCapturing(tree, node->child[0]);}
    if(node->child[1]){bubble |= naUpdateQuadTreeNodeCapturing(tree, node->child[1]);}
    if(node->child[2]){bubble |= naUpdateQuadTreeNodeCapturing(tree, node->child[2]);}
    if(node->child[3]){bubble |= naUpdateQuadTreeNodeCapturing(tree, node->child[3]);}
    if(bubble && tree->configuration.childchanged){
      const void* childdata[4];
      naFillQuadTreeNodeChildData(childdata, node);
      bubble = tree->configuration.childchanged(node->nodedata, -1, childdata, childsize);
    }
  }
  
  return bubble;
}



// Expects the node to be the node to remove.
NA_HDEF void naRemoveQuadTreeNode(NAQuadTree* tree, NAQuadTreeNode* node){
  if(node->parentnode){
  
    // Remove the segment from the tree
    #ifndef NDEBUG
      if(node->segmentinparent == -1)
        naCrash("naRemoveQuadTreeNode", "Inernal inconsistency detected: Segment number is -1");
    #endif
    node->parentnode->child[node->segmentinparent] = NA_NULL;
    
    // Test if there are other childs...
    if((node->parentnode->child[0]) || (node->parentnode->child[1]) || (node->parentnode->child[2]) || (node->parentnode->child[3])){
      // If there is at least one more child, we update the parent node.
      naUpdateQuadTreeNodeBubbling(tree, node->parentnode, -1);
    }else{
      // If there are no more childs, we remove the parent.
      naRemoveQuadTreeNode(tree, node->parentnode);
    }
  }else{
    // If this node has no parent, it was the root
    tree->root = NA_NULL;
  }
  // Finally, we deallocate the node.
  naDeallocQuadTreeNode(tree, node);
}



// Allocates and initializes a new node which becomes the parent of the existing
// treeroot. This function replaces the current root with a higher-level root.
NA_HDEF void naGrowQuadTreeNodeRoot(NAQuadTree* tree){

  NAInt childsegment;
  NAQuadTreeNode* parentnode;
  NAPos parentorigin = naGetQuadTreeNodeParentOrigin(tree->root->childexponent + 1, tree->root->childorigin[0]);
  
  parentnode = naAllocQuadTreeNode(
          tree->root->childexponent + 1,
          -1,
          NA_NULL,
          parentorigin,
          NA_NULL,
          tree->configuration.nodeallocator);
  
  NASize childsize = naGetQuadTreeSizeWithExponent(parentnode->childexponent);

  childsegment = naGetQuadTreeNodeSegment(parentnode, tree->root->childorigin[0]);
  parentnode->child[childsegment] = tree->root;
  tree->root->parentnode = parentnode;
  tree->root->segmentinparent = childsegment;
  if(tree->configuration.childchanged){
    const void* childdata[4];
    naFillQuadTreeNodeChildData(childdata, parentnode);
    tree->configuration.childchanged(parentnode->nodedata, childsegment, childdata, childsize);
  }
  tree->root = parentnode;
}



// This methods checks if the root of the tree has only one child. As long
// as there is only one child in the root, the root is transferred to that
// very child to save memory and allow garbage collectors to collect.
NA_HDEF void naShrinkQuadTreeNodeRoot(NAQuadTree* tree){
  while(1){
    if(tree->root->childexponent == naGetQuadTreeMinLeafExponent(tree)){
      // The root node has only leafes as childs.
      return;
    }else{
      // We count how many children there are.
      NAUInt childcount = 0;
      NAInt lastchildindex = -1;
      NAQuadTreeNode* lastchild;
      if(tree->root->child[0]){childcount++; lastchildindex = 0;}
      if(tree->root->child[1]){childcount++; lastchildindex = 1;}
      if(tree->root->child[2]){childcount++; lastchildindex = 2;}
      if(tree->root->child[3]){childcount++; lastchildindex = 3;}
      #ifndef NDEBUG
        if(!childcount)
          naError("naShrinkQuadTreeNodeRoot", "Root should not be empty.");
      #endif
      // If there are more than 1 child, return.
      if(childcount != 1){return;}  // note that it should be >1 but code sanity checks think 0 can happen as well.
      lastchild = tree->root->child[lastchildindex];
      // Now, the root has only one child node which now is stored in lastchild.
      // We adjust the child to become the new root.
      lastchild->parentnode = NA_NULL;
      lastchild->segmentinparent = -1;
      // We remove the child from the current root
      tree->root->child[lastchildindex] = NA_NULL;
      // ... deallocate the current root
      naDeallocQuadTreeNode(tree, tree->root);
      // ...and set the new root.
      tree->root = lastchild;
    }
  }
}








// ////////////////////////////
// Iterator
// ////////////////////////////


// Debugging flags
#define NA_QUADTREE_ITERATOR_MUTATOR      0x01
#define NA_QUADTREE_ITERATOR_MODIFIER     0x02
#define NA_QUADTREE_ITERATOR_HAS_ORIGIN   0x04


// Checks whether the iterator is positioned at a leaf and it exists.
NA_HIDEF NABool naIsQuadTreeIteratorAtLeaf(NAQuadTreeIterator* iter){
  const NAQuadTree* tree = naGetPtrConst(&(iter->tree));
  return (iter->curnode) && (iter->cursegment != -1) && (iter->curnode->child[iter->cursegment]) && (iter->curnode->childexponent == naGetQuadTreeMinLeafExponent(tree));
}



NA_HIDEF void naSetQuadTreeIteratorLeafOrigin(NAQuadTreeIterator* iter, NAPos origin){
  iter->leaforigin = origin;
  #ifndef NDEBUG
    iter->flags |= NA_QUADTREE_ITERATOR_HAS_ORIGIN;
  #endif
}



NA_HIDEF void naSetQuadTreeIteratorCurNode(NAQuadTreeIterator* iter, NAQuadTreeNode* newnode){
  #ifndef NDEBUG
    if(iter->curnode){iter->curnode->itercount--;}
  #endif
  iter->curnode = newnode;
  #ifndef NDEBUG
    if(iter->curnode){iter->curnode->itercount++;}
  #endif
}



NA_HIDEF void naCreateQuadTreeLeaf(NAQuadTreeIterator* iter, const void* data){
  NAQuadTree* tree;

  #ifndef NDEBUG
    if(!(iter->flags & NA_QUADTREE_ITERATOR_MODIFIER))
      naError("naCreateQuadTreeLeaf", "iterator must be modifier");
    if(!(iter->flags & NA_QUADTREE_ITERATOR_HAS_ORIGIN))
      naError("naCreateQuadTreeLeaf", "origin required for leaf creation");
  #endif

  tree = (NAQuadTree*)naGetPtrMutable(&(iter->tree));

  if(!iter->curnode){
    // The tree root does not contain the coord or does not exist at all.
    if(tree->root){
      NARect noderect;
      // We expand the tree at the root and call this function recursively.
      naGrowQuadTreeNodeRoot(tree);
      noderect = naGetQuadTreeNodeRect(tree->root);
      if(naContainsRectPosE(noderect, iter->leaforigin)){naSetQuadTreeIteratorCurNode(iter, tree->root);}
    }else{
      // We create the very first node of this tree.
      NAInt minleafexponent = naGetQuadTreeMinLeafExponent(tree);
      NAPos rootorigin = naGetQuadTreeNodeParentOrigin(minleafexponent, iter->leaforigin);
      tree->root = naAllocQuadTreeNode(
                                        minleafexponent,
                                        -1,
                                        NA_NULL,
                                        rootorigin,
                                        NA_NULL,
                                        tree->configuration.nodeallocator);
      naSetQuadTreeIteratorCurNode(iter, tree->root);
    }
    // We call this function recursively.
    naCreateQuadTreeLeaf(iter, data);
    
  }else{
    NAInt segment;
    NAInt minleafexponent;

    // We have a current node which is expected to contain the coord.
    #ifndef NDEBUG
      NARect noderect = naGetQuadTreeNodeRect(iter->curnode);
      if(!naContainsRectPosE(noderect, iter->leaforigin))
        naError("naCreateQuadTreeLeaf", "Cur node does not contain the desired origin.");
    #endif
        
    // There is a current node but no subnode has been identified.
    segment = naGetQuadTreeNodeSegment(iter->curnode, iter->leaforigin);
    #ifndef NDEBUG
      if(iter->curnode->child[segment])
        naError("naCreateQuadTreeLeaf", "Tree has been modified between positioning this iterator and creating a leaf. Don't do that. Memory leaks and major pointer confusion expected.");
    #endif

    minleafexponent = naGetQuadTreeMinLeafExponent(tree);
    if(iter->curnode->childexponent == minleafexponent){
      // The node can not be divided anymore. Create a leaf.
      NARect leafrect = naMakeRect(iter->curnode->childorigin[segment], naGetQuadTreeSizeWithExponent(minleafexponent));
      iter->curnode->child[segment] = tree->configuration.leafallocator(leafrect, tree->configuration.userdata, data);
      iter->cursegment = segment;
      // Good ending. We created the leaf.
    }else{
      // We create an internal subnode and call this function recursively.
      NAQuadTreeNode* newnode = naAddQuadTreeNodeChild(tree, iter->curnode, segment);
      naSetQuadTreeIteratorCurNode(iter, newnode);
      naCreateQuadTreeLeaf(iter, data);
    }    
  }
}



NA_HDEF void* naGetQuadTreeCurMutableWithData(NAQuadTreeIterator* iter, NABool create, const void* data){
  #ifndef NDEBUG
    if(!(iter->flags & NA_QUADTREE_ITERATOR_MUTATOR))
      naError("naGetQuadTreeCurMutable", "Trying to mutate element with iterator which was created non-mutable");
  #endif
  if(naIsQuadTreeIteratorAtLeaf(iter)){
    return iter->curnode->child[iter->cursegment];
  }else{
    if(create){
      naCreateQuadTreeLeaf(iter, data);
      return iter->curnode->child[iter->cursegment];
    }else{
      return NA_NULL;
    }
  }
}



NA_HIDEF void naInitQuadTreeIterator(NAQuadTreeIterator* iter){
  #ifndef NDEBUG
    NAQuadTree* mutabletree = (NAQuadTree*)naGetPtrConst(&(iter->tree));
    mutabletree->itercount++;
  #endif
  iter->curnode = NA_NULL;
  iter->cursegment= -1;
  #ifndef NDEBUG
    iter->flags = 0;
  #endif
}



NA_DEF NAQuadTreeIterator naMakeQuadTreeAccessor(const NAQuadTree* tree){
  NAQuadTreeIterator iter;
  iter.tree = naMakePtrWithDataConst(tree);
  naInitQuadTreeIterator(&iter);
  return iter;
}



NA_DEF NAQuadTreeIterator naMakeQuadTreeMutator(const NAQuadTree* tree){
  NAQuadTreeIterator iter;
  iter.tree = naMakePtrWithDataConst(tree);
  naInitQuadTreeIterator(&iter);
  #ifndef NDEBUG
    iter.flags |= NA_QUADTREE_ITERATOR_MUTATOR;
  #endif
  return iter;
}



NA_DEF NAQuadTreeIterator naMakeQuadTreeModifier(NAQuadTree* tree){
  NAQuadTreeIterator iter;
  iter.tree = naMakePtrWithDataMutable(tree);
  naInitQuadTreeIterator(&iter);
  #ifndef NDEBUG
    iter.flags |= NA_QUADTREE_ITERATOR_MUTATOR;
    iter.flags |= NA_QUADTREE_ITERATOR_MODIFIER;
  #endif
  return iter;
}



NA_DEF void naClearQuadTreeIterator(NAQuadTreeIterator* iter){
  #ifndef NDEBUG
    NAQuadTree* mutabletree = (NAQuadTree*)naGetPtrConst(&(iter->tree));
    mutabletree->itercount--;
    if(iter->curnode){iter->curnode->itercount--;}
  #endif
  naCleanupPtr(&(iter->tree), NA_NULL);
}



NA_DEF void naResetQuadTreeIterator(NAQuadTreeIterator* iter){
  naSetQuadTreeIteratorCurNode(iter, NA_NULL);
  iter->cursegment = -1;
  #ifndef NDEBUG
    iter->flags &= ~NA_QUADTREE_ITERATOR_HAS_ORIGIN;
  #endif
}



NA_DEF const void* naGetQuadTreeCurConst(NAQuadTreeIterator* iter){
  if(naIsQuadTreeIteratorAtLeaf(iter)){
    return iter->curnode->child[iter->cursegment];
  }else{
    return NA_NULL;
  }
}



NA_DEF void* naGetQuadTreeCurMutable(NAQuadTreeIterator* iter, NABool create){
  return naGetQuadTreeCurMutableWithData(iter, create, NA_NULL);
}



NA_DEF NAPos naGetQuadTreeCurOrigin(const NAQuadTreeIterator* iter){
  #ifndef NDEBUG
    if(!(iter->flags & NA_QUADTREE_ITERATOR_HAS_ORIGIN))
      naError("naGetQuadTreeCurOrigin", "Iterator has no origin");
  #endif
  return iter->leaforigin;
}



NA_DEF NARect naGetQuadTreeCurRect(const NAQuadTreeIterator* iter){
  const NAQuadTree* tree = naGetPtrConst(&(iter->tree));
  #ifndef NDEBUG
    if(!(iter->flags & NA_QUADTREE_ITERATOR_HAS_ORIGIN))
      naError("naGetQuadTreeCurRect", "Iterator has no origin");
  #endif
  return naMakeRect(iter->leaforigin, naGetQuadTreeSizeWithExponent(naGetQuadTreeMinLeafExponent(tree)));
}



NA_DEF NARecti naGetQuadTreeCurRecti(const NAQuadTreeIterator* iter){
  const NAQuadTree* tree = naGetPtrConst(&(iter->tree));
  #ifndef NDEBUG
    if(!(iter->flags & NA_QUADTREE_ITERATOR_HAS_ORIGIN))
      naError("naGetQuadTreeCurRecti", "Iterator has no origin");
    if(naGetQuadTreeMinLeafExponent(tree) < 0)
      naError("naGetQuadTreeCurRecti", "Can not return valid integer rect for trees with exponents < 0");
  #endif
  // todo
  NASize size = naGetQuadTreeSizeWithExponent(naGetQuadTreeMinLeafExponent(tree));
  return naMakeRectiSE((NAInt)naRound(iter->leaforigin.x), (NAInt)naRound(iter->leaforigin.y), (NAInt)naRound(size.width), (NAInt)naRound(size.height));
}



NA_DEF void naRemoveQuadTreeCur(NAQuadTreeIterator* iter){
  NAQuadTree* tree;
  if(!iter->curnode){return;}
  tree = naGetPtrMutable(&(iter->tree));
    
  // First, we destroy the data of the leaf chunk
  tree->configuration.leafdeallocator(iter->curnode->child[iter->cursegment], tree->configuration.userdata);
  iter->curnode->child[iter->cursegment] = NA_NULL;

  // Test if there are other childs in the current parent.
  if((iter->curnode->child[0]) || (iter->curnode->child[1]) || (iter->curnode->child[2]) || (iter->curnode->child[3])){
    // If there is at least one more child, we update the node.
    naUpdateQuadTreeNodeBubbling(tree, iter->curnode, -1);
  }else{
    // If there are no more childs, we remove the node.
    NAQuadTreeNode* delnode = iter->curnode;
    naSetQuadTreeIteratorCurNode(iter, NA_NULL);
    naRemoveQuadTreeNode(tree, delnode);
  }
  if(tree->root){
    // Now, the root may have only one child left, so we check if we can adjust
    // the tree accordingly.
    naShrinkQuadTreeNodeRoot(tree);
  }
}



// Moves the iterator to the closest parent node containing coord.
NA_HDEF NABool naLocateQuadTreeNode(NAQuadTreeIterator* iter, NAPos origin){
  NARect noderect;
  const NAQuadTree* tree = naGetPtrConst(&(iter->tree));
  if(!tree->root){return NA_FALSE;}
  if(!iter->curnode){
    naSetQuadTreeIteratorCurNode(iter, tree->root);
    #ifndef NDEBUG
      if(!iter->curnode)
      naCrash("naLocateQuadTreeNode", "No current node after setting the current node to the root");
    #endif
  }

  noderect = naGetQuadTreeNodeRect(iter->curnode);
  if(naContainsRectPosE(noderect, origin)){
  
    // The coord is stored somewhere inside the rectangle of this node
    NAInt segment = naGetQuadTreeNodeSegment(iter->curnode, origin);
    if((iter->curnode->child[segment])){
      // There is a segment available which contains the coord.
      if(iter->curnode->childexponent == naGetQuadTreeMinLeafExponent(tree)){
        // We are at leaf-level. We found it!
        iter->cursegment = segment;
        // Good ending.
        return NA_TRUE;
      }else{
        // Go on searching in the sub-node
        naSetQuadTreeIteratorCurNode(iter, iter->curnode->child[segment]);
        return naLocateQuadTreeNode(iter, origin);
      }
      
    }else{
      // There is no segment where the coord should be. No leaf found.
      iter->cursegment = -1;
      // Bad ending. We are stuck with the closest parent node.
      return NA_FALSE;
    }

  }else{
  
    // The coord is not stored within the rect of this node.
    if(iter->curnode->parentnode){
      // We have a parent. Search there.
      naSetQuadTreeIteratorCurNode(iter, iter->curnode->parentnode);
      return naLocateQuadTreeNode(iter, origin);
    }else{
      // There is no parent
      naSetQuadTreeIteratorCurNode(iter, NA_NULL);
      iter->cursegment = -1;
      // Bad ending. The desired coord lies outside of the whole tree.
      return NA_FALSE;
    }
  }
}



NA_DEF NABool naIterateQuadTree(NAQuadTreeIterator* iter, const NARect* limit){
  const NAQuadTree* tree = (const NAQuadTree*)naGetPtrConst(&(iter->tree));
  
  if(!tree->root){return NA_FALSE;}
  if(!iter->curnode){
    naSetQuadTreeIteratorCurNode(iter, tree->root);
    #ifndef NDEBUG
      if(!iter->curnode)
      naCrash("naIterateQuadTree", "No current node after setting the current node to the root");
    #endif
  }

  // We go to the next segment. When we came to this function with segment -1,
  // we therefore start with the first segment.
  iter->cursegment++;
  
  // Search for a segment which is available.
  while(iter->cursegment < 4){
    if(iter->curnode->child[iter->cursegment]){
      // We have a child which is present and overlaps with the limit if
      // available.
      break;
    }
    iter->cursegment++;
  }
  
  if(iter->cursegment < 4){
    // There is a segment available, either use the given leaf or go
    // downwards if it is an inner node.
    if(iter->curnode->childexponent == naGetQuadTreeMinLeafExponent(tree)){
      naSetQuadTreeIteratorLeafOrigin(iter, iter->curnode->childorigin[iter->cursegment]);
      return NA_TRUE;
    }else{
      // This is an inner node. We move downwards.
      naSetQuadTreeIteratorCurNode(iter, iter->curnode->child[iter->cursegment]);
      // We start looking in the subnode from the start.
      iter->cursegment = -1;
      return naIterateQuadTree(iter, limit);
    }
    
  }else{
    // There is no more segment available in this node. Go upwards.
    if(iter->curnode->parentnode){
      #ifndef NDEBUG
        if(iter->curnode->segmentinparent == -1)
          naError("naIterateQuadTreeNode", "Inernal inconsistency detected: Segment in parent should not be -1");
      #endif
      iter->cursegment = iter->curnode->segmentinparent;
      naSetQuadTreeIteratorCurNode(iter, iter->curnode->parentnode);
      return naIterateQuadTree(iter, limit);
    }else{
      // There is no parent node. This is the root and there are no more
      // elements to be iterated.
      naResetQuadTreeIterator(iter);
      return NA_FALSE;
    }
  }
}



NA_DEF NABool naLocateQuadTreeCoord(NAQuadTreeIterator* iter, NAPos coord){
  const NAQuadTree* tree = naGetPtrConst(&(iter->tree));

  NAPos neworigin = naGetQuadTreeAlignedCoord(naGetQuadTreeMinLeafExponent(tree), coord);

  naSetQuadTreeIteratorLeafOrigin(iter, neworigin);  
  return naLocateQuadTreeNode(iter, neworigin);
}



NA_DEF NABool naLocateQuadTreeIterator(NAQuadTreeIterator* dstiter, const NAQuadTreeIterator* srciter){
  NAPos srcpos = naGetQuadTreeCurOrigin(srciter);
  return naLocateQuadTreeCoord(dstiter, srcpos);
}



NA_DEF NABool naLocateQuadTreeSteps(NAQuadTreeIterator* iter, NAInt stepx, NAInt stepy){
  NAPos neworigin;
  const NAQuadTree* tree = naGetPtrConst(&(iter->tree));
  #ifndef NDEBUG
    if(!(iter->flags & NA_QUADTREE_ITERATOR_HAS_ORIGIN))
      naError("naLocateQuadTreeSteps", "Iterator must be positioned first before moving relative.");
  #endif

  NASize minleafsize = naGetQuadTreeSizeWithExponent(naGetQuadTreeMinLeafExponent(tree));
  neworigin = iter->leaforigin;
  neworigin.x += stepx * minleafsize.width;
  neworigin.y += stepy * minleafsize.height;

  naSetQuadTreeIteratorLeafOrigin(iter, neworigin);
  return naLocateQuadTreeNode(iter, neworigin);
}



NA_DEF void naUpdateQuadTreeCur(NAQuadTreeIterator* iter){
  if(iter->curnode){
    NAQuadTree* tree = naGetPtrMutable(&(iter->tree));
    naUpdateQuadTreeNodeBubbling(tree, iter->curnode, iter->cursegment);
  }
}



NA_DEF void naUpdateQuadTree(NAQuadTree* tree){
  if(tree->root){
    naUpdateQuadTreeNodeCapturing(tree, tree->root);
  }
}








// ////////////////////////////
// Tree
// ////////////////////////////


NA_DEF NAQuadTree* naInitQuadTree(NAQuadTree* tree, NAQuadTreeConfiguration configuration){
  #ifndef NDEBUG
    if(!tree)
      naCrash("naInitQuadTree", "tree is Null-Pointer");
    if(!configuration.leafallocator)
      naError("naInitQuadTree", "Must have a data allocator");
    if(!configuration.leafdeallocator)
      naError("naInitQuadTree", "Must have a data destructor");
  #endif
  tree->configuration = configuration;
  tree->root = NA_NULL;
  #ifndef NDEBUG
    tree->itercount = 0;
  #endif
  return tree;
}



NA_DEF NAQuadTree* naInitQuadTreeCopy(NAQuadTree* newtree, const NAQuadTree* duptree){
  NAQuadTreeIterator newiter;
  NAQuadTreeIterator dupiter;
  newtree = naInitQuadTree(newtree, duptree->configuration);

  newiter = naMakeQuadTreeModifier(newtree);
  dupiter = naMakeQuadTreeAccessor(duptree);
  while(naIterateQuadTree(&dupiter, NA_NULL)){
    NAPos duporigin;
    const void* dupdata = naGetQuadTreeCurConst(&dupiter);
    duporigin = naGetQuadTreeCurOrigin(&dupiter);
    naLocateQuadTreeCoord(&newiter, duporigin);
    naCreateQuadTreeLeaf(&newiter, dupdata);
  }
  
  naClearQuadTreeIterator(&dupiter);
  naClearQuadTreeIterator(&newiter);

  return newtree;
}



NA_DEF NAQuadTree* naInitQuadTreeCopyShifted(NAQuadTree* newtree, const NAQuadTree* duptree, NAPos shift){
  double x1bound;
  double y1bound;
  double x2bound;
  double y2bound;
  NARect rect0;
  NARect rect1;
  NARect rect2;
  NARect rect3;
  NAQuadTreeIterator newiter;
  NAQuadTreeIterator iter;
  NASize minleafsize;
  
  #ifndef NDEBUG
    if(!newtree)
      naCrash("naCopyQuadTreeWithShift", "newtree is Null-Pointer");
    if(!duptree->configuration.datacopier)
      naCrash("naCopyQuadTreeWithShift", "Data copier callback required for this function to work.");
  #endif
  newtree = naInitQuadTree(newtree, duptree->configuration);
  if(!duptree->root){return newtree;}

  // Create four rects which denote the rects in the new shifted tree which
  // are aligned to a leaflength.
  minleafsize = naGetQuadTreeSizeWithExponent(naGetQuadTreeMinLeafExponent(duptree));

  x1bound = naMod(shift.x, minleafsize.width);
  y1bound = naMod(shift.y, minleafsize.height);
  x2bound = minleafsize.width - x1bound;
  y2bound = minleafsize.height - y1bound;
  rect0 = naMakeRectE(naMakePos(shift.x, shift.y), naMakeSizeE(x2bound, y2bound));
  rect1 = naMakeRectE(naMakePos(shift.x + x2bound, shift.y), naMakeSizeE(x1bound, y2bound));
  rect2 = naMakeRectE(naMakePos(shift.x, shift.y + y2bound), naMakeSizeE(x2bound, y1bound));
  rect3 = naMakeRectE(naMakePos(shift.x + x2bound, shift.y + y2bound), naMakeSizeE(x1bound, y1bound));
  
  newiter = naMakeQuadTreeModifier(newtree);
  iter = naMakeQuadTreeAccessor(duptree);
  
  while(naIterateQuadTree(&iter, NA_NULL)){
    NAPos origin;
    NAPos neworigin;
    void* newdata;
    const void* dupchunk = naGetQuadTreeCurConst(&iter);
    origin = naGetQuadTreeCurOrigin(&iter);

    // We have a leaf with data. Now, we create all leafes in the new tree
    // containing the shifted leaf. There are max 4 new leaves.
    if(!naIsRectEmpty(rect0)){
      neworigin = naMakePos(origin.x + rect0.pos.x, origin.y + rect0.pos.y);
      naLocateQuadTreeCoord(&newiter, neworigin);
      newdata = naGetQuadTreeCurMutable(&newiter, NA_TRUE);
      duptree->configuration.datacopier(newdata, naMakePos(x1bound, y1bound), dupchunk, naMakePos(0, 0), rect0.size, shift, minleafsize);
    }
    if(!naIsRectEmpty(rect1)){
      neworigin = naMakePos(origin.x + rect1.pos.x, origin.y + rect1.pos.y);
      naLocateQuadTreeCoord(&newiter, neworigin);
      newdata = naGetQuadTreeCurMutable(&newiter, NA_TRUE);
      duptree->configuration.datacopier(newdata, naMakePos(0, y1bound), dupchunk, naMakePos(x2bound, 0), rect1.size, shift, minleafsize);
    }
    if(!naIsRectEmpty(rect2)){
      neworigin = naMakePos(origin.x + rect2.pos.x, origin.y + rect2.pos.y);
      naLocateQuadTreeCoord(&newiter, neworigin);
      newdata = naGetQuadTreeCurMutable(&newiter, NA_TRUE);
      duptree->configuration.datacopier(newdata, naMakePos(x1bound, 0), dupchunk, naMakePos(0, y2bound), rect2.size, shift, minleafsize);
    }
    if(!naIsRectEmpty(rect3)){
      neworigin = naMakePos(origin.x + rect3.pos.x, origin.y + rect3.pos.y);
      naLocateQuadTreeCoord(&newiter, neworigin);
      newdata = naGetQuadTreeCurMutable(&newiter, NA_TRUE);
      duptree->configuration.datacopier(newdata, naMakePos(0, 0), dupchunk, naMakePos(x2bound, y2bound), rect3.size, shift, minleafsize);
    }
  }
  naClearQuadTreeIterator(&iter);
  naClearQuadTreeIterator(&newiter);
  
  // Now, all the data has been copied. Update the whole tree.
  naUpdateQuadTree(newtree);
  return newtree;
}



NA_DEF void naClearQuadTree(NAQuadTree* tree){
  naEmptyQuadTree(tree);
}




NA_DEF void naEmptyQuadTree(NAQuadTree* tree){
  #ifndef NDEBUG
    if(tree->itercount != 0)
      naError("naEmptyQuadTree", "There are still iterators running on this tree. Did you miss a naClearQuadTreeIterator call?");
  #endif
  if(tree->root){naDeallocQuadTreeNode(tree, tree->root);}
  tree->root = NA_NULL;
}



NA_DEF void* naGetQuadTreeRootNodeData(NAQuadTree* tree){
  if(tree->root){return tree->root->nodedata;}
  return NA_NULL;
}






// Copyright (c) NALib, Tobias Stamm
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
