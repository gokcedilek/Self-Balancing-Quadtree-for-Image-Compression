/**
 *
 * Balanced Quad Tree (pa3)
 *
 * This file will be used for grading.
 *
 */

#include "QTree.h"

// Return the biggest power of 2 less than or equal to n
int biggestPow2(int n) {
  if( n < 1 ) return 0;
  int v = 1;
  while( v <= n ) v <<= 1;
  return v >> 1;
}

QTree::Node::Node(PNG & im, pair<int,int> ul, int sz, Node *par)
  :upLeft(ul),size(sz),parent(par),nw(NULL),ne(NULL),sw(NULL),se(NULL)
{
  var = varAndAvg(im,ul,size,avg); //calculate the color variability for a pixel using its RGB and the average RGB of its neighbours
}

QTree::~QTree(){
  clear(); //deallocate all the dynamic (heap) memory associated with a quadtree object
}

QTree::QTree(const QTree & other) {
  copy(other);
}

QTree & QTree::operator=(const QTree & rhs){
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}


QTree::QTree(PNG & imIn, int leafB, RGBAPixel frameC, bool bal)
  : leafBound(leafB), balanced(bal), drawFrame(true), frameColor(frameC)
{
  im = imIn;
  pair<int,int> rx_ry(0,0);
  numLeaf = 1; //before any split, num leaves = 1 (the whole image)
  root = new Node(im, rx_ry, biggestPow2(im.width()), NULL);
  split(root); //recursively perform node splitting to compress the image
}


QTree::QTree(PNG & imIn, int leafB, bool bal)
  : leafBound(leafB), balanced(bal), drawFrame(false)
{
  //no frameColor version of the one above
  im = imIn;
  pair<int,int> rx_ry(0,0);
  numLeaf = 1;
  root = new Node(im, rx_ry, biggestPow2(im.width()), NULL);
  split(root);
}


bool QTree::isLeaf( Node *t ) {
  
  //check if a node has children (a node has either 0 or 4 children)
  if(t == NULL) {
    return false;
  }
  return (t->nw == NULL);
}
  
void QTree::split( Node *t ) {
  
  // FOR BALANCED QTREES-------------------------------------------------
  // A split might cause one or two nbrs of the parent of t to split
  // to maintain balance.  Note that these two nbrs exist (unless they're
  // not in the image region) because the current set of leaves are
  // balanced.
  // if( t is a NW (or NE or SW or SE) child ) then we need to check that
  // the North and West (or North and East or South and West or
  // South and East) nbrs of t->parent have children. If they don't
  // we need to split them.

  priority_queue<QTree::Node*, vector<QTree::Node*>, CmpNodes> nodePQ; //PQ that holds all the leaf nodes in the image, and removes, upon requested, the node with the highest variance

  if(balanced == false){
    splitRecursiveImb(t, nodePQ); //recursive imbalanced splitting
  } else {
    splitRecursiveBal(t, nodePQ); //recursive balanced splitting
  }  
}

//helper function for balanced splitting
bool QTree::needsSplit(Node* t, Node* neighbour){
  if(t == NULL || neighbour == NULL) return false;
  int childSize;
  if(isLeaf(t) == true){
    childSize = t->size; //compare with the smallest size
  } else {
    childSize = t->size/2; //compare with the smallest size
  }
  /*the above distinction (the if-else condition to determine the childSize) is NEEDED because!!:
  a leaf node and its parent will have the same neighbour if that neighbour node does not have children (is a leaf)
  we have to compare the smallest existing node length with the neighbour to correctly identify the need for a rebalance */
  if( (childSize == neighbour->size) || (childSize == (neighbour->size / 2)) || (childSize == (neighbour->size * 2)) ){
    return false; //neighbour size is not 'significantly' different, don't split the neighbour yet
  } 
  return true;
}

//coordinate checks to determine NW/NE/SW/SE of parent
bool QTree::isNorthWestChild(Node* t){
  return ( (t->upLeft.first == t->parent->upLeft.first) && (t->upLeft.second == t->parent->upLeft.second) );
}

bool QTree::isNorthEastChild(Node* t){
  return ( (t->upLeft.first == t->parent->upLeft.first + t->size) && (t->upLeft.second == t->parent->upLeft.second) );
}

bool QTree::isSouthWestChild(Node* t){
  return ( (t->upLeft.first == t->parent->upLeft.first) && (t->upLeft.second == t->parent->upLeft.second + t->size) );  
}

bool QTree::isSouthEastChild(Node* t){
  return ( (t->upLeft.first == t->parent->upLeft.first + t->size) && (t->upLeft.second == t->parent->upLeft.second + t->size) );
}

//non-recursive helper function for balanced split, which simply splits a given node and does nothing else
//why? because maintainBalance() needs a function that will run non-recursively (once) to split a given node (for example, neighbours of a node to maintain the balance)
//because maintainBalance() itself is recursive, it needs this non-rec helper to do a single split of a given node
void QTree::balanceSplit(Node *t, priority_queue<QTree::Node*, vector<QTree::Node*>, CmpNodes>& nodePQ){
  pair<int,int> nw_pair(t->upLeft.first, t->upLeft.second);
  pair<int,int> ne_pair(t->upLeft.first + t->size/2, t->upLeft.second);
  pair<int,int> sw_pair(t->upLeft.first, t->upLeft.second + t->size/2);
  pair<int,int> se_pair(t->upLeft.first + t->size/2, t->upLeft.second + t->size/2);

  t->nw = new Node(im, nw_pair, t->size/2, t);
  t->ne = new Node(im, ne_pair, t->size/2, t);
  t->sw = new Node(im, sw_pair, t->size/2, t);
  t->se = new Node(im, se_pair, t->size/2, t);

  nodePQ.push(t->nw);
  nodePQ.push(t->ne);
  nodePQ.push(t->sw);
  nodePQ.push(t->se);

  numLeaf += 3;  
}

//recursive function to check if a given node's neighbours (2 of them - NW/NE/SW/SE) need to be split to maintain the balance of the qtree
//if any neighbour needs to be split; split it (balanceSplit()), and call maintainBalance on that node - why?
//because the concept of "maintaining balance" itself is RECURSIVE!!! a split may cause other splits which may cause other splits...
//when to stop? when no other neighbour needs split
void QTree::maintainBalance(Node *t, priority_queue<QTree::Node*, vector<QTree::Node*>, CmpNodes>& nodePQ){
  bool split = false;

  if(t->parent != NULL){
    if(isNorthWestChild(t)){
      Node* nnbr = NNbr(t);
      split = needsSplit(t, nnbr);
      if(split) {
        balanceSplit(nnbr, nodePQ);
        maintainBalance(nnbr, nodePQ); 
      }
      Node* wnbr = WNbr(t);
      split = needsSplit(t, wnbr);
      if(split) { 
        balanceSplit(wnbr, nodePQ);
        maintainBalance(wnbr, nodePQ); 
      }
    }
    else if(isNorthEastChild(t)){
      Node* nnbr = NNbr(t);
      split = needsSplit(t, nnbr);
      if(split) { 
        balanceSplit(nnbr, nodePQ);
        maintainBalance(nnbr, nodePQ); 
      }
      Node* enbr = ENbr(t);
      split = needsSplit(t, enbr);
      if(split) { 
        balanceSplit(enbr, nodePQ);
        maintainBalance(enbr, nodePQ);
      }
    }
    else if(isSouthWestChild(t)){
      Node* snbr = SNbr(t);
      split = needsSplit(t, snbr);
      if(split) { 
        balanceSplit(snbr, nodePQ); 
        maintainBalance(snbr, nodePQ);
      }
      Node* wnbr = WNbr(t);
      split = needsSplit(t, wnbr);
      if(split) { 
        balanceSplit(wnbr, nodePQ); 
        maintainBalance(wnbr, nodePQ);
      }
    }
    else if(isSouthEastChild(t)){
      Node* snbr = SNbr(t);
      split = needsSplit(t, snbr);
      if(split) { 
        balanceSplit(snbr, nodePQ); 
        maintainBalance(snbr, nodePQ);
      }
      Node* enbr = ENbr(t);
      split = needsSplit(t, enbr);
      if(split) { 
        balanceSplit(enbr, nodePQ); 
        maintainBalance(enbr, nodePQ);
      }
    }
  }
}

void QTree::splitRecursiveBal(Node *t, priority_queue<QTree::Node*, vector<QTree::Node*>, CmpNodes>& nodePQ){
  //if(t->size <= 1) return;
  if(numLeaf < leafBound){
    if(isLeaf(t) == true){ //we need this check right here! why? (this correction is the line that made my code pass the gradescope tests!)
      pair<int,int> nw_pair(t->upLeft.first, t->upLeft.second);
      pair<int,int> ne_pair(t->upLeft.first + t->size/2, t->upLeft.second);
      pair<int,int> sw_pair(t->upLeft.first, t->upLeft.second + t->size/2);
      pair<int,int> se_pair(t->upLeft.first + t->size/2, t->upLeft.second + t->size/2);
      
      t->nw = new Node(im, nw_pair, t->size/2, t);
      t->ne = new Node(im, ne_pair, t->size/2, t);
      t->sw = new Node(im, sw_pair, t->size/2, t);
      t->se = new Node(im, se_pair, t->size/2, t);

      nodePQ.push(t->nw);
      nodePQ.push(t->ne);
      nodePQ.push(t->sw);
      nodePQ.push(t->se);

      numLeaf += 3;

      maintainBalance(t, nodePQ); //note this is different than the imbalanced version - look at the function for info!
    }
    //well, we need that line because, a node in the PQ that we pop to split may have already been split while balancing the quadtree
    //if this is the case, ignore that node and simply pop the next node from the PQ!  
    Node* next = nodePQ.top();
    nodePQ.pop();
    splitRecursiveBal(next, nodePQ);
  }
}

void QTree::splitRecursiveImb(Node *t, priority_queue<QTree::Node*, vector<QTree::Node*>, CmpNodes>& nodePQ){
  //if(t->size <= 1) return;
  if(numLeaf < leafBound){
    pair<int,int> nw_pair(t->upLeft.first, t->upLeft.second);
    pair<int,int> ne_pair(t->upLeft.first + t->size/2, t->upLeft.second);
    pair<int,int> sw_pair(t->upLeft.first, t->upLeft.second + t->size/2);
    pair<int,int> se_pair(t->upLeft.first + t->size/2, t->upLeft.second + t->size/2);

    t->nw = new Node(im, nw_pair, t->size/2, t);
    t->ne = new Node(im, ne_pair, t->size/2, t);
    t->sw = new Node(im, sw_pair, t->size/2, t);
    t->se = new Node(im, se_pair, t->size/2, t);

    nodePQ.push(t->nw);
    nodePQ.push(t->ne);
    nodePQ.push(t->sw);
    nodePQ.push(t->se);

    //1 leaf removed, 4 leaves added
    numLeaf += 3;

    Node* next = nodePQ.top(); //find the leaf node with the highest variance (= largest sum of color differences between a pixel in the node and the average of all pixels in that node)
    nodePQ.pop(); //remove - the node that will be split next
    splitRecursiveImb(next, nodePQ);
  }
}


/* NNbr(t)
 * return the same-sized quad tree node that is north of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::NNbr(Node *t) {

  if(t == NULL) return NULL; 
  else if(t->parent == NULL) return NULL;
  else if(isSouthEastChild(t)) return t->parent->ne; //one of the base cases: if t is se, t's north is in the same parent node
  else if(isSouthWestChild(t)) return t->parent->nw; //one of the base cases: if t is sw, t's north is in the same parent node
  //else, we have to travel in the tree to find a new node
  else{
    Node* n = NNbr(t->parent);
    if(n == NULL) return NULL;
    if(isLeaf(n)) return n; //t may not have the same sized neighbour (ie. may not have children, may be a leaf) - in that case, simply return the node we got back from recursion
    //if the node we got back has children, return the correct one of the children:
    if(isNorthEastChild(t)) return n->se;
    else if(isNorthWestChild(t)) return n->sw;
  }
}

/* SNbr(t)
 * return the same-sized quad tree node that is south of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::SNbr(Node *t) {

  if(t == NULL) {
    return NULL; 
  }
  else if(t->parent == NULL) {
    return NULL;
  }
  else if(isNorthEastChild(t)) {
    return t->parent->se;
  }
  else if(isNorthWestChild(t)){
    return t->parent->sw;
  } 
  else{
    Node* n = SNbr(t->parent);
    if(n == NULL){
      return NULL;
    }
    if(isLeaf(n)){
      return n;
    }
    if(isSouthEastChild(t)){
      return n->ne;
    } 
    else if(isSouthWestChild(t)) {
      return n->nw;
    }
  }
}

/* ENbr(t)
 * return the same-sized quad tree node that is east of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::ENbr(Node *t) {

  if(t == NULL) return NULL;
  else if(t->parent == NULL) return NULL;
  else if(isNorthWestChild(t)) return t->parent->ne;
  else if(isSouthWestChild(t)) return t->parent->se;
  else{
    Node* n = ENbr(t->parent);
    if(n == NULL){
      return NULL;
    }
    if(isLeaf(n)){
      return n;
    }
    if(isNorthEastChild(t)) return n->nw;
    else if(isSouthEastChild(t)) return n->sw;
  }
}

/* WNbr(t)
 * return the same-sized quad tree node that is west of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::WNbr(Node *t) {

  if(t == NULL) return NULL;
  else if(t->parent == NULL) return NULL;
  else if(isNorthEastChild(t)) return t->parent->nw;
  else if(isSouthEastChild(t)) return t->parent->sw;
  else{
    Node* n = WNbr(t->parent);
    if(n == NULL){
      return NULL;
    }
    if(isLeaf(n)){
      return n;
    }
    if(isNorthWestChild(t)) return n->ne;
    else if(isSouthWestChild(t)) return n->se;
  }
}

bool QTree::write(string const & fileName){

  PNG out_im(im.width(), im.height());

  findLeaves(root, out_im);
  // include the following line to write the image to file.
  return(out_im.writeToFile(fileName));
}

void QTree::findLeaves(Node* n, PNG& newIm){
  if(isLeaf(n) == true){
    colorSquare(n, newIm); //color each leaf in the new image (the compressed image that will be the output)
  } else {
    findLeaves(n->nw, newIm);
    findLeaves(n->ne, newIm);
    findLeaves(n->sw, newIm);
    findLeaves(n->se, newIm); 
  }
}

void QTree::colorSquare(Node* n, PNG& newIm){
  pair<int,int> upleft = n->upLeft;
  int startX = upleft.first;
  int startY = upleft.second;
  int endX = startX + n->size;
  int endY = startY + n->size;
  for(int x = startX; x < endX; x++){
    for(int y = startY; y < endY; y++){
      *(newIm.getPixel(x, y)) = n->avg; //color all the pixels in the node with the average color of the node
    }
  }
  //if drawFrame == true, then color the borders with the frameColor
  //note that there are 4 borders of a square:) 1st loop overrides the color of the horizontal borders, 2nd loop overrides vertical
  if(drawFrame == true){
    pair<int,int> upleft = n->upLeft;
    int startX = upleft.first;
    int startY = upleft.second;
    int endX = startX + n->size - 1;
    int endY = startY + n->size - 1;
    for(int x = startX; x < endX + 1; x++){
      *(newIm.getPixel(x, startY)) = frameColor;
      *(newIm.getPixel(x, endY)) = frameColor;
    }
    for(int y = startY; y < endY + 1; y++){
      *(newIm.getPixel(startX, y)) = frameColor;
      *(newIm.getPixel(endX, y)) = frameColor;  
    }
  }
  //note that we do the border coloring for each square (each node!), not just for the whole image!
}

//non-rec clear
void QTree::clear() {
  clearNodes(root);
}

//rec clear
void QTree::clearNodes(Node* &orig_n){
  if(orig_n != NULL){
    clearNodes(orig_n->nw);
    clearNodes(orig_n->ne);
    clearNodes(orig_n->sw);
    clearNodes(orig_n->se);
    delete orig_n;
    orig_n = NULL;
  }
}

//non-rec copy
void QTree::copy(const QTree & orig) {
  numLeaf = orig.numLeaf;
  im = orig.im;
  leafBound = orig.leafBound;
  balanced = orig.balanced;
  drawFrame = orig.drawFrame;
  frameColor = orig.frameColor;
  root = copyNodes(orig.root);
}

//rec copy; returns a new root pointer, and also creates new pointers to every node in the original image
QTree::Node* QTree::copyNodes(const Node* orig_n){
  Node* n = NULL;
  if(orig_n != NULL){
    n = new Node(this->im, orig_n->upLeft, orig_n->size, orig_n->parent);
    n->nw = copyNodes(orig_n->nw);
    n->ne = copyNodes(orig_n->ne);
    n->sw = copyNodes(orig_n->sw);
    n->se = copyNodes(orig_n->se);
  }
  return n;
}