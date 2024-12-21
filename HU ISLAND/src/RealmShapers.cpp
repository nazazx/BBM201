#include "RealmShapers.h"
#include <cmath>
#include <algorithm>
#include <condition_variable>
#include <queue>
using namespace std;
ShaperTree::ShaperTree()
{
}

ShaperTree::~ShaperTree()
{
    for (RealmShaper *curr:realmShapers) {
        delete curr;
    }
    for (RealmShaper *cur:afterDelete) {
        delete cur;
    }

    // TODO: Free any dynamically allocated memory if necessary
}

void ShaperTree::initializeTree(std::vector<RealmShaper *> shapers)
{
    for (RealmShaper *curr:shapers) {
        insert(curr);
    }

    // TODO: Insert innitial shapers to the tree
}

int ShaperTree::getSize()
{
    int n=realmShapers.size();
    // TODO: Return number of shapers in the tree
    return n;
}

std::vector<RealmShaper *> ShaperTree::getTree()
{
    return realmShapers;
}

bool ShaperTree::isValidIndex(int index)
{
    bool isValid = false;
    int n=realmShapers.size();
    if(index<n&& index>=0) {
        isValid=true;
    }
    // TODO: Check if the index is valin in the tree

    return isValid;
}

void ShaperTree::insert(RealmShaper *shaper)
{
    realmShapers.push_back(shaper);
    // TODO: Insert shaper to the tree
}

int ShaperTree::remove(RealmShaper *shaper)
{
    int index=findIndex(shaper);

    if(index!=-1) {
        afterDelete.push_back(realmShapers[index]);
        realmShapers.erase(realmShapers.begin()+index);
        return index;
    }
    // TODO: Remove the player from tree if it exists
    // Make sure tree protects its form (complate binary tree) after deletion of a node
    // return index if found and removed
    // else
    return -1;
}

int ShaperTree::findIndex(RealmShaper *shaper)
{
    if (shaper == nullptr) {
        return -1;
    }


    for (int i = 0; i < realmShapers.size(); i++) {
        if (realmShapers[i] == shaper) {
            return i;
        }
    }

    return -1;
}

int ShaperTree::getDepth(RealmShaper *shaper)
{
    int n=findIndex(shaper);
    int depth = floor(std::log2(n + 1));

    // return depth of the node in the tree if found
    // else
    if(n!=-1) {
        return depth;
    }
    return -1;
}

int ShaperTree::getDepth()
{
    RealmShaper * shaper=realmShapers.back();

    // return total|max depth|height of the tree
    return getDepth(shaper) ;
}

RealmShaper ShaperTree::duel(RealmShaper *challenger, bool result)
{
    RealmShaper *parent=getParent(challenger);
    if (!parent) {
        return *challenger;
    }
    RealmShaper *loser=nullptr;
    RealmShaper *winner=nullptr;
    if(result) {
        winner=challenger;
        loser=parent;
        parent->loseHonour();
        challenger->gainHonour();
        replace(challenger,parent);
    }
    else {
        winner=parent;
        loser=challenger;
        winner->gainHonour();
        loser->loseHonour();
    }

    // TODO: Implement duel logic, return the victor
    if (winner==challenger) {
        std::cout << "[Duel] " << challenger->getName() << " won the duel" << std::endl;
        std::cout << "[Honour] " << "New honour points: ";
        std::cout << challenger->getName() << "-" << challenger->getHonour() << " ";
        std::cout << parent->getName() << "-" << parent->getHonour() << std::endl;
    }
    else {
        std::cout << "[Duel] " << challenger->getName() << " lost the duel" << std::endl;
        std::cout << "[Honour] " << "New honour points: ";
        std::cout << challenger->getName() << " - " << challenger->getHonour() << " ";
        std::cout << parent->getName() << " - " << parent->getHonour() << std::endl;
    }


    if(loser->getHonour()<=0) {
        remove(loser);
        std::cout << "[Duel] " << loser->getName() << " lost all honour, delete" << std::endl;
    }
    return *winner;

}

RealmShaper *ShaperTree::getParent(RealmShaper *shaper)
{
    RealmShaper *parent = nullptr;
int index=findIndex(shaper);
    if (index < 0) {
        return nullptr;
    }
    if (index==0) {
        parent=shaper;
    }
    else {
        parent=realmShapers[(index-1)/2];
    }

    // TODO: return parent of the shaper

    return parent;
}

void ShaperTree::replace(RealmShaper *player_low, RealmShaper *player_high)
{
    if(player_high==getParent(player_low)) {
        int index1=findIndex(player_low);
        int index2=findIndex(player_high);
        if(index1!=-1 && index2!=-1) {
            std::swap(realmShapers[index1], realmShapers[index2]);
        }

    }
    // TODO: Change player_low and player_high's positions on the tree
}

RealmShaper *ShaperTree::findPlayer(RealmShaper shaper)
{
    RealmShaper *foundShaper = nullptr;
    for (RealmShaper *curr:realmShapers) {
        if((*curr)==shaper) {
            foundShaper=curr;
        }
    }
    // TODO: Search shaper by object
    // Return the shaper if found
    // Return nullptr if shaper not found

    return foundShaper;
}

// Find shaper by name
RealmShaper *ShaperTree::findPlayer(std::string name)
{
    RealmShaper *foundShaper = nullptr;
    for (RealmShaper *curr:realmShapers) {
        if(curr->getName()==name) {
            foundShaper=curr;
        }
    }
    // TODO: Search shaper by name
    // Return the shaper if found
    // Return nullptr if shaper not found

    return foundShaper;
}

std::vector<std::string> ShaperTree::inOrderTraversal(int index)
{
    std::vector<std::string> result = {};

    if(index>=realmShapers.size()) {
        return {};
    }
    vector<string> left=inOrderTraversal(2*index+1);
    result.insert(result.end(),left.begin(),left.end());
    result.push_back(realmShapers[index]->getName());

    vector<string> right=inOrderTraversal(2*index+2);
    result.insert(result.end(),right.begin(),right.end());
    // TODO: Implement inOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation

    // Note: Since SheperTree is not an binary search tree,
    // in-order traversal will not give rankings in correct order
    // for correct order you need to implement level-order traversal
    // still you are to implement this function as well
    return result;
}

std::vector<std::string> ShaperTree::preOrderTraversal(int index)
{
    std::vector<std::string> result = {};

    if(index>=realmShapers.size()) {
        return {};
    }
    result.push_back(realmShapers[index]->getName());

    vector<string> left=preOrderTraversal(2*index+1);
    result.insert(result.end(),left.begin(),left.end());

    vector<string> right=preOrderTraversal(2*index+2);
    result.insert(result.end(),right.begin(),right.end());

    // TODO: Implement preOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation
    return result;
}

std::vector<std::string> ShaperTree::postOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    if(index>=realmShapers.size()) {
        return {};
    }

    vector<string> left=postOrderTraversal(2*index+1);
    result.insert(result.end(),left.begin(),left.end());

    vector<string> right=postOrderTraversal(2*index+2);
    result.insert(result.end(),right.begin(),right.end());

    result.push_back(realmShapers[index]->getName());

    // TODO: Implement postOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation
    return result;
}

void ShaperTree::preOrderTraversal(int index, std::ofstream &outFile)
{
    vector<string> vec=preOrderTraversal(index);
    for(string s:vec) {
        outFile<<s<<endl;
    }

    // TODO: Implement preOrderTraversal in tree
    // write nodes to output file

    // Define and implement as many helper functions as necessary for recursive implementation
}

void ShaperTree::breadthFirstTraversal(std::ofstream &outFile)
{
    for (RealmShaper *curr:realmShapers) {

        outFile<<curr->getName()<<endl;


    }
    // TODO: Implement level-order traversal
    // write nodes to output file

    // Define and implement as many helper functions as necessary
}

void ShaperTree::displayTree()
{
    std::cout << "[Shaper Tree]" << std::endl;
    printTree(0, 0, "");
}

// Helper function to print tree with indentation
void ShaperTree::printTree(int index, int level, const std::string &prefix)
{
    if (!isValidIndex(index))
        return;

    std::cout << prefix << (level > 0 ? "   └---- " : "") << *realmShapers[index] << std::endl;
    int left = 2*index+1;  // TODO: Calculate left index
    int right = 2*index+2; // TODO: Calculate right index

    if (isValidIndex(left) || isValidIndex(right))
    {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : "")); // ╎
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}

void ShaperTree::writeShapersToFile(const std::string &filename)
{
    // TODO: Write the shapers to filename output level by level
     //std::cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << std::endl;
ofstream file(filename);
    if (!file.is_open()) {
        return;
    }
    breadthFirstTraversal(file);
    file.close();
}

void ShaperTree::writeToFile(const std::string &filename)
{
    // TODO: Write the tree to filename output pre-order
    // std::cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;
    ofstream file(filename);
    if (!file.is_open()) {
        return;
    }
    preOrderTraversal(0,file);
    file.close();
}
