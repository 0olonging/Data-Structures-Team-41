// Your AVL tree. Every member function below is a stub that compiles but does
// nothing useful yet; replace each one with your implementation.

#include "avl_tree.h"
#include <algorithm>
#include <iostream>

// TODO: Add the members your implementation needs, such as the key, the
// value, the height or balance factor, and links to the children.
struct AVLNode {
  std::int64_t key;
  std::int64_t value;
  int height=0;
  int BF=0;
  AVLNode * left;
  AVLNode * right;
  AVLNode(std::int64_t k,std::int64_t v):key{k},value{v},left{nullptr},right{nullptr}{
    
  }
};

AVLTree::AVLTree() {
  (void)root_;
}
void Destroy(AVLNode * Node){
  if(!Node){
    return;
  }
  Destroy(Node->left);
  Destroy(Node->right);
  delete Node;
}
AVLTree::~AVLTree() {
  Destroy(root_);
  root_ = nullptr;
}

int nodeHeight(AVLNode * Node){
  return Node?Node->height:-1;
}

int nodeBF(AVLNode * Node){
  return nodeHeight(Node->right)-nodeHeight(Node->left);
}

AVLNode *rightRotate(AVLNode *y) { 
    AVLNode *x = y->left; 
    AVLNode *xr = x->right; 

    x->right = y; 
    y->left = xr; 

    y->height = 1 + std::max(nodeHeight(y->left), nodeHeight(y->right)); 
    x->height = 1 + std::max(nodeHeight(x->left), nodeHeight(x->right)); 

    x->BF = nodeBF(x);
    y->BF = nodeBF(y);
    return x; 
} 

AVLNode *leftRotate(AVLNode *x) { 
    AVLNode *y = x->right; 
    AVLNode *yl = y->left; 

    y->left = x; 
    x->right = yl; 

    x->height = 1 + std::max(nodeHeight(x->left), nodeHeight(x->right)); 
    y->height = 1 + std::max(nodeHeight(y->left), nodeHeight(y->right));

    x->BF = nodeBF(x);
    y->BF = nodeBF(y);
    return y; 
} 
AVLNode * insetHelper(AVLNode * Node,std::int64_t key, std::int64_t value,bool& skey){
  if (Node == nullptr){
    return new AVLNode(key,value);
  }
  if(key < Node->key){
    Node->left = insetHelper(Node->left,key,value,skey);
  }else if(key > Node->key){
    Node->right = insetHelper(Node->right,key,value,skey);
  }else{
    skey = false;
    return Node;
  }

  Node->height = 1+ std::max(nodeHeight(Node->left), nodeHeight(Node->right));

  Node->BF = nodeBF(Node);

  if (Node->BF < -1 && key < Node->left->key) {
    return rightRotate(Node); 
  }
    
  if (Node->BF > 1 && key > Node->right->key) {
    return leftRotate(Node);
  }
   
  if (Node->BF < -1 && key > Node->left->key) { 
    Node->left = leftRotate(Node->left); 
    return rightRotate(Node); 
  } 

    
  if(Node->BF > 1 && key < Node->right->key) { 
    Node->right = rightRotate(Node->right); 
    return leftRotate(Node); 
  } 

  return Node;
}

bool AVLTree::insert(std::int64_t key, std::int64_t value) {
  bool check = true;
  root_=insetHelper(root_,key,value,check);
  if(check){
    size_++;
  }
  return check;  
}

AVLNode * minValNode(AVLNode * Node){
  AVLNode * current = Node;
  while (current->left){
    current = current->left;
  }
  return current;
}

AVLNode * eraseHelper(AVLNode *Node, std::int64_t key, bool& nkey){
  if (Node == nullptr){
    nkey = false;
    return Node;
  }
  if(key < Node->key){
    Node->left = eraseHelper(Node->left, key,nkey);
  }else if(key > Node->key) {
    Node->right = eraseHelper(Node->right, key,nkey);
  }else {
    if ((Node->left == nullptr) ||(Node->right == nullptr)) {
      AVLNode *temp = Node->left ? Node->left : Node->right;
      if (temp == nullptr) {
        delete Node; 
        Node = nullptr;
      } else{ 
        AVLNode * temp = Node->left ? Node->left : Node->right;
				delete Node; 
				return temp; 
      }
    } else {
      AVLNode * temp = minValNode(Node->right);

      Node->key = temp->key;
      Node->value = temp->value;

      Node->right = eraseHelper(Node->right, temp->key,nkey);
    }
  }

  if(Node == nullptr){
    return Node;
  }
  Node->height = 1+ std::max(nodeHeight(Node->left), nodeHeight(Node->right));

  Node->BF = nodeBF(Node);

  if (Node->BF < -1 && nodeBF(Node->left) <=0) {
    return rightRotate(Node); 
  }

  if (Node->BF <-1 && nodeBF(Node->left)>0) { 
    Node->left = leftRotate(Node->left); 
    return rightRotate(Node); 
  } 
    
  if (Node->BF > 1 && nodeBF(Node->right)>=0) {
    return leftRotate(Node);
  }
   
  if(Node->BF > 1 && nodeBF(Node->right)<0) { 
    Node->right = rightRotate(Node->right); 
    return leftRotate(Node); 
  } 
  return Node;
}


bool AVLTree::erase(std::int64_t key) {
  bool check = true;
  root_ = eraseHelper(root_, key,check);
  if(check){
    size_--;
  }
  return check;
}

AVLNode * search(AVLNode * Node,std::int64_t key){
  if(Node == nullptr || key==Node->key){
    return Node;
  }
  if(key<Node->key){
    return search(Node->left,key);
  }else{
    return search(Node->right,key);
  }

}

std::optional<std::int64_t> AVLTree::find(std::int64_t key) const {
  AVLNode * Searcher = search(root_,key);
  if(Searcher){
    return Searcher->value;
  }else{
    return std::nullopt;
  }
}

std::size_t AVLTree::size() const {
  return size_;
}

void keysInOrder( std::vector<std::int64_t>& vec ,AVLNode*Node){
  if(Node){
    keysInOrder(vec,Node->left);
    vec.push_back(Node->key);
    keysInOrder(vec,Node->right);
  }
}
std::vector<std::int64_t> AVLTree::keys() const {
  std::vector<std::int64_t> keylist;
  keysInOrder(keylist,root_);
  return keylist ;
}

void treeStruct(const AVLNode*Node,std::ostream & os){
  if(Node){
    treeStruct(Node->left,os);
    os<< Node->key<<' '<<Node->BF<<' ';
    if(Node->left){
      os<< Node->left->key<<' ';
    }else{
      os<< '-'<<' ';
    }
    if(Node->right){
      os<< Node->right->key;
    }else{
      os<< '-';
    }
    os<<'\n';
    treeStruct(Node->right,os);
  }
}

void AVLTree::export_shape(std::ostream & out) const {
if(root_ == nullptr){
  out<<"root -"<<'\n';
  return;
}
  out<<"root "<<root_->key<<'\n';
  treeStruct(root_,out);


}

int AVLTree::height() const {
  if(root_ == nullptr){
    return -1;
  }
  root_->height = 1+ std::max(nodeHeight(root_->left), nodeHeight(root_->right));
  return root_->height;  
}

void Adder( AVLNode*Node, double& sum){
  if(Node){
    Adder(Node->left,sum);
    sum += Node->height; 
    Adder(Node->right,sum);
  }
}
AVLNode * searchwithdepth(AVLNode * Node,std::int64_t key,double& depth){
  if(Node == nullptr || key==Node->key){
    return Node;
  }
  if(key<Node->key){
    ++depth;
    return searchwithdepth(Node->left,key,depth);
  }else{
    ++depth;
    return searchwithdepth(Node->right,key,depth);
  }

}
double AVLTree::average_depth() const {
  if(root_ == nullptr){
    return 0;
  }
  double sum=0.0;
  for(auto x: keys()){
    double depth = 0.0;
    AVLNode *Searcher = searchwithdepth(root_,x,depth);
    if (Searcher){
      sum+=depth;
    }
    
  }
  return sum/size();
}
