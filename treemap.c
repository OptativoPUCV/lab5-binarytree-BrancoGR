#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    new->lower_than = lower_than;
    new->root = NULL;
    new->current = NULL;
    return new;
}

void insertTreeMap(TreeMap * tree, void* key, void * value)
{
    if (tree == NULL || key == NULL || value == NULL) return;

    TreeNode *current = tree->root;
    TreeNode *parent = NULL;

    while (current != NULL)
    {
        if (is_equal(tree, key, current->pair->key)) return;
        parent = current;
        if (tree->lower_than(key, current->pair->key))
            current = current->left;
        else
            current = current->right;
    }

    TreeNode *new = createTreeNode(key, value);
    if (new == NULL) return;
    if (parent == NULL)
        tree->root = new;
    else if (tree->lower_than(key, parent->pair->key))
        parent->left = new;
    else
        parent->right = new;
    new->parent = parent;

    tree->current = new;
}    

TreeNode * minimum(TreeNode * x)
{
    if (x == NULL) return NULL;
    while (x->left != NULL)
        x = x->left;
    return x;
}

void removeNode(TreeMap * tree, TreeNode* node)
{
    if (tree == NULL || node == NULL) return;

    if (node->left == NULL && node->right == NULL){
        if (node->parent != NULL)
            if (node->parent->left == node)
                node->parent->left = NULL;
            else
                node->parent->right = NULL;
        else
            tree->root = NULL;
        free(node->pair);
        free(node);
    }

    else if (node->left == NULL || node->right == NULL)
    {
        TreeNode *child = (node->left != NULL) ? node->left : node->right;
        if(node->parent != NULL) {
            if (node->parent->left == node)
                node->parent->left = child;
            else
                node->parent->right = child;
            child->parent = node->parent;
        }
        else
        {
            tree->root = child;
            child->parent = NULL;
        }
        free(node->pair);
        free(node);
    }

    else
    {
        TreeNode *nodoMin = minimum(node->right);
        node->pair = nodoMin->pair;
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    
    TreeNode * current = tree->root;
    while (current != NULL)
    {
        if (is_equal(tree, current->pair->key, key))
        {
            tree->current = current;
            return current->pair;
        }
        else if (tree->lower_than(current->pair->key, key))
            current = current->right;
        else
            current = current->left;
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key)
{
    if (tree == NULL || tree->root == NULL) return NULL;

    TreeNode *current = tree->root;
    TreeNode *upBound = NULL;
    while (current != NULL)
    {
        if (is_equal(tree, current->pair->key, key))
            return current->pair;
        else if (tree->lower_than(current->pair->key, key))
            current = current->right;
        else
        {
            upBound = current;
            current = current->left;
        }
    }

    if (upBound != NULL)
        return upBound->pair;
    else
        return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;

    TreeNode *current = tree->root;
    while (current->left != NULL)
        current = current->left;
    tree->current = current;
    return current->pair;
}

Pair * nextTreeMap(TreeMap * tree)
{
    if (tree == NULL || tree->current == NULL) return NULL;
    if (tree->current->right != NULL) {
        tree->current = minimum(tree->current->right);
    }
    else {
        TreeNode *parent = tree->current->parent;
        while (parent != NULL && tree->current == parent->right) {
            tree->current = parent;
            parent = parent->parent;
        }
        tree->current = parent;
    }
    if (tree->current != NULL)
        return tree->current->pair;
    else
        return NULL;
}
