typedef struct bin_tree {
int data;
struct bin_tree * right, * left;
};
typedef struct bin_tree node;
void insert(node ** tree, int val)
{
    node *temp = NULL;
    if(!(*tree))
    {
        temp = (node *)malloc(sizeof(node));
        temp->left = temp->right = NULL;
        temp->data = val;
        *tree = temp;
        return;
    }

    if(val <= (*tree)->data)
    {
        insert(&(*tree)->left, val);
    }
    else if(val > (*tree)->data)
    {
        insert(&(*tree)->right, val);
    }

}

void print_preorder(node * tree)
{
    if (tree)
    {
        printf("%d\n",tree->data);
        print_preorder(tree->left);
        print_preorder(tree->right);
    }

}

void print_inorder(node * tree)
{
    if (tree)
    {
        print_inorder(tree->left);
        printf("%d\n",tree->data);
        print_inorder(tree->right);
    }
}

void print_postorder(node * tree)
{
    if (tree)
    {
        print_postorder(tree->left);
        print_postorder(tree->right);
        printf("%d\n",tree->data);
    }
}

void deltree(node * tree)
{
    if (tree)
    {
        deltree(tree->left);
        deltree(tree->right);
        free(tree);
    }
}

node* find(node *r, float d)
{
if (r==NULL)
   {
   return NULL;  // не найден
   }
if(r->data == d)
   {
   return r; // нашли!!!
   }
 
if (d<=r->data)
  {
  // left
  if (r->left!=NULL)
    return find(r->left,d); // рекурсивный поиск влево
  else
     {
     return NULL; // не найден
     }
  }
else
  {
  //right
  if (r->right)
    return find(r->right,d);// рекурсивный поиск вправо
  else
    {
    return NULL; // не найден
    }
  }
}

void tree_remove(node ** tr, int val){
 struct   bin_tree *  ptr1, *iter, *ptr2;

    for(iter = *tr; iter != NULL;){
        if(val == iter->data)
            break;
        else if(val <= iter->data) {
            tr   = &iter->left;
            iter = iter->left;
        } else {
            tr   = &iter->right;
            iter = iter->right;
        }
    }

    if(iter == NULL)
        return;

    if(iter->right == NULL)
        *tr = iter->left;
    else {
        ptr2 = iter->right;
        if(ptr2->left == NULL) {
            ptr2->left = iter->left;
            *tr = ptr2;
        } else {
            ptr1 = ptr2->left;
            while(ptr1->left != NULL){
                ptr2 = ptr1;
                ptr1 = ptr2->left;
            }
            ptr2->left  = ptr1->right;
            ptr1->left  = iter->left;
            ptr1->right = iter->right;
            *tr = ptr1;
        }
    }
    free(iter);
}
