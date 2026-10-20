// Your red-black tree. Every member function below is a stub that compiles
// but does nothing useful yet; replace each one with your implementation.

#include "red_black_tree.h"

// TODO: Add the members your implementation needs, such as the key, the
// value, the color, and links to the children and the parent.
enum class Color {
    Red,
    Black
};

struct RBNode
{
  std::int64_t key;
  std::int64_t value;
  Color color; // Red or black
  RBNode *left, *right, *parent; // pointer to other nodes

  // Creating the Node
  RBNode(int64_t key, int64_t value)
    : key(key),
      value(value),
      color(Color::Red),
      left(nullptr),
      right(nullptr),
      parent(nullptr)
{
}
};

RedBlackTree::RedBlackTree() {
  // TODO: Set up the tree, for example a sentinel for the NIL leaves.
  nil_ = new RBNode(0, 0);
  nil_->color = Color::Black;

  nil_->left = nil_;
  nil_->right = nil_;
  nil_->parent = nil_;

  root_ = nil_;
  size_ = 0;

}

RedBlackTree::~RedBlackTree() {
  // TODO: Release all nodes.
}

/* Helper functions -> called often in other functions
  so might as well make them into functions instead of repeating*/
void leftRotate(RBNode*& root, RBNode* nil, RBNode* x)
{
  RBNode* y = x->right;
  x->right = y->left;

  if (y->left != nil) 
  {
    y->left->parent = x;
  }

  y->parent = x->parent;

  if (x->parent == nil)
  {
    root = y;
  }
  
  else if (x == x->parent->left)
  { 
    x->parent->left = y;
  }

  else                          
  {
    x->parent->right = y;
  }

  y->left = x;
  x->parent = y;
}

void rightRotate(RBNode*& root, RBNode* nil, RBNode* x)
{
  RBNode* y = x->left;
  x->left = y->right;

  if (y->right != nil) 
  {
    y->right->parent = x;
  }    
  
  y->parent = x->parent;

  if (x->parent == nil)       
  {
    root = y;
  }
  
  else if (x == x->parent->right) 
  {
    x->parent->right = y;
  }
        
  else                           
  {
    x->parent->left = y;
  }

  y->right = x;
  x->parent = y;
}

void RBfixup(RBNode*& root, RBNode* nil, RBNode* x)
{
  while (x != root && x->parent->color == Color::Red)
  {
    // If z's parent is the LEFT child of its grandparent
    if (x->parent == x->parent->parent->left)
    {
      // Let uncle = grandparent's RIGHT child
      RBNode* uncle = x->parent->parent->right;
      
      // Case 1: uncle = red -> recolor parent, uncle and granparent
      if (uncle != nil && uncle->color == Color::Red)
      {
        x->parent->color = Color::Black;
        uncle->color = Color::Black;
        x->parent->parent->color = Color::Red;
        x = x->parent->parent;
      }

      else
      {
        // Case 2: Triangle shape -> Left rotate parent to form a
        if (x == x->parent->right)
        {
          x = x->parent;
          leftRotate(root, nil, x);
        }

        // Case 3: Line shape -> Recolor and right rotate grandparent
        x->parent->color = Color::Black;        
        x->parent->parent->color = Color::Red;
        rightRotate(root, nil, x->parent->parent);

      }
    }

    else
    {
      // Mirror Case: Parent is the right child
      RBNode* uncle = x->parent->parent->left;

      if (uncle != nil && uncle->color == Color::Red)
      {
        x->parent->color = Color::Black;
        uncle->color = Color::Black;
        x->parent->parent->color = Color::Red;
        x = x->parent->parent;
      } 
      
      else 
      {
        if (x == x->parent->left) 
        {
            x = x->parent;
            rightRotate(root, nil, x);
        }
        
        x->parent->color = Color::Black;
        x->parent->parent->color = Color::Red;
        
        leftRotate(root, nil, x->parent->parent);
      }    
    }
  }

  root->color = Color::Black;
}

RBNode* findMin(RBNode* node, RBNode* nil)
{
  if (node == nil)
  {
    return nil;
  }
  
  while (node->left != nil)
  {
    node = node->left;
  }

  return node;
}

void transplant(RBNode*& root, RBNode* nil, RBNode* u, RBNode* v)
{
  if (u->parent == nil)
  {
    root = v;
  }

  else if (u == u->parent->left)
  {
    u->parent->left = v;
  }

  else
  {
    u->parent->right = v;
  }

  if (v != nil)
  {
    v->parent = u->parent;
  }
}

void deleteFixup(RBNode*& root, RBNode* nil, RBNode* replacement)
{
  RBNode* sibling;
  RBNode* parent;
  while (replacement != root && replacement->color == Color::Black)
  {
    parent = replacement->parent;

    if (replacement == parent->left)
    {
      sibling = parent->right;

      // case 1: sibling is red
      if (sibling->color == Color::Red)
      {
        sibling->color = Color::Black;
        parent->color = Color::Red;
        leftRotate(root, nil, parent);
        sibling = parent->right;
      }

      // case 2: sibling is black and both children are black
      if (sibling->left->color == Color::Black 
        && sibling->right->color == Color::Black)
        {
          sibling->color = Color::Red;
          replacement = parent;
        }

      else
      {
        // case 3: sibling is black, left child is red and right child is black
        if (sibling->right->color == Color::Black)
        {
          sibling->left->color = Color::Black;
          sibling->color = Color::Red;
          rightRotate(root, nil, sibling);
          sibling = parent->right;
        }

        // case 4: sibling is black, right child is red
        sibling->color = parent->color;
        parent->color = Color::Black;
        sibling->right->color = Color::Black;
        leftRotate(root, nil, parent);
        replacement = root;
      }
    }

    else  // Mirror case: replacement is parent's RIGHT child
    {
      sibling = parent->left;

      // Case 1: sibling is red
      if (sibling->color == Color::Red)
      {
          sibling->color = Color::Black;
          parent->color = Color::Red;
          rightRotate(root, nil, parent);
          sibling = parent->left;
      }

      // Case 2: sibling is black and both children are black
      if (sibling->left->color == Color::Black
          && sibling->right->color == Color::Black)
      {
          sibling->color = Color::Red;
          replacement = parent;
      }

      else
      {
          // Case 3: sibling is black,
          // right child is red and left child is black
          if (sibling->left->color == Color::Black)
          {
              sibling->right->color = Color::Black;
              sibling->color = Color::Red;
              leftRotate(root, nil, sibling);
              sibling = parent->left;
          }

          // Case 4: sibling is black, left child is red
          sibling->color = parent->color;
          parent->color = Color::Black;
          sibling->left->color = Color::Black;
          rightRotate(root, nil, parent);
          replacement = root;
      }
    }
  }

  replacement->color = Color::Black;
}

bool RedBlackTree::insert(std::int64_t key, std::int64_t value) {
  RBNode *newNode = new RBNode(key, value);
  
  newNode->left = nil_;
  newNode->right = nil_;
  newNode->parent = nil_;

  RBNode *parent = nil_;  // will be the parent of newNode
  
  RBNode *current = root_;    // node being compared with newNode
  
  while (current != nil_) {        // descend until reaching an empty spot
    parent = current;
    if (key == current->key) {
      return false; // Duplicate found
    }

    else if (key < current->key) {
        current = current->left;
    }

    else {
        current = current->right;
    }
  }

  newNode->parent = parent;

  if (parent == nil_) {
    root_ = newNode;  // the tree was empty
  } 
  
  else if (key < parent->key) {
    parent->left = newNode;
  }
  
  else {
    parent->right = newNode;
  }

  RBfixup(root_, nil_, newNode);
  size_++;

  return true;
}

bool RedBlackTree::erase(std::int64_t key) {
  
  RBNode* curr = root_;

  // Stops when nil or key is found -> finding if key exists
  while (curr != nil_ && key != curr->key)
  {
    if (key > curr->key)
    {
      curr = curr->right;
    }

    else
    {
      curr = curr->left;
    }
  }

  if (curr == nil_)
  {
      return false;
  }

  // Preparing to delete the key
  RBNode* replacement;
  Color removedColor = curr->color;

  // If no left child
  if (curr->left == nil_)
  {
    replacement = curr->right;
    transplant(root_, nil_, curr, replacement);
  }

  // if no right child
  else if (curr->right == nil_)
  {
    replacement = curr->left;
    transplant(root_, nil_, curr, replacement);
  }

  // node to delete has to children -> need find successor
  else
  {
    RBNode* successor = findMin(curr->right, nil_);
    removedColor = successor->color;
    replacement = successor->right;

    if (successor->parent == curr)
    {
      replacement->parent = successor;
    }

    else
    {
      transplant(root_, nil_, successor, replacement);
      
      successor->right = curr->right;
      successor->right->parent = successor;
    }

     transplant(root_, nil_, curr, successor);
     successor->left = curr->left;
     successor->left->parent = succesor;
     successor->color = curr->color;

     if (removedColor == Color::Black)
     {
        deleteFixup(root_, nil_, replacement);
     }

  }

  // Deletion
  delete curr;
  size_--;

  return true;
}

std::optional<std::int64_t> RedBlackTree::find(std::int64_t key) const {
  
  RBNode* curr = root_;

  // Stops when nil or key is found
  while (curr != nil_ && key != curr->key)
  {
    if (key > curr->key)
    {
      curr = curr->right;
    }

    else
    {
      curr = curr->left;
    }
  }

  // Cannot find key
  if (curr == nil_)
  {
      return std::nullopt;
  }

  // Return value of key
  return curr->value;
}

std::size_t RedBlackTree::size() const {
  return size_;
}

std::vector<std::int64_t> RedBlackTree::keys() const {
  return {};  // TODO
}

void RedBlackTree::export_shape(std::ostream & /*out*/) const {
  // TODO
}

int RedBlackTree::height() const {
  return -1;  // TODO
}

double RedBlackTree::average_depth() const {
  return 0.0;  // TODO
}
