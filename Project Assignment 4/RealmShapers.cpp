#include "RealmShapers.h"
#include <cmath>
#include <algorithm>
#include <functional>
#include <queue>
#include <fstream> // Required for file I/O



ShaperTree::ShaperTree()
{

}

ShaperTree::~ShaperTree()
{
    // TODO: Free any dynamically allocated memory if necessary
    for (auto shaper : realmShapers)
    {
        delete shaper; // Free allocated memory
    }
    realmShapers.clear(); // Clear the vector
}

void ShaperTree::initializeTree(std::vector<RealmShaper *> shapers)
{
    // TODO: Insert initial shapers to the tree
    realmShapers.clear(); // Clear any existing nodes
    for (auto shaper : shapers)
    {
        realmShapers.push_back(shaper); // Add players in level-order
    }
}

int ShaperTree::getSize()
{
    // TODO: Return number of shapers in the tree
    return realmShapers.size();
}

std::vector<RealmShaper *> ShaperTree::getTree()
{
    return realmShapers;
}

bool ShaperTree::isValidIndex(int index)
{
    bool isValid = false;

    // TODO: Check if the index is valid in the tree
    isValid = index >= 0 && index < realmShapers.size();

    return isValid;
}

void ShaperTree::insert(RealmShaper *shaper)
{
    // TODO: Insert shaper to the tree
    realmShapers.push_back(shaper); // Add at the next available position
}

int ShaperTree::remove(RealmShaper *shaper)
{
    // TODO: Remove the player from tree if it exists
    // Make sure tree protects its form (complete binary tree) after deletion of a node
    // return index if found and removed
    // else return -1

    // Find the index of the node to remove
    int index = findIndex(shaper);
    if (index == -1)
    {
        // Shaper not found in the tree
        return -1;
    }

    // Free the memory of the node to be removed
    delete realmShapers[index]; // Delete the object at the specified index
    int foundIndex = index;
    // Shift elements to fill the gap
    for (size_t i = index; i < realmShapers.size() - 1; ++i) {
        realmShapers[i] = realmShapers[i + 1];
    }

    // Remove the last element in the vector
    realmShapers.pop_back();
    return foundIndex;
}

int ShaperTree::findIndex(RealmShaper *shaper)
{
    for (size_t index = 0; index < realmShapers.size(); ++index) {
        if (realmShapers[index]->getName() == shaper->getName()) {
            return index; // Match found
        }
    }
    return -1; // Not found
}


int ShaperTree::getDepth(RealmShaper *shaper)
{
    // return depth of the node in the tree if found
    // else return -1
    int index = findIndex(shaper);
    if (index == -1) {
        return -1;
    }
    int depth = 0;
    while (index > 0)
    {
        index = (index - 1) / 2; // Move to parent
        depth++;
    }
    return depth;
}

int ShaperTree::getDepth()
{
    // return total|max depth|height of the tree
    int lastIndex = realmShapers.size() - 1;
    int depth = getDepth(realmShapers[lastIndex]);
    return depth;
}

const std::vector<RealmShaper*>& ShaperTree::getRealmShapers() const {
    return realmShapers; // Return a const reference to the vector
}


RealmShaper ShaperTree::duel(RealmShaper *challenger, bool result)
{
    // TODO: Implement duel logic, return the victor
    // Use   std::cout << "[Duel] " << victorName << " won the duel" << std::endl;
    // Use   std::cout << "[Honour] " << "New honour points: ";
    // Use   std::cout << challengerName << "-" << challengerHonour << " ";
    // Use   std::cout << opponentName << "-" << opponentHonour << std::endl;
    // Use   std::cout << "[Duel] " << loserName << " lost all honour, delete" << std::endl;


    int challengerIndex = findIndex(challenger);
    std::string victorName;
    int loserIndex;
    int victorIndex;
    int parentIndex = (challengerIndex - 1) / 2;

    if (challengerIndex == -1 || challengerIndex == 0) {
        std::cout << "Challenger not found" << std::endl;
        return *challenger;
    } else {
        if (result == 1) {
            victorIndex = challengerIndex;
            loserIndex = parentIndex;
            victorName = challenger->getName();
            std::cout << "[Duel] " << victorName << " won the duel" << std::endl;
            challenger->gainHonour();
            realmShapers[parentIndex]->loseHonour();
            std::cout << "[Honour] " << "New honour points: ";
            std::cout << challenger->getName() << "-" << challenger->getHonour() << " ";
            std::cout << realmShapers[parentIndex]->getName() << "-" << realmShapers[parentIndex]->getHonour() << std::endl;
            RealmShaper *temp = realmShapers[parentIndex];
            realmShapers[parentIndex] = realmShapers[challengerIndex];
            realmShapers[challengerIndex] = temp;
        } else {
            loserIndex = challengerIndex;
            victorIndex = parentIndex;
            std::cout << "[Duel] " << realmShapers[loserIndex]->getName() << " lost the duel" << std::endl;
            challenger->loseHonour();
            realmShapers[parentIndex]->gainHonour();
            std::cout << "[Honour] " << "New honour points: ";
            std::cout << challenger->getName() << " - " << challenger->getHonour() << " ";
            std::cout << realmShapers[parentIndex]->getName() << " - " << realmShapers[parentIndex]->getHonour() << std::endl;
        }

        if (realmShapers[loserIndex]->getHonour() <= 0) {
            std::cout << "[Duel] " << realmShapers[loserIndex]->getName() << " lost all honour, delete" << std::endl;
            remove(realmShapers[loserIndex]);
        }
        return *realmShapers[victorIndex];
    }

}

RealmShaper *ShaperTree::getParent(RealmShaper *shaper)
{
    RealmShaper *parent = nullptr;

    // TODO: return parent of the shaper
    int index = findIndex(shaper);
    if (index == -1 || index == 0) {
        std ::cout << "ShaperTree::getParent: no parent found" << std::endl;
        return parent;
    }
    parent = realmShapers[(index - 1) / 2];

    return parent;
}

void ShaperTree::replace(RealmShaper *player_low, RealmShaper *player_high)
{
    // TODO: Change player_low and player_high's positions on the tree
    int lowPlayerIndex = findIndex(player_low);
    int highPlayerIndex = findIndex(player_high);
    if (lowPlayerIndex == -1 || highPlayerIndex == -1) {
        std ::cout << "ShaperTree::replace(): at least one of the player is not in the tree" << std::endl;
    } else {
        realmShapers[lowPlayerIndex] = player_high;
        realmShapers[highPlayerIndex] = player_low;
    }
    // std::swap(realmShapers[index_low], realmShapers[index_high]); // if upper code doesn't work, then try this
}

RealmShaper *ShaperTree::findPlayer(RealmShaper shaper)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by object
    // Return the shaper if found
    // Return nullptr if shaper not found

    for (RealmShaper *current : realmShapers) {
        if (*current == shaper) { // Assuming operator== is overloaded for RealmShaper
            foundShaper = current;
            return foundShaper; // Return pointer to the matching shaper
        }
    }
    return foundShaper;
}
// Find shaper by name
RealmShaper *ShaperTree::findPlayer(std::string name)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by name
    // Return the shaper if found
    // Return nullptr if shaper not found
    // Iterate through all RealmShapers in the vector
    for (RealmShaper *shaper : realmShapers) {
        // Check if the name matches
        if (shaper->getName() == name) {
            return shaper; // Return the pointer to the matching shaper
        }
    }
    return foundShaper;
}

std::vector<std::string> ShaperTree::inOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement inOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation

    // Note: Since ShaperTree is not a binary search tree,
    // in-order traversal will not give rankings in correct order
    // for correct order you need to implement level-order traversal
    // still you are to implement this function as well

    // Base case: If the index is invalid, return an empty vector
    if (!isValidIndex(index)) {
        return result;
    }

    // Traverse the left child
    std::vector<std::string> leftResult = inOrderTraversal(2 * index + 1);
    result.insert(result.end(), leftResult.begin(), leftResult.end());

    // Process the current node
    result.push_back(realmShapers[index]->getName());

    // Traverse the right child
    std::vector<std::string> rightResult = inOrderTraversal(2 * index + 2);
    result.insert(result.end(), rightResult.begin(), rightResult.end());

    return result;
}

std::vector<std::string> ShaperTree::preOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement preOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define a helper function for recursion
    std::function<void(int)> traverse = [&](int currentIndex) {
        // Base case: If the index is invalid, stop recursion
        if (!isValidIndex(currentIndex)) {
            return;
        }

        // Process the current node
        result.push_back(realmShapers[currentIndex]->getName());

        // Traverse the left child
        traverse(2 * currentIndex + 1);

        // Traverse the right child
        traverse(2 * currentIndex + 2);
    };

    // Start the traversal from the given index
    traverse(index);

    return result;
}

std::vector<std::string> ShaperTree::postOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement postOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define a helper function for recursion
    std::function<void(int)> traverse = [&](int currentIndex) {
        // Base case: If the index is invalid, stop recursion
        if (!isValidIndex(currentIndex)) {
            return;
        }

        // Traverse the left subtree
        traverse(2 * currentIndex + 1);

        // Traverse the right subtree
        traverse(2 * currentIndex + 2);

        // Process the current node
        result.push_back(realmShapers[currentIndex]->getName());
    };

    // Start the traversal from the given index
    traverse(index);

    return result;
}

void ShaperTree::preOrderTraversal(int index, std::ofstream &outFile)
{
    // TODO: Implement preOrderTraversal in tree
    // write nodes to output file

    // Base case: If the index is invalid, stop recursion
    if (!isValidIndex(index)) {
        return;
    }

    // Write the current node to the file
    outFile << realmShapers[index]->getName() << std::endl;

    // Traverse the left child
    preOrderTraversal(2 * index + 1, outFile);

    // Traverse the right child
    preOrderTraversal(2 * index + 2, outFile);
}

void ShaperTree::breadthFirstTraversal(std::ofstream &outFile)
{
    // TODO: Implement level-order traversal
    // write nodes to output file
    // Define and implement as many helper functions as necessary

    // Check if the tree is empty
    if (realmShapers.empty()) {
        return;
    }

    // Queue for level-order traversal
    std::queue<int> traversalQueue;

    // Start with the root node (index 0)
    traversalQueue.push(0);

    while (!traversalQueue.empty()) {
        // Get the current node's index
        int currentIndex = traversalQueue.front();
        traversalQueue.pop();

        // Write the current node's name to the file
        outFile << realmShapers[currentIndex]->getName() << std::endl;

        // Add left child to the queue (if valid)
        int leftChildIndex = 2 * currentIndex + 1;
        if (isValidIndex(leftChildIndex)) {
            traversalQueue.push(leftChildIndex);
        }

        // Add right child to the queue (if valid)
        int rightChildIndex = 2 * currentIndex + 2;
        if (isValidIndex(rightChildIndex)) {
            traversalQueue.push(rightChildIndex);
        }
    }
}

void ShaperTree::displayTree()
{
    std::cout << "[Shaper Tree]" << std::endl;
    printTree(0, 0, "");
}

// Helper function to print tree with indentation
void ShaperTree::printTree(int index, int level, const std::string &prefix)
{
    if (!isValidIndex(index)) {
        return;
    }

    std::cout << prefix << (level > 0 ? "   └---- " : "") << *realmShapers[index] << std::endl;
    int left = 2 * index + 1;  // TODO: Calculate left index
    int right = 2 * index + 2; // TODO: Calculate right index

    if (isValidIndex(left) || isValidIndex(right))
    {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : "")); // ╎
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}

void ShaperTree::writeShapersToFile(const std::string &filename)
{
    // TODO: Write the shapers to filename output level by level
    // Use std::cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << std::endl;
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << " for writing." << std::endl;
        return;
    }

    if (realmShapers.empty()) {
        std::cerr << "Error: The ShaperTree is empty, nothing to write." << std::endl;
        outFile.close();
        return;
    }

    std::queue<int> traversalQueue;
    traversalQueue.push(0); // Start with the root (index 0)

    while (!traversalQueue.empty()) {
        int currentIndex = traversalQueue.front();
        traversalQueue.pop();

        // Write the current node's name to the file
        if (isValidIndex(currentIndex)) {
            outFile << realmShapers[currentIndex]->getName() << std::endl;

            // Add children to the queue
            int leftChildIndex = 2 * currentIndex + 1;
            int rightChildIndex = 2 * currentIndex + 2;

            if (isValidIndex(leftChildIndex)) {
                traversalQueue.push(leftChildIndex);
            }
            if (isValidIndex(rightChildIndex)) {
                traversalQueue.push(rightChildIndex);
            }
        }
    }

    outFile.close();
    std::cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << std::endl;
}

void ShaperTree::writeToFile(const std::string &filename)
{
    // TODO: Write the tree to filename output pre-order
    // Use std::cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;

    // Open the file for writing
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << " for writing." << std::endl;
        return;
    }

    // Get the pre-order traversal as a vector
    std::vector<std::string> result = preOrderTraversal(0);

    // Write each item to the file
    for (const std::string &name : result) {
        outFile << name << std::endl;
    }

    // Close the file
    outFile.close();
    std::cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;
}
