#ifndef __MYBST_H__
#define __MYBST_H__

#include <iostream>
#include <fstream>
#include <cstdlib>

//g++ --std=c++11 MainTest.cpp -o Lab7 

// for i in {0..9}; do
//     ./Lab7 Inputs/input_$i.txt > output_$i.txt
//     python3 GradingScript.py output_$i.txt Outputs/output_$i.txt
// done

// for i in {0..9}; do
//     printf $i" my output vs given output line be line:\n"
//     diff output_$i.txt Outputs/output_$i.txt
//     printf "\n\n"
// done

enum TreeTravOrderType
{   PreOrder,
    InOrder,
    PostOrder
};

enum NodePosType
{   Left,
    Right,
    Root
};

template <typename ComparableType>
class MyBST
{
  private:
    struct BinaryNode
    {   
        ComparableType element;
        size_t height;
        BinaryNode *left;
        BinaryNode *right;
        
        BinaryNode(const ComparableType & x, const size_t h, BinaryNode *l, BinaryNode *r) : 
            element{x}, 
            height{h},
            left{l}, 
            right{r} 
        { ;}

        BinaryNode(ComparableType && x, const size_t h, BinaryNode *l, BinaryNode *r) : 
            element{std::move(x)}, 
            height{h},
            left{l},
            right{r}
        { ;}
    };

    BinaryNode *root;       // the root node
    size_t theSize;         // the number of data elements in the tree

    BinaryNode* findMin(BinaryNode *t) const // finds the minimum data element from the tree rooted at t
    {   if (t!=nullptr) 
        {   while (t->left!=nullptr) //until left becomes null traverse all the way to the left 
                t=t->left; 
        }
        return t; //then return the left-most element 
    }

    BinaryNode* findMax(BinaryNode *t) const // finds the maximum data element from the tree rooted at t
    {   if (t!=nullptr) 
        {   while(t->right!=nullptr) //until right becomes null traverse all the way to the right 
                t=t->right; 
        }
        return t; //then return the right-most element 
    }

    bool contains(const ComparableType& x, BinaryNode *t) const // checks if x is contained in the tree rooted at t
    {   bool found=false; 
        if (t==nullptr) ; //if tree given is null do nothing 
        else if(x<t->element) //if x is smaller than current go left 
            found=contains(x, t->left); 
        else if (x>t->element) 
            found=contains(x, t->right); //if x is bigger than current go right 
        else found=true; //match found 
        return found; 
    }

    void clear(BinaryNode*& t) // deletes the tree rooted at t
    {   if (t) //if t exists recursivley traverse left/right and when you cant anymore, delete node 
        {   clear(t->left); 
            clear(t->right); 
            delete t; 
            t=nullptr; 
        }
    }
    
    size_t height(BinaryNode* t) const // returns the height of the node; leaf node have height of 1 
    {   return (t == nullptr ? 0 : t->height);    } //if tree is null return 0 otherwise return height 

    void balance(BinaryNode*& t) // balances tree node t
    {   if (t==nullptr) return; //if empty root of subtree do nothing 
        if (height(t->left) > height(t->right)+1) //if height of LST>RST+1 check if LST scewed to the left 
        {   if (height(t->left->left)>=height(t->left->right)) rotateLeft(t); // case 1 
            else doubleRotateLeft(t); //case 3 LST scewed to the right 
        }
        else if (height(t->right) > height(t->left) + 1) //if RST>LST+1 check if LST scewed to the right 
        {   if(height(t->right->right)>=height(t->right->left)) rotateRight(t); // case 2 
            else doubleRotateRight(t); // case 4 LST scewed to the left 
        } 
        t->height =1+(height(t->left)>height(t->right) ? height(t->left) : height(t->right)); //new height adjusted 
        return; 
    }
    
    void rotateLeft(BinaryNode*& t) // single rotation that reduces left branch depth
    {   BinaryNode* tl=t->left; //saves LST 
        t->left=tl->right; //currents LST is now saved's RST 
        tl->right=t; //save's RST is now current 
        t->height=1+(height(t->left) > height(t->right) ? height(t->left) : height(t->right)); //adjust the height of the now subtree (RST of saved)
        tl->height=1+(height(tl->left) > height(tl->right) ? height(tl->left) : height(tl->right)); //adjust height of saved 
        t=tl; //t is now the updated tree 
        return; 
    }

    void rotateRight(BinaryNode*& t) // single rotation that reduces right branch depth
    {   BinaryNode* tr=t->right; 
        t->right=tr->left; 
        tr->left=t; 
        t->height = 1+(height(t->left) > height(t->right) ? height(t->left) : height(t->right)); 
        tr->height = 1+(height(tr->left) > height(tr->right) ? height(tr->left) : height(tr->right)); 
        t=tr; 
        return; 
    }

    void doubleRotateLeft(BinaryNode*& t) // double rotation that reduces left branch depth
    {   rotateRight(t->left); 
        rotateLeft(t); 
        return; 
    }
    
    void doubleRotateRight(BinaryNode*& t) // double rotation that reduces right branch depth
    {   rotateLeft(t->right); 
        rotateRight(t); 
        return; 
    }

    void insert(const ComparableType& x, BinaryNode*& t) // inserts x to the tree rooted at t (copy)
    {   if (t==nullptr) //if we find leaf node to insert at... 
        {   t=new BinaryNode{x,1,nullptr,nullptr}; //...make new node and takes x element, height, andmakes it leaf node 
            ++theSize; //increase size since we made new node 
        } 
        else if(x < t->element) insert(x, t->left); //recurse left if inserted is smaller than current 
        else if(x > t->element) insert(x, t->right); //recurse right if inserted is greater than current 
        else ; //if inserrted node is duplicate, do nothing 
        balance(t); //since we inserted new check if we need to balance (balance checks and does balance if needed)
        return; 
    }

    void insert(ComparableType && x, BinaryNode*& t) // inserts x to the tree rooted at t (move)
    {   if (t==nullptr) //if we find leaf node to insert at... 
        {   t=new BinaryNode{std::move(x),1,nullptr,nullptr}; //...make new node and moves x element, height, andmakes it leaf node 
            ++theSize; //increase size since we made new node 
        } 
        else if(x < t->element) insert(std::move(x), t->left); //recurse left if inserted is smaller than current 
        else if(x > t->element) insert(std::move(x), t->right); //recurse right if inserted is greater than current 
        else ; //duplicate, do nothing 
        balance(t); 
        return; 
    }

    void remove(const ComparableType& x, BinaryNode*& t) // removes x from the tree rooted at t; when deleting a node with two children, replace it with the smallest child of the right subtree 
    {   if (t==nullptr) return; //item not found; do nothing 
        if (x<t->element) remove(x, t->left); //if x smaller than current go left 
        else if (x>t->element) remove(x, t->right); //if x greater than current go right 
        else if (t->left!=nullptr && t->right!=nullptr) //if x=current and has 2 children...
        {   t->element=findMin(t->right)->element; //...update current's element to be the min of the RST 
            remove(t->element, t->right); //remove the min of RST since we just copied its element (moved it up)
        }
        else //if x=current one child case 
        {   BinaryNode *oldNode=t; //save current 
            t= (t->left!= nullptr) ? t->left : t->right; //if left not null current becomes left, otherwise right 
            delete oldNode; 
            theSize--; 
        }
        balance(t); //check if the tree needs to be balanced and balance if needed 
        return; 
    }

    BinaryNode* clone(BinaryNode* t) const // clones the node t and returns the clone
    {   if(t==nullptr) return nullptr;
        else
            return new BinaryNode{t->element, t->height, clone(t->left), clone(t->right)};
    }

    void printPreOrder(BinaryNode* t, std::ostream& out) const // prints all data elements rooted at t pre-order
    {   const char delim = ' ';
        if(t != nullptr) //if current not null 
        {   out << t->element<< delim; //send out element with space delimiter 
            printPreOrder(t->left, out); //recurse left 
            printPreOrder(t->right, out); //recurse right 
        }
        return;
    }

    void printInOrder(BinaryNode* t, std::ostream& out) const    // prints all data elements rooted at t in-order
    {   const char delim = ' ';
        if(t != nullptr) //if current not null 
        {   printInOrder(t->left, out); //recurse left 
            out << t->element<< delim; //this is std::cout << t->element; in vid wo const char delim= ' '; 
            printInOrder(t->right, out); //recurse right 
        }
        return;
    }

    void printPostOrder(BinaryNode* t, std::ostream& out) const // prints all data elements rooted at t post-order
    {   const char delim = ' '; 
        if(t != nullptr) //if current not null 
        {   printPostOrder(t->left, out); //recurse left  
            printPostOrder(t->right, out); //recurse right 
            out << t->element << delim; //send out element with space delimiter 
        }
        return;
    }

    // prints the subtree rooted at t
    // lv is the level of t (the root corresponds to level 0)
    // p is the relative topological position of t (left/right/root) 
    void printTree(BinaryNode* t, const int lv, const NodePosType p)
    {   if(t == nullptr) //if current null do nothing 
            return;

        char pos;
        switch(p) //gets position of the node and prints if its LST, RST, or root 
        {   case Left:
                pos = 'L';
                break;
            case Right:
                pos = 'R';
                break;
            case Root:
                pos = 'T';
                break;
            default:
                std::cout << "Error: MyBST::printTree: unrecognized position type." << std::endl; 
        }        
        std::cout << t->element << "|" << lv << "|" << pos << std::endl; //prints element, height/level, then right/left/tree position 
        printTree(t->left, lv + 1, Left); //recurses left 
        printTree(t->right, lv + 1, Right); //recurses right  
        return;
    }

  public:

    
    MyBST() : // default constructor
        root{nullptr},
        theSize{0}
    { ;}

    MyBST(const MyBST& rhs) : // copy constructor
        root{nullptr},
        theSize{rhs.theSize}
    {   root = clone(rhs.root); }

    MyBST(MyBST && rhs) : // move constructor
        root{rhs.root},
        theSize{rhs.theSize}
    {   rhs.root = nullptr;   }
      
    ~MyBST() // destructor
    {   clear(); }

    const ComparableType& findMin() const // finds the minimum data element in the tree
    {   BinaryNode* node=findMin(this->root); 
        return node->element; 
    }

    const ComparableType& findMax( ) const // finds the maximum data element in the tree
    {   BinaryNode* node=findMax(this->root); 
        return node->element; 
    }

    bool contains(const ComparableType& x) const // checks whether x is contained in the tree; just calls BST? 
    {   return contains(x, this->root);    }
    
    size_t size(void) const // returns the number of data elements in the tree
    {   return theSize;     }

    // returns the depth of the tree
    // depth defined as the longest path length from the root to any leaf
    // e.g. an empty tree has a depth of 0, a tree with a single node has a depth of 1
    size_t depth(void) const
    {   return (height(this->root)); }//idk wut else this could do other than return height since we arent given a node to measure depth of? 
    
    bool empty() const // checks whether the tree is empty
    {   return theSize==0;     }

    void clear() // delete all data elements in the tree
    {   clear(this->root); 
        root=nullptr; 
        theSize=0; 
    }

    void insert(const ComparableType& x) // insert x into the tree (copy)
    {   insert(x, this->root);    }
    
    void insert(ComparableType && x) // insert x into the tree (move)
    {   insert(x, this->root);    }

    void remove(const ComparableType& x) // removes x from the tree
    {   remove(x, this->root);    }

    MyBST& operator=(const MyBST& rhs) // copy assignment
    {   if (this!=&rhs)
        {   clear(); //clear current tree 
            root=clone(rhs.root); //make new nodes and tree and take from the given one 
            theSize=rhs.theSize; 
        }
        return *this; 
    }

    MyBST& operator=(MyBST && rhs) // move assignment
    {   if (this!=&rhs) 
        {   clear(); //clear current tree 
            root=rhs.root; //take the root 
            theSize=rhs.theSize; //take the size 
            rhs.root=nullptr; //set other's root to null since we took it 
            rhs.theSize=0; //set other's size to null since we took it 
        }
        return *this; 
    }

    bool lowestCommonAncestor(const ComparableType& x, const ComparableType& y, ComparableType& lca) // finds the lowest common ancestor (LCA) of x and y, // if x and y are both in the tree, return true and store their LCA in lca
    {   if (!contains(x, root)) //checks that x is in tree, reutrns false if not 
        {   std::cout << "x given not in tree; LCA" << std::endl; 
            return false; 
        }
        if (!contains(y, root)) //checks that y is in tree, returns false if not 
        {   std::cout << "y given not in tree; LCA" << std::endl; 
            return false; 
        }

        BinaryNode* current=root; //set current to root so we can move throughout the tree and update it 
        while (current!=nullptr)
        {   if (x<current->element && y<current->element) //if both elements are smaller than current go left 
                current=current->left; 
            else if (x>current->element && y>current->element) //if both elements are bigger than current go right  
                current=current->right; 
            else //x and y are in different subtrees; split 
            {   lca=current->element; //found LCA since we split so save it and return true 
                return true; 
            }
        }
        lca=root->element; //we went through the tree and didnt find LCA so only LCA is root and return true 
        return true; 
    }

    void print(TreeTravOrderType order, std::ostream& out = std::cout) const // print all data elements in the tree
    {   switch (order) //choose to print in pre/in/post order 
        {   case PreOrder:
                printPreOrder(root, out);
                out << std::endl;
                break;
            case InOrder:
                printInOrder(root, out);
                out << std::endl;
                break;
            case PostOrder:
                printPostOrder(root, out);
                out << std::endl;
                break;
            default:
                out << "Error: MyBST::print: Unsupported print order." << std::endl;
                break;
        }
        return;
    }

    void printTree(void) // print all data elements in the tree including partial topological information (layer and relative position among siblings)
    {   printTree(root, 0, Root);
        return;
    }
      
};

#endif // __MYBST_H__