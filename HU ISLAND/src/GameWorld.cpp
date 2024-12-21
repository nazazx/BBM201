#include "GameWorld.h"
using namespace std;
GameWorld::GameWorld() : mapTree(), shaperTree() {}

void GameWorld::initializeGame(std::vector<Isle *> places, std::vector<RealmShaper *> players)
{
    shaperTree.initializeTree(players);
    mapTree.initializeMap(places);
}

Map& GameWorld::getMapTree()
{
    return mapTree;
}
ShaperTree& GameWorld::getShaperTree()
{
    return shaperTree;
}

bool GameWorld::hasAccess(RealmShaper *realmShaper, Isle *isle)
{

    if (shaperTree.findPlayer(*realmShaper)==nullptr) {
        std::cout << "[Access Control] " << "RealmShaper not found!" << std::endl;
        return false;
    }
    bool hasAccess = false;
   if (shaperTree.findIndex(realmShaper)==0) {
       hasAccess=true;
       return hasAccess;
   }


    int depth=shaperTree.getDepth(realmShaper);
    int isle_depth=mapTree.getIsleDepth(isle);
    int calculated=mapTree.calculateMinMapDepthAccess(depth,shaperTree.getDepth(),mapTree.getDepth());


    if (isle_depth>=calculated) {
        hasAccess=true;
    }


    // TODO: Check if the realmShaper has access to explore the isle
    // Get necessary depth values
    // Use mapTree.calculateMinMapDepthAccess


    return hasAccess;
}

void GameWorld::exploreArea(RealmShaper *realmShaper, Isle *isle)
{

    if(!hasAccess(realmShaper, isle)) {
        std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << *isle <<"."<<std::endl;
        return;
    }
    if (mapTree.findIsle(*isle)!=nullptr) {

        if (realmShaper->current) {

            realmShaper->current->decreaseShaperCount();
        }
    std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;

    realmShaper->collectItem(isle->getItem());

    std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;



    isle->increaseShaperCount();
        realmShaper->current=isle;

    if (!isle->capacity_control()) {
        shaperTree.reset(isle);
        std::cout << "[Owercrowding] " << isle->getName() << "self-destructed, it will be removed from the map" << std::endl;
        mapTree.remove(isle);

    }

    }

    // TODO:
    // Check if realmShaper has access
    // If realmShaper has access
    // Visit isle, 
    // collect item, 
    // check overcrowding for Isle, 
    // delete Isle if necessary

    // Use // std::cout << "[Owercrowding] " << isle->getName() << " self-destructed, it will be removed from the map" << std::endl;

    // You will need to implement a mechanism to keep track of how many realm shapers are at an Isle at the same time
    // There are more than one ways to do this, so it has been left completely to you
    // Use shaperCount, but that alone will not be enough,
    // you will likely need to add attributes that are not currently defined
    // to RealmShaper or Isle or other classes depending on your implementation
}

void GameWorld::craft(RealmShaper *shaper, const std::string &isleName){

    if (shaper->hasEnoughEnergy()) {
        if (shaper->current) {
            shaper->current->decreaseShaperCount();
        }

        std::cout << "[Energy] " << shaper->getName()<< " has enough energy points: " << shaper->getEnergyLevel() << std::endl;


        Isle *isle=new Isle(isleName);
        shaper->current=isle;
        mapTree.insert(isle);
        shaper->loseEnergy();
        std::cout << "[Craft] " << shaper->getName() << " crafted new Isle " << isleName << std::endl;

    }
    else {
        std::cout << "[Energy] " << shaper->getName() << " does not have enough energy points: " << shaper->getEnergyLevel() << std::endl;

    }
    // TODO: Check energy and craft new isle if possible
}

void GameWorld::displayGameState()
{
/*
mapTree.displayMap();
    shaperTree.displayTree();
*/
}

// TODO: Implement functions to read and parse Access and Duel logs

void GameWorld::processGameEvents(const std::string &accessLogs, const std::string &duelLogs)
{


    std::ifstream file1(accessLogs);
    if (!file1.is_open()) {

        return;
    }
    std::ifstream file2(duelLogs);
    if (!file2.is_open()) {

        return;
    }
    string line,line2;
    getline(file1,line);
    getline(file2,line2);

    int count=0;
    while (getline(file1,line)) {
        stringstream ss(line);
        string player;
        string isle_str;
        ss>>player>>isle_str;


        RealmShaper *shaper=shaperTree.findPlayer(player);
        if (shaper == nullptr) {
            continue;
        }
        count++;
        Isle *isle = mapTree.findIsle(isle_str);

        if (isle==nullptr) {
            craft(shaper,isle_str);
        }
        else {
            exploreArea(shaper,isle);
        }
        if (count%5==0&&getline(file2,line2)) {
            stringstream ss(line2);
            string name;
            int res;
            ss>>name>>res;
            RealmShaper *shaperp=shaperTree.findPlayer(name);

            if (shaperp!=nullptr) {

                shaperTree.duel(shaperp,res);
                if (shaperTree.findPlayer(*shaperp)==nullptr) {
                    shaperp->current->decreaseShaperCount();
                    delete shaperp;
                }
            }
        }
        displayGameState();

    }
    while (getline(file2,line2)) {
        stringstream ss(line2);
        string name;
        int res;
        ss>>name>>res;
        RealmShaper *shaperp=shaperTree.findPlayer(name);
        if (shaperp!=nullptr) {
            shaperTree.duel(shaperp,res);

        }
        displayGameState();
    }

    // TODO:
    // Read logs
    // For every 5 access, 1 duel happens
    // If there are still duel logs left after every access happens duels happens one after other

    // This function should call exploreArea and craft functions

    // Use displayGameState();
}

void GameWorld::saveGameState(const std::string &currentIsles, const std::string &currentWorld, const std::string &currentShapers, const std::string &currentPlayerTree)
{
    mapTree.writeIslesToFile(currentIsles);
    mapTree.writeToFile(currentWorld);
    shaperTree.writeToFile(currentPlayerTree);
    shaperTree.writeShapersToFile(currentShapers);
}