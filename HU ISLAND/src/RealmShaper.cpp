#include "RealmShaper.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;
#define NECESSARY_ENERGY 2000 // Necessary energy to craft an Isle

RealmShaper::RealmShaper(std::string name, int honour) : name(name)
{
    this->honour = honour;
}

const std::string &RealmShaper::getName() const
{
    return this->name;
}

int RealmShaper::getEnergyLevel()
{
    return this->collectedEnergyPoints;
}

int RealmShaper::getHonour()
{
    return this->honour;
}

void RealmShaper::gainHonour()
{
    this->honour += 150;
}

void RealmShaper::loseHonour()
{
    this->honour -= 150;
}

void RealmShaper::collectItem(Item item)
{
    this->collectedEnergyPoints += item;
}

void RealmShaper::loseEnergy()
{
    this->collectedEnergyPoints -= NECESSARY_ENERGY;
}

bool RealmShaper::hasEnoughEnergy()
{
    return this->collectedEnergyPoints >= NECESSARY_ENERGY;
}

std::vector<RealmShaper *> RealmShaper::readFromFile(const std::string &filename)
{
    std::vector<RealmShaper *> players;

    std::ifstream file(filename);
    std::string line;
    while (getline(file,line)) {
        stringstream ss(line);
        string name;
        int honor;
        ss>>name>>honor;
        RealmShaper *player=new RealmShaper(name,honor);
        players.push_back(player);
    }
    // TODO: Read RealmShapers from the file,
    // add them to vector
    // return the vector
    // Input format: playerName[tab]honourPoints

    return players;
}

bool RealmShaper::operator==(const RealmShaper &other) const
{
    if(this->name==other.name) {
        return true;
    }
    // TODO: Compare by name, return true if same
    return false;
}

std::ostream &operator<<(std::ostream &os, const RealmShaper &p)
{
    // Outs the player name
    return (os << p.name); // << std::endl);
}
