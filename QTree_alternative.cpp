// /**
//  *
//  * Balanced Quad Tree (pa3)
//  *
//  * This file will be used for grading.
//  *
//  */

// #include "QTree.h"

// // Return the biggest power of 2 less than or equal to n
// int biggestPow2(int n) {
//   if( n < 1 ) return 0;
//   int v = 1;
//   while( v <= n ) v <<= 1;
//   return v >> 1;
// }

// QTree::Node::Node(PNG & im, pair<int,int> ul, int sz, Node *par)
//   :upLeft(ul),size(sz),parent(par),nw(NULL),ne(NULL),sw(NULL),se(NULL)
// {
//   var = varAndAvg(im,ul,size,avg);
// }

// QTree::~QTree(){
//   clear();
// }

// QTree::QTree(const QTree & other) {
//   copy(other);
// }

// QTree & QTree::operator=(const QTree & rhs){
//   if (this != &rhs) {
//     clear();
//     copy(rhs);
//   }
//   return *this;
// }


// QTree::QTree(PNG & imIn, int leafB, RGBAPixel frameC, bool bal)
//   : leafBound(leafB), balanced(bal), drawFrame(true), frameColor(frameC)
// {

//   /* YOUR CODE HERE */
//   im = imIn;
//   pair<int,int> rx_ry(0,0);
//   numLeaf = 1;
//   root = new Node(im, rx_ry, biggestPow2(im.width()), NULL);
//   split(root);
// }


// QTree::QTree(PNG & imIn, int leafB, bool bal)
//   : leafBound(leafB), balanced(bal), drawFrame(false)
// {
  
//   /* YOUR CODE HERE */
//   im = imIn;
//   pair<int,int> rx_ry(0,0);
//   numLeaf = 1;
//   root = new Node(im, rx_ry, biggestPow2(im.width()), NULL);
//   split(root);
// }


// bool QTree::isLeaf( Node *t ) {
  
//   /* YOUR CODE HERE */
//   if(t == NULL) {
//     return false;
//   }
//   return (t->nw == NULL);
// }
  
// void QTree::split( Node *t ) {
  
//   /* YOUR CODE HERE */

//   // FOR BALANCED QTREES-------------------------------------------------
//   // A split might cause one or two nbrs of the parent of t to split
//   // to maintain balance.  Note that these two nbrs exist (unless they're
//   // not in the image region) because the current set of leaves are
//   // balanced.
//   // if( t is a NW (or NE or SW or SE) child ) then we need to check that
//   // the North and West (or North and East or South and West or
//   // South and East) nbrs of t->parent have children. If they don't
//   // we need to split them.

//   priority_queue<QTree::Node*, vector<QTree::Node*>, CmpNodes> nodePQ;
//   bool skipLeafCondition = false;

//   if(balanced == false){
//     splitRecursiveImb(t, nodePQ);
//   } else {
//     splitRecursiveBal(t, nodePQ, skipLeafCondition);
//   }  
// }

// bool QTree::needsSplit(Node* t, Node* neighbour){
//   if(t == NULL || neighbour == NULL) return false;
//   int childSize;
//   if(isLeaf(t) == true){
//     childSize = t->size;
//   } else {
//     childSize = t->size/2;
//   }
// //   if(isLeaf(neighbour) == false){
// //     return false;
// //   }
//   if( (childSize == neighbour->size) || (childSize == (neighbour->size / 2)) || (childSize == (neighbour->size * 2)) ){
//     return false;
//   }  
//   return true;
// }

// bool QTree::isNorthWestChild(Node* t){
//   return ( (t->upLeft.first == t->parent->upLeft.first) && (t->upLeft.second == t->parent->upLeft.second) );
// }

// bool QTree::isNorthEastChild(Node* t){
//   return ( (t->upLeft.first == t->parent->upLeft.first + t->size) && (t->upLeft.second == t->parent->upLeft.second) );
// }

// bool QTree::isSouthWestChild(Node* t){
//   return ( (t->upLeft.first == t->parent->upLeft.first) && (t->upLeft.second == t->parent->upLeft.second + t->size) );  
// }

// bool QTree::isSouthEastChild(Node* t){
//   return ( (t->upLeft.first == t->parent->upLeft.first + t->size) && (t->upLeft.second == t->parent->upLeft.second + t->size) );
// }

// void QTree::splitRecursiveBal(Node *t, priority_queue<QTree::Node*, vector<QTree::Node*>, CmpNodes>& nodePQ, bool skipLeafCondition){
//   //cout << "entered split with t: " << t->upLeft.first << " " << t->upLeft.second << " and numLeaf: " << numLeaf << endl;
//   if( (numLeaf < leafBound) || (skipLeafCondition == true) ){
//     pair<int,int> nw_pair(t->upLeft.first, t->upLeft.second);
//     pair<int,int> ne_pair(t->upLeft.first + t->size/2, t->upLeft.second);
//     pair<int,int> sw_pair(t->upLeft.first, t->upLeft.second + t->size/2);
//     pair<int,int> se_pair(t->upLeft.first + t->size/2, t->upLeft.second + t->size/2);

//     t->nw = new Node(im, nw_pair, t->size/2, t);
//     t->ne = new Node(im, ne_pair, t->size/2, t);
//     t->sw = new Node(im, sw_pair, t->size/2, t);
//     t->se = new Node(im, se_pair, t->size/2, t);

//     nodePQ.push(t->nw);
//     nodePQ.push(t->ne);
//     nodePQ.push(t->sw);
//     nodePQ.push(t->se);

//     // cout << "added: " << t->nw->upLeft.first << " " << t->nw->upLeft.second << endl;
//     // cout << "added: " << t->ne->upLeft.first << " " << t->ne->upLeft.second << endl;
//     // cout << "added: " << t->sw->upLeft.first << " " << t->sw->upLeft.second << endl;
//     // cout << "added: " << t->se->upLeft.first << " " << t->se->upLeft.second << endl;    

//     numLeaf += 3;
//     //numLeaf = nodePQ.size();

//     if(t->parent != NULL){
//       bool split = false;
//       if(isNorthWestChild(t)){
//         split = needsSplit(t, NNbr(t));
//         if(split) { splitRecursiveBal(NNbr(t), nodePQ, true); }
//         split = needsSplit(t, WNbr(t));
//         if(split) { splitRecursiveBal(WNbr(t), nodePQ, true); }
//       }
//       else if(isNorthEastChild(t)){
//         split = needsSplit(t, NNbr(t));
//         if(split) { splitRecursiveBal(NNbr(t), nodePQ, true); }
//         split = needsSplit(t, ENbr(t));
//         if(split) { splitRecursiveBal(ENbr(t), nodePQ, true); }
//       }
//       else if(isSouthWestChild(t)){
//         split = needsSplit(t, SNbr(t));
//         if(split) { splitRecursiveBal(SNbr(t), nodePQ, true); }
//         split = needsSplit(t, WNbr(t));
//         if(split) { splitRecursiveBal(WNbr(t), nodePQ, true); }
//       }
//       else if(isSouthEastChild(t)){
//         split = needsSplit(t, SNbr(t));
//         if(split) { splitRecursiveBal(SNbr(t), nodePQ, true); }
//         split = needsSplit(t, ENbr(t));
//         if(split) { splitRecursiveBal(ENbr(t), nodePQ, true); }
//       }
//     }
//     Node* next = nodePQ.top();
//     //cout << "splitting: " << next->upLeft.first << " " << next->upLeft.second << endl;
//     nodePQ.pop();
//     splitRecursiveBal(next, nodePQ, false);
//   } 
// }

// void QTree::splitRecursiveImb(Node *t, priority_queue<QTree::Node*, vector<QTree::Node*>, CmpNodes>& nodePQ){
//   if(numLeaf < leafBound){
//     pair<int,int> nw_pair(t->upLeft.first, t->upLeft.second);
//     pair<int,int> ne_pair(t->upLeft.first + t->size/2, t->upLeft.second);
//     pair<int,int> sw_pair(t->upLeft.first, t->upLeft.second + t->size/2);
//     pair<int,int> se_pair(t->upLeft.first + t->size/2, t->upLeft.second + t->size/2);

//     t->nw = new Node(im, nw_pair, t->size/2, t);
//     t->ne = new Node(im, ne_pair, t->size/2, t);
//     t->sw = new Node(im, sw_pair, t->size/2, t);
//     t->se = new Node(im, se_pair, t->size/2, t);

//     nodePQ.push(t->nw);
//     nodePQ.push(t->ne);
//     nodePQ.push(t->sw);
//     nodePQ.push(t->se);

//     numLeaf += 3;

//     Node* next = nodePQ.top();
//     nodePQ.pop();
//     splitRecursiveImb(next, nodePQ);
//   }
// }


// /* NNbr(t)
//  * return the same-sized quad tree node that is north of Node t.
//  * return NULL if this node is not in the QTree.
//  */
// QTree::Node * QTree::NNbr(Node *t) {

//   /* YOUR CODE HERE */
//   if(t == NULL){
//     return NULL;
//   } 
//   else if(t->parent == NULL){
//     return NULL;
//   } 
//   else if(isSouthEastChild(t)){
//     return t->parent->ne;
//   }
//   else if(isSouthWestChild(t)){
//     return t->parent->nw;
//   }
//   else{
//     Node* n = NNbr(t->parent);
//     if(n == NULL){
//       return NULL;
//     }
//     if(isLeaf(n)) {
//       return n;
//     }
//     if(isNorthEastChild(t)){
//       return n->se;
//     }
//     else if(isNorthWestChild(t)){
//       return n->sw;
//     }
//   }
// }

// /* SNbr(t)
//  * return the same-sized quad tree node that is south of Node t.
//  * return NULL if this node is not in the QTree.
//  */
// QTree::Node * QTree::SNbr(Node *t) {

//   /* YOUR CODE HERE */
//   if(t == NULL) {
//     return NULL; 
//   }
//   else if(t->parent == NULL) {
//     return NULL;
//   }
//   else if(isNorthEastChild(t)) {
//     return t->parent->se;
//   }
//   else if(isNorthWestChild(t)){
//     return t->parent->sw;
//   } 
//   else{
//     Node* n = SNbr(t->parent);
//     if(n == NULL){
//       return NULL;
//     }
//     if(isLeaf(n)){
//       return n;
//     }
//     if(isSouthEastChild(t)){
//       return n->ne;
//     } 
//     else if(isSouthWestChild(t)) {
//       return n->nw;
//     }
//   }
// }

// /* ENbr(t)
//  * return the same-sized quad tree node that is east of Node t.
//  * return NULL if this node is not in the QTree.
//  */
// QTree::Node * QTree::ENbr(Node *t) {

//   /* YOUR CODE HERE */
//   if(t == NULL) return NULL;
//   else if(t->parent == NULL) return NULL;
//   else if(isNorthWestChild(t)) return t->parent->ne;
//   else if(isSouthWestChild(t)) return t->parent->se;
//   else{
//     Node* n = ENbr(t->parent);
//     if(n == NULL){
//       return NULL;
//     }
//     if(isLeaf(n)){
//       return n;
//     }
//     if(isNorthEastChild(t)) return n->nw;
//     else if(isSouthEastChild(t)) return n->sw;
//   }
// }

// /* WNbr(t)
//  * return the same-sized quad tree node that is west of Node t.
//  * return NULL if this node is not in the QTree.
//  */
// QTree::Node * QTree::WNbr(Node *t) {

//   /* YOUR CODE HERE */
//   if(t == NULL) return NULL;
//   else if(t->parent == NULL) return NULL;
//   else if(isNorthEastChild(t)) return t->parent->nw;
//   else if(isSouthEastChild(t)) return t->parent->sw;
//   else{
//     Node* n = WNbr(t->parent);
//     if(n == NULL){
//       return NULL;
//     }
//     if(isLeaf(n)){
//       return n;
//     }
//     if(isNorthWestChild(t)) return n->ne;
//     else if(isSouthWestChild(t)) return n->se;
//   }
// }

// bool QTree::write(string const & fileName){

//   /* YOUR CODE HERE */
//   PNG out_im(im.width(), im.height());

//   findLeaves(root, out_im);
//   // include the following line to write the image to file.
//   return(out_im.writeToFile(fileName));
// }

// void QTree::findLeaves(Node* n, PNG& newIm){
//   if(isLeaf(n) == true){
//     colorSquare(n, newIm);
//   } else {
//     findLeaves(n->nw, newIm);
//     findLeaves(n->ne, newIm);
//     findLeaves(n->sw, newIm);
//     findLeaves(n->se, newIm); 
//   }
// }

// void QTree::colorSquare(Node* n, PNG& newIm){
//   pair<int,int> upleft = n->upLeft;
//   int startX = upleft.first;
//   int startY = upleft.second;
//   int endX = startX + n->size;
//   int endY = startY + n->size;
//   for(int x = startX; x < endX; x++){
//     for(int y = startY; y < endY; y++){
//       *(newIm.getPixel(x, y)) = n->avg;
//     }
//   }
//   //if drawFrame == true, then color the borders with the frameColor
//   //note that there are 4 borders of a square:)
//   if(drawFrame == true){
//     pair<int,int> upleft = n->upLeft;
//     int startX = upleft.first;
//     int startY = upleft.second;
//     int endX = startX + n->size - 1;
//     int endY = startY + n->size - 1;
//     for(int x = startX; x < endX + 1; x++){
//       *(newIm.getPixel(x, startY)) = frameColor;
//       *(newIm.getPixel(x, endY)) = frameColor;
//     }
//     for(int y = startY; y < endY + 1; y++){
//       *(newIm.getPixel(startX, y)) = frameColor;
//       *(newIm.getPixel(endX, y)) = frameColor;  
//     }
//   }
// }

// void QTree::clear() {

//   /* YOUR CODE HERE */
//   clearNodes(root);
// }


// void QTree::copy(const QTree & orig) {

//   /* YOUR CODE HERE */
//   numLeaf = orig.numLeaf;
//   im = orig.im;
//   leafBound = orig.leafBound;
//   balanced = orig.balanced;
//   drawFrame = orig.drawFrame;
//   frameColor = orig.frameColor;
//   root = copyNodes(orig.root);
// }

// QTree::Node* QTree::copyNodes(const Node* orig_n){
//   Node* n = NULL;
//   if(orig_n != NULL){
//     n = new Node(this->im, orig_n->upLeft, orig_n->size, orig_n->parent);
//     n->nw = copyNodes(orig_n->nw);
//     n->ne = copyNodes(orig_n->ne);
//     n->sw = copyNodes(orig_n->sw);
//     n->se = copyNodes(orig_n->se);
//   }
//   return n;
// }

// void QTree::clearNodes(Node* &orig_n){
//   if(orig_n != NULL){
//     clearNodes(orig_n->nw);
//     clearNodes(orig_n->ne);
//     clearNodes(orig_n->sw);
//     clearNodes(orig_n->se);
//     delete orig_n;
//     orig_n = NULL;
//   }
// }