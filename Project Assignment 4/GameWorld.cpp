#include "GameWorld.h"


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
    bool hasAccess = false;
    // TODO: Check if the realmShaper has access to explore the isle
    // Get necessary depth values
    // Use mapTree.calculateMinMapDepthAccess
    // Use // std::cout << "[Access Control] " << "RealmShaper not found!" << std::endl;
    if (realmShaper == nullptr || isle == nullptr)
    {
        std::cout << "[Access Control] " << "RealmShaper not found!" << std::endl;
        return false;
    }

    // Get the depth of the RealmShaper in the ShaperTree
    int playerDepth = shaperTree.getDepth(realmShaper);
    if (playerDepth == -1) {
        std::cout << "[Access Control] " << "RealmShaper not found!" << std::endl;
        return false;
    }

    // Get the depth of the target Isle in the MapTree
    int isleDepth = mapTree.getIsleDepth(isle);
    if (isleDepth == -1)
    {
        std::cout << "[Access Control] Isle not found in MapTree!" << std::endl;
        return false;
    }

    // Total depth of the ShaperTree and MapTree
    int totalShaperTreeHeight = shaperTree.getDepth();
    int totalMapDepth = mapTree.getDepth();
    if (totalMapDepth == -1 || totalShaperTreeHeight == -1) {
        std::cout << "[Access Control] Isle not found in MapTree!" << std::endl;
        return false;
    }

    // Calculate minimum depth the player can access
    int minAccessibleDepth = mapTree.calculateMinMapDepthAccess(playerDepth,totalShaperTreeHeight,totalMapDepth);
    if (minAccessibleDepth < 0) {
        std::cout << "[Access Control] Isle not found in MapTree!" << std::endl;
        return false;
    }
    // Determine if the player has access
    hasAccess = (isleDepth >= minAccessibleDepth);

    return hasAccess;
}

void GameWorld::exploreArea(RealmShaper *realmShaper, Isle *isle)
{
    // TODO:
    // Check if realmShaper has access
    // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << *isle << std::endl;
    // If realmShaper has access
    // Visit isle, 
    // collect item, 
    // check overcrowding for Isle, 
    // delete Isle if necessary

    // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
    // Use // std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
    // Use // std::cout << "[Owercrowding] " << isle->getName() << " self-destructed, it will be removed from the map" << std::endl;

    // You will need to implement a mechanism to keep track of how many realm shapers are at an Isle at the same time
    // There are more than one way to do this, so it has been left completely to you
    // Use shaperCount, but that alone will not be enough,
    // you will likely need to add attributes that are not currently defined
    // to RealmShaper or Isle or other classes depending on your implementation
    // Null checks for realmShaper and isle
    if (!realmShaper || !isle)
    {
        std::cerr << "[Error] Null RealmShaper or Isle provided to exploreArea!" << std::endl;
        return;
    }
    bool playerHasAccess = hasAccess(realmShaper, isle);
    if (playerHasAccess) {

        if (realmShaper->getIsle() != nullptr) {
            realmShaper->getIsle()->decreaseShaperCount();
        }

        realmShaper->collectItem(isle->getItem());

        realmShaper->setIsle(isle);
        bool isFull = realmShaper->getIsle()->increaseShaperCount();

        if (isFull) {
            for (RealmShaper *shaper : shaperTree.getRealmShapers()) {
                if (shaper != nullptr && shaper->getIsle() != nullptr) {
                    if (shaper->getIsle()->getName() == isle->getName()) {
                        shaper->setIsle(nullptr);
                    }
                }
            }
            mapTree.remove(isle);
            std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
            std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
            std::cout << "[Owercrowding] " << isle->getName() << "self-destructed, it will be removed from the map" << std::endl;
        } else {
            std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
            std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
        }


    } else {
        std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << *isle << "." << std::endl;
    }
}

void GameWorld::craft(RealmShaper *shaper, const std::string &isleName){
    // TODO: Check energy and craft new isle if possible
    // Use std::cout << "[Energy] " << shaperName << " has enough energy points: " << shaperEnergyLevel << std::endl;
    // Use std::cout << "[Craft] " << shaperName << " crafted new Isle " << isleName << std::endl;
    // Use std::cout << "[Energy] " << shaperName << " does not have enough energy points: " << shaperEnergyLevel << std::endl;

    if (!shaper) {
        return;
    }
    // Check if the player has enough energy to craft
    if (shaper->hasEnoughEnergy()) { // Define the required energy constant
        std::cout << "[Energy] " << shaper->getName() << " has enough energy points: " << shaper->getEnergyLevel() << std::endl;
        shaper->loseEnergy();
        Isle *newIsle = new Isle(isleName);

        mapTree.insert(newIsle);

        std::cout << "[Craft] " << shaper->getName() << " crafted new Isle " << isleName << std::endl;
    } else {
        std::cout << "[Energy] " << shaper->getName() << " does not have enough energy points: " << shaper->getEnergyLevel() << std::endl;
    }

}

void GameWorld::displayGameState()
{
    //mapTree.displayMap();
    //shaperTree.displayTree();
}

// TODO: Implement functions to read and parse Access and Duel logs

void GameWorld::processGameEvents(const std::string &accessLogs, const std::string &duelLogs)
{
    // TODO:
    // Read logs
    // For every 5 access, 1 duel happens
    // If there are still duel logs left after every access happens duels happens one after other

    // This function should call exploreArea and craft functions

    // Use displayGameState();
    std::ifstream accessFile(accessLogs);
    std::ifstream duelFile(duelLogs);

    if (!accessFile.is_open() || !duelFile.is_open()) {
        std::cerr << "Error: Unable to open logs!" << std::endl;
    } else {

        std::string accessEvent, duelEvent;
        int accessCount = 0;
        std::getline(accessFile, accessEvent);
        std::getline(duelFile, duelEvent);
        while (std::getline(accessFile, accessEvent))
        {
            // Process access events
            // Parse RealmShaper and Isle from accessEvent (implementation depends on log format)

            std::istringstream iss(accessEvent);
            std::string playerName, placeName;

            // Extract the player and place names
            iss >> playerName >> placeName;

            RealmShaper *shaper = shaperTree.findPlayer(playerName);
            Isle *isle = mapTree.findIsle(placeName);

            if (shaper && isle)
            {
                exploreArea(shaper, isle);
                accessCount++;
            }
            else if (shaper)
            {
                // If the place doesn't exist, create a new one if the player has access

                craft(shaper, placeName);
                accessCount++;
            }

            // Trigger a duel every 5 accesses
            if (accessCount % 5 == 0 && std::getline(duelFile, duelEvent))
            {
                // Process duel events (implementation depends on duel format)
                std::istringstream iss(duelEvent);

                std::string playerName;
                int resultInt;
                bool resultBool;

                // Extract the player name and result
                iss >> playerName >> resultInt;
                resultBool = (resultInt != 0);
                RealmShaper *foundPlayer = shaperTree.findPlayer(playerName);
                if (foundPlayer) {
                    shaperTree.duel(foundPlayer, resultBool);
                }
            }
        }
        while (std::getline(duelFile, duelEvent)) {
            // Process duel events (implementation depends on duel format)
            std::istringstream iss(duelEvent);

            std::string playerName;
            int resultInt;
            bool resultBool;

            // Extract the player name and result
            iss >> playerName >> resultInt;
            resultBool = (resultInt != 0);
            RealmShaper *foundPlayer = shaperTree.findPlayer(playerName);
            if (foundPlayer != nullptr) {
                shaperTree.duel(foundPlayer, resultBool);
            }
            displayGameState();
        }
    }
    displayGameState();
}

void GameWorld::saveGameState(const std::string &currentIsles, const std::string &currentWorld, const std::string &currentShapers, const std::string &currentPlayerTree)
{
    mapTree.writeIslesToFile(currentIsles);
    mapTree.writeToFile(currentWorld);
    shaperTree.writeToFile(currentPlayerTree);
    shaperTree.writeShapersToFile(currentShapers);
}