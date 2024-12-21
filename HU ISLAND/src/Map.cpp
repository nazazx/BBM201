#include "Map.h"

#include <algorithm>
using namespace std;
Map::Map()
{
    this->root = nullptr;
}


Map::~Map() {
    if (root == nullptr) {
        return;
    }

    std::queue<MapNode*> q;
    q.push(root);

    while (!q.empty()) {
        MapNode* current = q.front();
        q.pop();
        if (current) {


        if (current->left) {
            q.push(current->left);
        }
        if (current->right) {
            q.push(current->right);
        }


        delete current;
        }
    }


}




void Map::initializeMap(std::vector<Isle *> isles)
{

    for (Isle *isle:isles) {
        insert(isle);


    }
    rock=true;
    int post=0,pre=0;
    postOrderItemDrop(root,post);
    preOrderItemDrop(root,pre);


    // TODO: Insert innitial isles to the tree
    // Then populate with Goldium and Einstainium items
}

MapNode *Map::rotateRight(MapNode *current) {
    if (current == nullptr || current->left == nullptr) {
        std::cerr << "[Right Rotation] Called on invalid node!" << std::endl;
        return current;
    }

    MapNode *left = current->left;
    MapNode *temp=left->right;
    left->right = current;
    current->left = temp;




        current->height = 1 + std::max(height(current->left), height(current->right));


        left->height = 1 + std::max(height(left->left), height(left->right));



    return left;


    // TODO: Perform right rotation according to AVL
    // return necessary new root

}

MapNode *Map::rotateLeft(MapNode *current) {
    if (current == nullptr || current->right == nullptr) {
        std::cerr << "[Left Rotation] Called on invalid node!" << std::endl;
        return current;
    }

    MapNode *right = current->right;
    MapNode *temp=right->left;
    right->left = current;
    current->right = temp;



        current->height = 1 + std::max(height(current->left), height(current->right));


        right->height = 1 + std::max(height(right->left), height(right->right));


    return right;



    // TODO: Perform left rotation according to AVL
    // return necessary new root
}

int Map::calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth)
{
    return (int)totalMapDepth * ((double)playerDepth / totalShaperTreeHeight);
}

int Map::height(MapNode *node)
{

    if(node==nullptr) {
        return 0;
    }
    // TODO: Return height of the node
    return node->height;
}

MapNode *Map::insert(MapNode *node, Isle *isle) {
    MapNode *newNode = nullptr;

    if (node == nullptr) {
        newNode = new MapNode(isle);
        return newNode;
    }

    if (*isle < *(node->isle)) {
        node->left = insert(node->left, isle);
    } else if (*isle > *(node->isle)) {
        node->right = insert(node->right, isle);
    } else {
        return node;
    }

    node->height = 1 + std::max(height(node->left), height(node->right));


    int diff = height(node->left) - height(node->right);

    if((diff<-1 || diff>1)&&rock) {
     balanced=true;
    }

    if ( diff > 1 && node->left != nullptr && *isle < *(node->left->isle)) {
        return rotateRight(node);
    }

    if ( diff < -1 && node->right != nullptr && *isle > *(node->right->isle)) {
        return rotateLeft(node);
    }

    if ( diff > 1 && node->left != nullptr&&*isle > *(node->left->isle)) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if ( diff < -1 &&node->right != nullptr&& *isle < *(node->right->isle)) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}




void Map::insert(Isle *isle)
{
    root = insert((root), isle);
    if (balanced) {
        balanced=false;
        counter++;
        if (counter%3==0) {
            populateWithItems();
        }
    }
    // you might need to insert some checks / functions here depending on your implementation
}

MapNode* Map::min(MapNode* node) {
    while (node && node->right != nullptr) {
        node = node->right;
    }
    return node;
}


vector<MapNode*> global;

void Map::pointer(MapNode *node) {
    if (root == nullptr) {
        return;
    }

    std::queue<MapNode*> q;
    q.push(root);

    while (!q.empty()) {
        MapNode* current = q.front();
        q.pop();
        if (current) {
            if (current->left==node) {
                current->left=nullptr;
            }
            if (current->right==node) {
                current->right=nullptr;
            }

            if (current->left) {
                q.push(current->left);
            }
            if (current->right) {
                q.push(current->right);
            }


        }
    }
}

MapNode *Map::remove(MapNode *node, Isle *isle)
{

    if (!node)
    {
        std::cout << "[Remove] Tree is Empty" << std::endl;
        return nullptr;
    }

    if (isle->getName() < node->isle->getName())
    {
        node->left = remove(node->left, isle);
    }
    else if (isle->getName() > node->isle->getName())
    {
        node->right = remove(node->right, isle);
    }
    else
    {

        if (node->left == nullptr || node->right == nullptr)
        {
            MapNode *temp = (node->left) ? node->left : node->right;
            delete node;
            return temp;
        }
        else
        {

            MapNode *successor = min(node->left);
            if (successor) {
                node->isle = new Isle(*successor->isle);
                node->left = remove(node->left, successor->isle);
            }

        }
    }


    node->height = std::max(height(node->left), height(node->right)) + 1;

    int balance = height(node->left) - height(node->right);

    if((balance<-1 || balance>1)&&rock) {
        balanced=true;
    }

    if (balance > 1)
    {
        if (height(node->left->left) >= height(node->left->right))
            return rotateRight(node);
        else
        {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
    }

    if (balance < -1)
    {
        if (height(node->right->right) >= height(node->right->left))
            return rotateLeft(node);
        else
        {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
    }

    return node;

}

void Map::remove(Isle *isle)
{
    root = remove(root, isle);
    if (balanced) {
        balanced=false;
        counter++;
        if (counter%3==0) {
            populateWithItems();
        }
    }
/*
    for (int i=0;i<global.size();i++) {
        pointer(global[i]);
        if (global[i]) {
            delete global[i];
        }
    }
    */
}


void Map::preOrderItemDrop(MapNode *current, int &count)
{
    if(current==nullptr) {
        return;
    }
    count++;
    if (count%5==0) {
        current->isle->setItem(EINSTEINIUM);
        std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }

    preOrderItemDrop(current->left,count);
    preOrderItemDrop(current->right,count);



    // TODO: Drop EINSTEINIUM according to rules
}

// to Display the values by Post Order Method .. left - right - node
void Map::postOrderItemDrop(MapNode *current, int &count)
{
    if(current==nullptr) {
        return;
    }
    postOrderItemDrop(current->left,count);
    postOrderItemDrop(current->right,count);
    count++;
    if (count%3==0) {
        current->isle->setItem(GOLDIUM);
        std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;

    }

    // TODO: Drop GOLDIUM according to rules
}

MapNode *Map::findFirstEmptyIsle(MapNode *node)
{
    if (node == nullptr) return nullptr;

    queue<MapNode *> q;
    q.push(node);

    while (!q.empty()) {
        MapNode *temp=q.front();
        q.pop();

        if(temp->isle->getItem()==EMPTY) {
            return temp;
        }

        if(temp->left) {
            q.push(temp->left);
        }
        if(temp->right) {
            q.push(temp->right);
        }
    }
    // TODO: Find first Isle with no item
}
void Map::dropItemBFS()
{
    if (root == nullptr) {
        std::cout << "[BFS Drop] Tree is empty, no item can be dropped." << std::endl;
        return;
    }

    queue<MapNode *> q;
    q.push(root);


        while (!q.empty()) {
            MapNode *temp = q.front();
            q.pop();

            if (temp == nullptr || temp->isle == nullptr) {
                continue;
            }

            if (temp->isle->getItem() == EMPTY) {
                temp->isle->setItem(AMAZONITE);
                std::cout << "[BFS Drop] AMAZONITE dropped on Isle: " << temp->isle->getName() << std::endl;
                return;
            }


            if (temp->left) {
                q.push(temp->left);
            }
            if (temp->right) {
                q.push(temp->right);
            }
        }
        std::cout << "[BFS Drop] No eligible Isle found for AMAZONITE drop." << std::endl;

}



void Map::displayMap()
{
    std::cout << "[World Map]" << std::endl;
    display(root, 0, 0);
}

int Map::getDepth(MapNode *node)
{
    if (root == nullptr || node == nullptr) {
        return -1;
    }
    MapNode *curr = root;
    int depth = 0;

    while (curr != nullptr) {
        if (node->isle->getName() == curr->isle->getName()) {
            return depth;
        } else if (node->isle->getName() < curr->isle->getName()) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
        depth++;
    }

    // TODO: Return node depth if found, else
    return -1;
}

// Function to calculate the depth of a specific node in the AVL tree
int Map::getIsleDepth(Isle *isle)
{
    MapNode *node=findNode(*isle);
    if (node == nullptr) {
        return -1;
    }
    // TODO: Return node depth by isle if found, else
    return getDepth(node);
}

int Map::getDepth()
{

    // TODO: Return max|total depth of tree
    return height(root)-1;
}

void Map::populateWithItems()
{
    int post=0,pre=0;
    postOrderItemDrop(root,post);
    preOrderItemDrop(root,pre);
    dropItemBFS();


    // TODO: Distribute fist GOLDIUM than EINSTEINIUM
}

Isle *Map::findIsle(Isle isle)
{
    // TODO: Find isle by value
    MapNode *curr = root;

    while (curr != nullptr)
    {
        if (*(curr->isle) == isle)
            return curr->isle;
        else if (isle < *(curr->isle))
            curr = curr->left;
        else
            curr = curr->right;
    }
    return nullptr;

}

Isle *Map::findIsle(std::string name)
{
    MapNode *temp=root;
    while (temp) {
        if(temp->isle->getName()<name) {
            temp=temp->right;
        }
        else if(temp->isle->getName()>name) {
            temp=temp->left;
        }
        else {
            return temp->isle;
        }
    }
    // TODO: Find isle by name
    return nullptr;
}

MapNode *Map::findNode(Isle isle)
{
    MapNode *curr = root;

    while (curr != nullptr)
    {
        if (*(curr->isle) == isle)
            return curr;
        else if (isle < *(curr->isle))
            curr = curr->left;
        else
            curr = curr->right;
    }


    // TODO: Find node by value
    return nullptr;
}

MapNode *Map::findNode(std::string name)
{
    // TODO: Find node by name
    MapNode *temp=root;
    while (temp) {
        if(temp->isle->getName()<name) {
            temp=temp->right;
        }
        else if(temp->isle->getName()>name) {
            temp=temp->left;
        }
        else {
            return temp;
        }
    }
    return nullptr;
}

void Map::display(MapNode *current, int depth, int state)
{

    // SOURCE:
    if (current==nullptr||current->left==current||current->right==current) {

        return;
    }
    if (current->left)
        display(current->left, depth + 1, 1);

    for (int i = 0; i < depth; i++)
        printf("     ");

    if (state == 1) // left
        printf("   ┌───");
    else if (state == 2) // right
        printf("   └───");

    std::cout << "[" << *current->isle << "] - (" << current->height << ")\n"
              << std::endl;

    if (current->right)
        display(current->right, depth + 1, 2);


}

void helper(ofstream &file,MapNode *node,int level) {
    if(node==nullptr) {
        file<<"NULL ";
        return;
    }
    if (level==0) {
        file<<node->isle->getName()<<" ";
    }
    else {
        helper(file,node->left,level-1);
        helper(file,node->right,level-1);
    }
}
void Map::writeToFile(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        return;
    }
    // TODO: Write the tree to filename output level by level
    int i=0;
    int depth=getDepth();
    while (i<=depth) {
        helper(file,root,i);
        i++;
        file<<endl;
    }
    file.close();
}

void Map::writeIslesToFile(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        return;
    }
    vector<string> s;
    queue<MapNode *> q;
    q.push(root);
    while (!q.empty()) {
        MapNode *curr=q.front();
        q.pop();
        s.push_back(curr->isle->getName());
        if(curr->left) {
            q.push(curr->left);
        }
        if(curr->right) {
            q.push(curr->right);
        }
    }
    sort(s.begin(),s.end());
    for (const std::string &name : s) {
        file << name << endl;
    }
    file.close();
    // TODO: Write Isles to output file in alphabetical order
    // Use std::cout << "[Output] " << "Isles have been written to " << filename << " in in alphabetical order." << std::endl;
}