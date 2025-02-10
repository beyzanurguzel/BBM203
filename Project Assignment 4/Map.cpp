#include "Map.h"

#include <algorithm>

#include <queue>


#include <stack>

Map::Map()
{
    this->root = nullptr;
}
Map::~Map()
{
    // TODO: Free any dynamically allocated memory if necessary

    /// If the tree is empty, nothing to delete
    if (!root) {
        return;
    }
    // Stack for iterative post-order traversal
    std::stack<MapNode *> stack;
    MapNode *lastVisited = nullptr;
    MapNode *current = root;

    while (!stack.empty() || current != nullptr)
    {
        if (current != nullptr)
        {
            stack.push(current);
            current = current->left; // Traverse to the left child
        }
        else
        {
            MapNode *peekNode = stack.top();
            if (peekNode->right != nullptr && lastVisited != peekNode->right)
            {
                // If the right child exists and hasn't been visited, traverse right
                current = peekNode->right;
            }
            else
            {
                // Visit the node
                stack.pop();
                delete peekNode->isle;
                delete peekNode;
                lastVisited = peekNode;
            }
        }
    }

    root = nullptr; // Clear the root after deleting all nodes
}

void Map::initializeMap(std::vector<Isle *> isles)
{
    // TODO: Insert initial isles to the tree
    // Then populate with Goldium and Einsteinium items

    if (isles.empty()) {
        std::cerr << "Warning: No Isles provided to initialize the map." << std::endl;
        return;
    }

    // Insert each Isle into the AVL tree
    for (Isle *isle : isles) {
        if (isle) { // Ensure the Isle is not null
            insert(isle); // Calls the insert method to add Isle into the tree
            rotateCount = 0;
        } else {
            std::cerr << "Warning: Encountered a null Isle during initialization." << std::endl;
        }
    }

    // Populate the tree with items (Goldium and Einstainium)
    populateWithItems();
    rotateCount = 0;
}


MapNode *Map::rotateRight(MapNode *current)
{
    // TODO: Perform right rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;

    // Check for invalid input
    if (current == nullptr || current->left == nullptr) {
        std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;
        return nullptr;
    }

    // Perform rotation
    MapNode *newRoot = current->left;
    current->left = newRoot->right;
    newRoot->right = current;

    // Update heights
    current->height = std::max(height(current->left), height(current->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;

    return newRoot;
}

MapNode *Map::rotateLeft(MapNode *current)
{
    // TODO: Perform left rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;

    // Check for invalid input
    if (current == nullptr || current->right == nullptr) {
        std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;
        return nullptr;
    }

    // Perform rotation
    MapNode *newRoot = current->right;
    current->right = newRoot->left;
    newRoot->left = current;

    // Update heights
    current->height = std::max(height(current->left), height(current->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;

    return newRoot;
}

int Map::calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth)
{
    return (int)totalMapDepth * ((double)playerDepth / totalShaperTreeHeight);
}

int Map::height(MapNode *node)
{
    // TODO: Return height of the node
    if (node != nullptr && findNode(node->isle->getName()) != nullptr) {
        return node->height;
    }
    return 0;
}

MapNode *Map::insert(MapNode *node, Isle *isle)
{
    MapNode *newNode = nullptr;

    // TODO: Recursively insert isle to the tree
    // returns inserted node

    if (node == nullptr) {
        newNode = new MapNode(isle); // Create and return a new MapNode
        return newNode;
    }

    // Recursive insertion
    if (isle->getName() < node->isle->getName()) {
        node->left = insert(node->left, isle);
    } else if (isle->getName() > node->isle->getName()) {
        node->right = insert(node->right, isle);
    } else {
        // Duplicate names, return the existing node
        return node;
    }

    // Update height of the current node
    //if (node->left != nullptr && node->right != nullptr) {
        node->height = std::max(height(node->left), height(node->right)) + 1;
    //}

    // Calculate the balance factor
    int balance = height(node->left) - height(node->right);

    // Perform rotations to balance the tree
    // Left Left Case
    if (balance > 1 && isle->getName() < node->left->isle->getName()) {
        rotateCount++;
        return rotateRight(node);
    }

    // Right Right Case
    if (balance < -1 && isle->getName() > node->right->isle->getName()) {
        rotateCount++;
        return rotateLeft(node);
    }

    // Left Right Case
    if (balance > 1 && isle->getName() > node->left->isle->getName()) {
        rotateCount++;
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Left Case
    if (balance < -1 && isle->getName() < node->right->isle->getName()) {
        rotateCount++;
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    // No rotation needed; return the node itself
    return node;
}

void Map::insert(Isle *isle)
{
    root = insert((root), isle);

    // you might need to insert some checks / functions here depending on your implementation
    if (rotateCount != 0 && rotateCount % 3 == 0) {
        populateWithItems();
        dropItemBFS();
    }
}

MapNode *Map::remove(MapNode *node, Isle *isle)
{
    // TODO: Recursively delete isle from the tree
    // Will be called if there is overcrowding
    // returns node
    // Use std::cout << "[Remove] " << "Tree is Empty" << std::endl;
    if (node == nullptr) {
        std::cout << "[Remove] " << "Tree is Empty" << std::endl;
        return nullptr;
    }

    // Recursive deletion
    if (isle->getName() < node->isle->getName()) {
        node->left = remove(node->left, isle);
    } else if (isle->getName() > node->isle->getName()) {
        node->right = remove(node->right, isle);
    } else {
        // Node to be deleted found
        if (node->left == nullptr && node->right == nullptr ) {
            delete node;
            return nullptr; // No children
        }
        if (node->left != nullptr && node->right != nullptr) {
            // Two children: Replace with in-order predecessor (largest in left subtree)
            MapNode *predecessor = node->left;
            while (predecessor->right != nullptr) {
                predecessor = predecessor->right;
            }
            node->isle = predecessor->isle; // Replace isle
            node->left = remove(node->left, predecessor->isle); // Remove predecessor
        } else {
            // One child
            MapNode *child = (node->left != nullptr) ? node->left : node->right;
            delete node;
            return child;
        }
    }

    // Update height
    node->height = std::max(height(node->left), height(node->right)) + 1;

    // Rebalance tree
    int balance = height(node->left) - height(node->right);

    // Left Left Case
    if (balance > 1 && height(node->left->left) >= height(node->left->right)) {
        rotateCount++;
        return rotateRight(node);
    }

    // Left Right Case
    if (balance > 1 && height(node->left->left) < height(node->left->right)) {
        rotateCount++;
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Right Case
    if (balance < -1 && height(node->right->right) >= height(node->right->left)) {
        rotateCount++;
        return rotateLeft(node);
    }

    // Right Left Case
    if (balance < -1 && height(node->right->right) < height(node->right->left)) {
        rotateCount++;
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

void Map::remove(Isle *isle)
{
    root = remove((root), isle);
    // you might need to insert some checks / functions here depending on your implementation
    if (rotateCount != 0 && rotateCount % 3 == 0) {
        populateWithItems();
        dropItemBFS();
    }
}

void Map::preOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop EINSTEINIUM according to rules
    // Use std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
    if (current == nullptr) {
        return; // Base case: Null node
    }

    // Increment the counter
    count++;

    // Drop EINSTEINIUM on every 5th Isle
    if (count % 5 == 0) {
        if (current->isle != nullptr) {
            current->isle->setItem(EINSTEINIUM); // Set the item
            std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
        } else {
            std::cerr << "[Warning] Isle is null at count " << count << std::endl;
        }
    }

    // Recursive pre-order traversal
    preOrderItemDrop(current->left, count);
    preOrderItemDrop(current->right, count);
}

// to Display the values by Post Order Method .. left - right - node
void Map::postOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop GOLDIUM according to rules
    // Use  std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
    if (current == nullptr) {
        return;
    }
    postOrderItemDrop(current->left, count);
    postOrderItemDrop(current->right, count);
    count++;
    if (count % 3 == 0) {
        if (current->isle != nullptr) {
            current->isle->setItem(GOLDIUM); // Set the item
            std::cout << "[Item Drop] GOLDIUM dropped on Isle: "
                      << current->isle->getName() << std::endl;
        } else {
            std::cerr << "[Warning] Isle is null at count " << count << std::endl;
        }
    }
}

MapNode *Map::findFirstEmptyIsle(MapNode *node)
{
    // TODO: Find first Isle with no item
    if (!node) {
        return nullptr; // Tree is empty
    }
    // Queue for level-order traversal
    std::queue<MapNode *> q;
    q.push(node);

    while (!q.empty())
    {
        MapNode *current = q.front();
        q.pop();

        // Check if the current Isle has no item
        if (current->isle && current->isle->getItem() == EMPTY) // Assume hasItem() checks if an item exists
        {
            return current; // Return the first node with no item
        }

        // Enqueue left and right children if they exist
        if (current->left)
            q.push(current->left);
        if (current->right)
            q.push(current->right);
    }

    // If no empty Isle is found, return nullptr
    return nullptr;
}

void Map::dropItemBFS()
{
    // TODO: Drop AMAZONITE according to rules
    // Use std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << targetNode->isle->getName() << std::endl;
    // Use std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;
    MapNode *targetNode = findFirstEmptyIsle(root);
    if (targetNode == nullptr) {
        std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;
    } else {
        targetNode->isle->setItem(AMAZONITE);
        std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << targetNode->isle->getName() << std::endl;
    }
}

void Map::displayMap()
{
    std::cout << "[World Map]" << std::endl;
    display(root, 0, 0);
}

int Map::getDepth(MapNode *node)
{
    // TODO: Return node depth if found, else return -1
    if (node == nullptr || root == nullptr) {
        return -1;
    }
    MapNode *current = root;
    int depth = 0;

    while (current != nullptr)
    {
        if (current == node)
        {
            return depth;
        }

        if (node->isle->getName() < current->isle->getName())
        {
            current = current->left;
        }
        else if (node->isle->getName() > current->isle->getName())
        {
            current = current->right;
        }
        else
        {
            return -1;
        }

        depth++;
    }
    // Node not found
    return -1;
}

// Function to calculate the depth of a specific node in the AVL tree
int Map::getIsleDepth(Isle *isle)
{
    // TODO: Return node depth by isle if found, else

    if (isle == nullptr || root == nullptr) {
        return -1; // Invalid input or empty tree
    }

    MapNode *current = root;
    int depth = 0;

    // Traverse the tree to find the Isle
    while (current != nullptr)
    {
        if (current->isle->getName() == isle->getName())
        {
            // Isle found, return the depth
            return depth;
        }

        // Move to the left or right subtree
        if (isle->getName() < current->isle->getName())
        {
            current = current->left;
        }
        else //if (isle->getName() > current->isle->getName())
        {
            current = current->right;
        }

        // Increment depth for each level traversed
        depth++;
    }

    // Isle not found
    return -1;
}

int Map::getDepth()
{
    // TODO: Return max|total depth of tree

    if (root == nullptr) {
        return -1;
    }

    std::queue<MapNode*> q;
    q.push(root);
    int depth = -1;

    while (!q.empty()) {
        int levelSize = q.size();
        for (int i = 0; i < levelSize; i++) {
            MapNode* current = q.front();
            q.pop();

            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
        depth++;
    }

    return depth;
}

void Map::populateWithItems()
{
    // TODO: Distribute first GOLDIUM than EINSTEINIUM
    if (root == nullptr) {
        std::cerr << "[Populate Items] The tree is empty. No items to distribute." << std::endl;
        return;
    }
    int goldiumCount = 0;
    int einsteiniumCount = 0;
    // Drop GOLDIUM using post-order traversal
    postOrderItemDrop(root, goldiumCount);

    // Drop EINSTEINIUM using pre-order traversal
    preOrderItemDrop(root, einsteiniumCount);
}

Isle *Map::findIsle(Isle isle)
{
    // TODO: Find isle by value
    MapNode *current = root;

    while (current)
    {
        // Compare the target Isle with the current Isle
        if (current->isle->getName() == isle.getName()) // Assuming Isle has overloaded operator==
        {
            return current->isle;
        }

        // Traverse left or right based on comparison
        if (isle.getName() < current->isle->getName())
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    // If not found, return nullptr
    return nullptr;
}

Isle *Map::findIsle(std::string name)
{
    // TODO: Find isle by name
    MapNode *current = root;

    while (current)
    {
        // Compare the target name with the current Isle's name
        if (current->isle && current->isle->getName() == name)
        {
            return current->isle;
        }

        // Traverse left or right based on lexicographical comparison
        if (name < current->isle->getName())
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    // If not found, return nullptr
    return nullptr;
}

MapNode *Map::findNode(Isle isle)
{
    // TODO: Find node by value
    MapNode *current = root;

    while (current)
    {
        // Compare the target Isle with the current node's Isle
        if (current->isle->getName() == isle.getName()) // Assuming Isle has overloaded operator==
        {
            return current;
        }

        // Traverse left or right based on comparison
        if (isle.getName() < current->isle->getName())
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    // If not found, return nullptr
    return nullptr;
}

MapNode *Map::findNode(std::string name)
{
    // TODO: Find node by name
    MapNode *current = root;

    while (current)
    {
        // Compare the target name with the current node's Isle name
        if (current->isle && current->isle->getName() == name)
        {
            return current;
        }

        // Traverse left or right based on lexicographical comparison
        if (name < current->isle->getName())
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    // If not found, return nullptr
    return nullptr;
}

void Map::display(MapNode *current, int depth, int state)
{
    // SOURCE:

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

void Map::writeToFile(const std::string &filename)
{
    // TODO: Write the tree to filename output level by level
    // Open the file for writing

    // Open the file for writing
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << " for writing." << std::endl;
        return;
    }

    // If the tree is empty, write "The tree is empty" and return
    if (!root) {
        outFile << "The tree is empty." << std::endl;
        outFile.close();
        return;
    }

    // Perform a level-order traversal using a queue
    std::queue<MapNode*> q;
    q.push(root);

    int currentLevelNodes = 1; // Nodes at the current level
    int nextLevelNodes = 0;    // Nodes at the next level
    bool hasNonNullNodes = true; // To detect if there are any non-NULL nodes left

    while (hasNonNullNodes) {
        hasNonNullNodes = false;

        while (currentLevelNodes > 0) {
            MapNode* current = q.front();
            q.pop();
            currentLevelNodes--;

            if (current) {
                // Write the current node's Isle name
                outFile << current->isle->getName() << " ";
                // Enqueue left and right children
                q.push(current->left);
                q.push(current->right);

                if (current->left || current->right) {
                    hasNonNullNodes = true; // Mark that there are still non-NULL nodes
                }
            } else {
                // Write "NULL" for missing nodes
                outFile << "NULL ";
                q.push(nullptr); // Add placeholders for the next level
                q.push(nullptr);
            }
        }

        // End the current level
        outFile << std::endl;

        // Update for the next level
        currentLevelNodes = nextLevelNodes = q.size();
    }

    // Close the file
    outFile.close();

}

void Map::writeIslesToFile(const std::string &filename)
{
    // TODO: Write Isles to output file in alphabetical order
    // Use std::cout << "[Output] " << "Isles have been written to " << filename << " in in alphabetical order." << std::endl;
    std::ofstream outFile(filename);
    if (!outFile.is_open())
    {
        std::cerr << "Error: Unable to open file " << filename << " for writing." << std::endl;
        return;
    }
    // Handle empty tree case
    if (root == nullptr)
    {
        outFile.close();
        return;
    }

    MapNode *current = root;
    std::stack<MapNode *> stack;

    while (current != nullptr || !stack.empty())
    {
        while (current != nullptr)
        {
            stack.push(current);
            current = current->left;
        }

        current = stack.top();
        stack.pop();

        if (current->isle)
        {
            outFile << current->isle->getName() << std::endl;
        }

        current = current->right;
    }

    // Close the file
    outFile.close();

    std::cout << "[Output] " << "Isles have been written to " << filename << " in in alphabetical order." << std::endl;
}