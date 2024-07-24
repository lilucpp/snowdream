#include <iostream>
#include <string>

std::string g_target;

struct TreeNode {
  TreeNode *left;
  TreeNode *right;
  char val;

  TreeNode(char c) : left(nullptr), right(nullptr), val(c) {}
};
/*
             F
           /   \
          B     G
        /   \     \
       A     D     I
           /  \    /
          C    E  H
*/
TreeNode *initTreeNode() {
  TreeNode *root = new TreeNode('F');
  TreeNode *b = new TreeNode('B');
  TreeNode *g = new TreeNode('G');
  TreeNode *a = new TreeNode('A');
  TreeNode *d = new TreeNode('D');
  TreeNode *i = new TreeNode('I');
  TreeNode *c = new TreeNode('C');
  TreeNode *e = new TreeNode('E');
  TreeNode *h = new TreeNode('H');

  root->left = b;
  root->right = g;
  b->left = a;
  b->right = d;
  g->right = i;
  d->left = c;
  d->right = e;
  i->left = h;

  return root;
}

void preOrderTraversal(TreeNode *root) {
  if (root == nullptr)
    return;

  if (g_target.empty())
    g_target = std::string(1, root->val);
  else
    g_target += "->" + std::string(1, root->val);

  preOrderTraversal(root->left);
  preOrderTraversal(root->right);
}

void inOrderTraversal(TreeNode *root) {
  if (root == nullptr)
    return;

  inOrderTraversal(root->left);

  if (g_target.empty())
    g_target = std::string(1, root->val);
  else
    g_target += "->" + std::string(1, root->val);

  inOrderTraversal(root->right);
}

void postOrderTraversal(TreeNode *root) {
  if (root == nullptr)
    return;

  postOrderTraversal(root->left);
  postOrderTraversal(root->right);

  if (g_target.empty())
    g_target = std::string(1, root->val);
  else
    g_target += "->" + std::string(1, root->val);
}

int main() {
  TreeNode *root = initTreeNode();

  g_target = "";
  std::string proOrderStr("F->B->A->D->C->E->G->I->H");
  preOrderTraversal(root);
  if (g_target.compare(proOrderStr) == 0) {
    std::cout << "preOrderTraversal success." << std::endl;
  } else {
    std::cout << g_target << std::endl;
    std::cout << "preOrderTraversal fail." << std::endl;
  }

  g_target = "";
  std::string inOrderStr("A->B->C->D->E->F->G->H->I");
  inOrderTraversal(root);
  if (g_target.compare(inOrderStr) == 0) {
    std::cout << "inOrderTraversal success." << std::endl;
  } else {
    std::cout << "inOrderTraversal fail." << std::endl;
  }

  g_target = "";
  std::string postOrderStr("A->C->E->D->B->H->I->G->F");
  postOrderTraversal(root);
  if (g_target.compare(postOrderStr) == 0) {
    std::cout << "postOrderTraversal success." << std::endl;
  } else {
    std::cout << "postOrderTraversal fail." << std::endl;
  }

  return 0;
}