#include <stdio.h>
#include <stdlib.h>

struct tree_t
{
  struct tree_t* parent;
  struct tree_t* left;
  struct tree_t* right;
  int key;
};

void add(struct tree_t**, int);
void delete(struct tree_t**, int);
struct tree_t* find(struct tree_t*, int);
void succesor(struct tree_t*);
void print(struct tree_t*);
struct tree_t* min(struct tree_t*);
struct tree_t* max(struct tree_t*);
void inorder(struct tree_t*);
void preorder(struct tree_t*);
void postorder(struct tree_t*);


int main(void)
{
  struct tree_t* tree = NULL;
  add(&tree, 20);
  add(&tree, 5);
  add(&tree, 25);
  add(&tree, 35);
  add(&tree, 15);
  add(&tree, 18);
  add(&tree, 29);
  add(&tree, 50);
  add(&tree, 44);
  add(&tree, 39);
  add(&tree, 13);

  inorder(tree);
  printf("\n");

  preorder(tree);
  printf("\n");

  postorder(tree);
  printf("\n");

  succesor(tree->right->right->left);

  return 0;
}

void add(struct tree_t** tree, int key)
{
  struct tree_t* var = NULL;
  struct tree_t* temp = *tree;

  // TWORZENIE NOWEGO ELEMENTU ///////////////////////////////////
  struct tree_t* new_element = NULL;                            //
  new_element = (struct tree_t*)malloc(sizeof(struct tree_t));  //
  new_element->parent = NULL;                                   //
  new_element->left = NULL;                                     //
  new_element->right = NULL;                                    //
  new_element->key = key;                                       //
  ////////////////////////////////////////////////////////////////

  // SZUKAMY GDZIE WRZUCIC NOWY ELEMENT ////////////////////////
  while (temp != NULL)                                        //
  {                                                           //
    var = temp;                                               //
    if (new_element->key < temp->key)                         //
    {                                                         //
      temp = temp->left;                                      //
    }                                                         //
    else                                                      //
    {                                                         //
      temp = temp->right;                                     //
    }                                                         //
  }                                                           //
  // KONIEC SZUKANIA MIEJSCA ///////////////////////////////////

  new_element->parent = var;
  if (var == NULL)  // BRAK DRZEWA
  {
    *tree = new_element;
  }
  else if (new_element->key < var->key)
  {
    var->left = new_element;
  }
  else
  {
    var->right = new_element;
  }
}

struct tree_t* find(struct tree_t* tree, int key)
{
  struct tree_t* temp = tree;
  while(temp != NULL)
  {
    if (key == temp->key)
    {
      // znaleziono ELEMENT
      return temp;
    }
    else if(key < temp->key)
    {
      temp = temp->left;
    }
    else
    {
      temp = temp->right;
    }
  }
  return NULL;
}

void succesor(struct tree_t* tree)
{
  struct tree_t* temp = tree;
  if (temp->right)
  {
    temp = temp->right;
    while (temp->left)
    {
      temp = temp->left;
    }
  }
  else if (temp->parent->left == temp)
  {
    temp = temp->parent;
  }
  else if(temp->parent != NULL)
  {
    while(temp != temp->parent->left)
    {
      temp = temp->parent;
    }
  }

  printf(" %d\n", temp->key);
  return;
}

void inorder(struct tree_t* root)
{
  struct tree_t* tree = root;
  if(tree->left)
  {
    inorder(tree->left);
  }
  printf("%d  ", tree->key);
  if(root->right)
  {
    inorder(tree->right);
  }
}

void preorder(struct tree_t* root)
{
  struct tree_t* tree = root;
  printf("%d  ", tree->key);
  if(tree->left)
  {
    preorder(tree->left);
  }
  if(tree->right)
  {
    preorder(tree->right);
  }
}

void postorder(struct tree_t* root)
{
  struct tree_t* tree = root;
  if(tree->left)
  {
    postorder(tree->left);
  }
  if(tree->right)
  {
    postorder(tree->right);
  }
  printf("%d  ", tree->key);
}

void print(struct tree_t* tree)
{
  struct tree_t* temp = tree;
  printf("adres = %p, parent = %p, left son = %p, right son = %p, key = %d\n",
          tree, tree->parent, tree->left, tree->right, tree->key);
  if(tree->left == NULL && tree->right == NULL)
  {
    return;
  }
  if(tree->left)
  {
    tree = tree->left;
    print(tree);
  }
  if(temp->right)
  {
    temp = temp->right;
    print(temp);
  }
}

struct tree_t* min(struct tree_t* tree)
{
  struct tree_t* min = tree;
  while(min->left)
  {
    min = min->left;
  }
  return min;
}

struct tree_t* max(struct tree_t* tree)
{
  struct tree_t* max = tree;
  while(max->right)
  {
    max = max->right;
  }
  return max;
}

void delete(struct tree_t** tree, int key)
{
  struct tree_t* temp3; //x
  struct tree_t* temp2; //y
  struct tree_t* temp;  //z

  temp = find(*tree, key);
  if(temp == NULL)  //nie znaleziono elementu, albo drzewo jest puste
  {
    return;
  }

  if (temp->left == NULL && temp->right == NULL)  //lisc
  {
    temp2 = temp;
    if (temp->parent->left == temp2)
    {
      temp->parent->left = NULL;
    }
    else
    {
      temp->parent->right = NULL;
    }
    free(temp2);
  }


//FIXME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // if (temp2->left != NULL)
  // {
  //   temp3 = temp2->left;
  // }
  // else
  // {
  //   temp3 = temp2->right;
  // }
  //
  // if (temp3 == NULL)
  // {
  //   temp3->parent = temp2->parent;
  // }
//FIXME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}
