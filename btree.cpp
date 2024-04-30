#include "btree.hpp"

#ifndef B_TREE_CPP
#define B_TREE_CPP

TreeMember::TreeMember(int ID, string name, string birthDate, string streetAddress, string currentState, string cityName, int zipcode){
    this->ID = ID;
    this->name = name;
    this->birthDate = birthDate;
    this->streetAddress = streetAddress;
    this->currentState = currentState;
    this->cityName = cityName;
    this->zipcode = zipcode;
    this->left = nullptr;
    this->right = nullptr;
}

void TreeMember::displayMember(){
    cout << "ID: " << this->ID;
    cout << "\nName: " << this->name;
    cout << "\nBirthday: " << this->birthDate;
    cout << "\nAddress: " << this->streetAddress << ", " << this->cityName << ", " << this->currentState << ", " << this->zipcode << endl;
}

void TreeMember::updateMember(string name, string birthDate, string streetAddress, string currentState, string cityName, int zipcode){
    this->name = name;
    this->birthDate = birthDate;
    this->streetAddress = streetAddress;
    this->currentState = currentState;
    this->cityName = cityName;
    this->zipcode = zipcode;
}

AVLTree::AVLTree()
{
    root = nullptr;
}

TreeMember* AVLTree::get_root(){
    return this->root;
}

void AVLTree::update_root(TreeMember* newRoot){
    this->root = newRoot;
}

TreeMember* AVLTree::search_with_ID(int searchID){
    TreeMember* currNode = this->root;
    while(currNode != nullptr){
        if(searchID == currNode->ID){
            return currNode;
        }else if(currNode->ID < searchID){
            currNode = currNode->right;
        }else{
            currNode = currNode->left;
        }
    }
    return nullptr;
}



/*void AVLTree::remove(int searchID){
    root = remove(root, searchID);
}
*/

int AVLTree::get_height(TreeMember* targetMember){
    int height = 0;
    if(targetMember != nullptr){
        int left_height = get_height(targetMember->left);
        int right_height = get_height(targetMember->right);
        int max_height = max(left_height, right_height);
        height = max_height + 1;
    }
    return height;
}

int AVLTree::get_difference(TreeMember* targetMember){
    int left_height = get_height(targetMember->left);
    int right_height = get_height(targetMember->right);
    int balance_factor = left_height - right_height;
    return balance_factor;
}

TreeMember* AVLTree::rr_rotate(TreeMember* parentNode){
    TreeMember* tempNode = parentNode->right;
    parentNode->right = tempNode->left;
    tempNode->left = parentNode;
    return tempNode;
}

TreeMember* AVLTree::ll_rotate(TreeMember* parentNode){
    TreeMember* tempNode = parentNode->left;
    parentNode->left = tempNode->right;
    tempNode->right = parentNode;
    return tempNode;
}

TreeMember* AVLTree::lr_rotate(TreeMember* parentNode){
    TreeMember* tempNode = parentNode->left;
    parentNode->left = rr_rotate(tempNode);
    return ll_rotate(parentNode);
}

TreeMember* AVLTree::rl_rotate(TreeMember* parentNode){
    TreeMember* tempNode = parentNode->right;
    parentNode->right = ll_rotate(tempNode);
    return rr_rotate(parentNode);
}


TreeMember* AVLTree::balance(TreeMember* targetMember){
    int balance_factor = get_difference(targetMember);
    if(balance_factor > 1){
        if(get_difference(targetMember->left) >= 0){
            targetMember = ll_rotate(targetMember);
        }
        else{
            targetMember = lr_rotate(targetMember);
        }
    }
    else if(balance_factor < -1){
        if(get_difference(targetMember->right) > 0){
            targetMember = rl_rotate(targetMember);
        }
        else{
            targetMember = rr_rotate(targetMember);
        }
    }
    return targetMember;
}

TreeMember* AVLTree::insert(TreeMember* currRoot,TreeMember* memberToAdd){
    if(currRoot == nullptr){
        currRoot = memberToAdd;
        return currRoot;
    }else if(memberToAdd->ID < currRoot->ID){
        currRoot->left = insert(currRoot->left, memberToAdd);
        currRoot = balance(currRoot);
    }else{
        currRoot->right = insert(currRoot->right, memberToAdd);
        currRoot = balance(currRoot);
    }
    return currRoot;//in the end it will return the new root node
}

// Function to find the node with minimum value in a tree rooted at currRoot
TreeMember* AVLTree::minValueNode(TreeMember* currRoot){
    TreeMember* current = currRoot;

    // Loop down to find the leftmost leaf
    while(current->left != nullptr){
        current = current->left;
    }
    return current;
}



TreeMember* AVLTree::remove(TreeMember* currRoot, int searchID){ 
    if (currRoot == NULL) 
        return currRoot; 
 
    // If the key to be deleted is smaller 
    // than the root's key, then it lies
    // in left subtree 
    if ( searchID < currRoot->ID ) 
        root->left = remove(currRoot->left, searchID); 
 
    // If the key to be deleted is greater 
    // than the root's key, then it lies 
    // in right subtree 
    else if( searchID < currRoot->ID ) 
        root->right = remove(currRoot->right, searchID); 
 
    // if key is same as root's key, then 
    // This is the node to be deleted 
    else
    { 
        // node with only one child or no child 
        if( (currRoot->left == NULL) ||
            (currRoot->right == NULL) ) 
        { 
            TreeMember* temp = currRoot->left ? 
                         currRoot->left : 
                         currRoot->right; 
 
            // No child case 
            if (temp == NULL) 
            { 
                temp = currRoot; 
                currRoot = NULL; 
            } 
            else // One child case 
            *currRoot = *temp; // Copy the contents of 
                           // the non-empty child 
            delete temp;
        } 
        else
        { 
            // node with two children: Get the inorder 
            // successor (smallest in the right subtree) 
            TreeMember* temp = minValueNode(currRoot->right); 
 
            // Copy the inorder successor's 
            // data to this node 
            currRoot->ID = temp->ID; 
 
            // Delete the inorder successor 
            currRoot->right = remove(currRoot->right, temp->ID); 
        } 
    } 
 
    // If the tree had only one node
    if (currRoot == NULL) 
    return currRoot; 
 
 
    // THIS NODE (to check whether this 
    // node became unbalanced) 
    int balance = get_difference(currRoot);  
 
    // Left Left Case 
    if (balance > 1 && 
        get_difference(currRoot->left) >= 0) 
        return rightRotate(currRoot); 
 
    // Left Right Case 
    if (balance > 1 && 
        get_difference(currRoot->left) < 0) 
    { 
        currRoot->left = leftRotate(currRoot->left); 
        return rightRotate(currRoot); 
    } 
 
    // Right Right Case 
    if (balance < -1 && 
        get_difference(currRoot->right) <= 0) 
        return leftRotate(currRoot); 
 
    // Right Left Case 
    if (balance < -1 && 
        get_difference(currRoot->right) > 0) 
    { 
        currRoot->right = rightRotate(currRoot->right); 
        return leftRotate(currRoot); 
    } 
 
    return currRoot; 
} 





TreeMember* AVLTree::rightRotate(TreeMember *y){ 
    TreeMember *x = y->left; 
    TreeMember *T2 = x->right; 
 
    // Perform rotation 
    x->right = y; 
    y->left = T2; 
 
    return x; 
} 
  
TreeMember* AVLTree::leftRotate(TreeMember *x){ 
    TreeMember *y = x->right; 
    TreeMember *T2 = y->left; 
 
    // Perform rotation 
    y->left = x; 
    x->right = T2; 
 
    return y; 
} 


#endif