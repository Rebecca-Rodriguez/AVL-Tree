#include <string>
#include <vector>
#include <iomanip>
#include <iostream>
using namespace std;

struct Node {
    string studentName;
    int studentID;
    int balFactor;
    Node* left;
    Node* right;

    // Default Constructor
    Node() : studentName(""), studentID(0), balFactor(0), left(nullptr), right(nullptr) {}

    // Overloaded Constructors
    Node(string Name, int ID) : studentName(Name), studentID(ID), left(nullptr), right(nullptr){}
    Node(string Name, int ID, Node* left, Node* right) : studentName(Name), studentID(ID), left(left), right(right){}
};

class AVLTree
{
    Node* head;
    int count;
public:
    // Constructor
    AVLTree() : head(nullptr), count(0) {}
    
    // Mutators
    void setHead(Node* head){this->head = head;};

    // Accessors
    Node* getHead(){return head;};
    int getNodeCout(){return count;};
    
    Node* rotateLeft(Node* node)
    {
        Node* rightChild = node->right;
        node->right = rightChild->left;
        rightChild->left = node;
        return rightChild;
    }

    Node* rotateRight(Node* node)
    {
        Node* leftChild = node->left;
        node->left = leftChild->right;
        leftChild->right = node;
        return leftChild;
    }

    Node* rotateLeftRight(Node* node)
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    
    Node* rotateRightLeft(Node* node)
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    
    // calculate the height by recursively going from going top down
    int getHeight(Node* head)
    {
        if (head == nullptr)
            return 0;
        int leftH = getHeight(head->left);
        int rightH = getHeight(head->right);
        return 1 + max(leftH, rightH);
    }
    
    // to calculate the balance factor, subtract the left subtree from the right subtree
    int balanceFactor(Node* head)
    {
        if (head == nullptr)
            return 0;
        else
            return getHeight(head->left) - getHeight(head->right);
    }
    
    // Add a Student object into the tree with the specified name, NAME and GatorID number, ID
    Node* insert(Node* head, string studentName, int studentID)
    {
        // traverse through the tree starting with the root and insert the node in the correct spot
        if (head == nullptr)
        {
            Node* node = new Node(studentName, studentID);
            cout << "successful" << endl;
            count++;
            return node;
        }
        
        if (head->studentID > studentID)
            head->left = insert(head->left, studentName, studentID);
        else if (head->studentID < studentID)
            head->right = insert(head->right, studentName, studentID);
        else
        {
            cout << "unsuccessful" << endl;
            return head;
        }

        // get the balance factor of this ancestor node to check whether this node bacame unbalanced
        head->balFactor = balanceFactor(head);

        // perform rotations on the tree as necessary (recursive)
        if (head->balFactor > 1 && studentID < head->left->studentID)
            return rotateRight(head);
        else if (head->balFactor < -1 && studentID > head->right->studentID)
            return rotateLeft(head);
        else if (head->balFactor > 1 && studentID > head->left->studentID)
            return rotateLeftRight(head);
        else if (head->balFactor < -1 && studentID <  head->right->studentID)
            return rotateRightLeft(head);
        
        return head;
    }

    // helper function for remove; finds the inorder successor
    Node* smallestOnLeft(Node* node)
    {
        Node* temp = node;
        while (temp->left != nullptr)
            temp = temp->left;
        return temp;
    }
    
    // Find and remove the account with the specified ID from the tree
    Node* remove(Node* head, int studentID)
    {
        if (head == nullptr)
            return nullptr;
        
        if (studentID < head->studentID) head->left = remove(head->left, studentID);
        else if (studentID > head->studentID) head->right = remove(head->right, studentID);
        else
        {
            count--;
            // if no children: simply delete the node and set parent pointer to be null on that side
            if (head->left == nullptr && head->right == nullptr)
            {
                delete head;
                head = nullptr;
                return head;
            }
            // if one child: replace the current node that we are trying to remove with the data of that one child
            else if (head->left == nullptr)
            {
                Node* temp = head;
                head = head->right;
                delete temp;
                return head;
            }
            else if (head->right == nullptr)
            {
                Node* temp = head;
                head = head->left;
                delete temp;
                return head;
            }

            // if two children: inorder successor
            else
            {
                Node* temp = smallestOnLeft(head->right);
                head->studentID = temp->studentID;
                head->studentName = temp->studentName;
                head->right = remove(head->right, head->studentID);
                return head;
            }
        }
        return head;
    }

    // Search for the student with the specified ID from the tree
    bool searchID(Node* head, int studentID)
    {
        // If the ID does not exist within the tree, print “unsuccessful”
        if (head == nullptr)
        {
            cout << "unsuccessful" << endl;
            return false;
        }

        // If the ID was found, print out their NAME
        if (head->studentID == studentID)
        {
            cout << head->studentName << endl;
            return true;
        }
        if (studentID < head->studentID)
            searchID(head->left, studentID);
        if (studentID > head->studentID)
            searchID(head->right, studentID);
    }

    // Search for the student with the specified NAME in the tree
    bool searchName(Node* head, string studentName)
    {
        vector<Node*> namesMatch;       // might be multiple people with same name
        vector<Node*> res;
        sortPreorder(head, res);

        for (int i = 0; i < res.size(); i++)
        {
            // If the tree has more than one object with the same NAME, print each ID on a new line with no other spaces and in the same relative order as a pre-order traversal
            if (res[i]->studentName == studentName)
            {
                namesMatch.push_back(res[i]);
                // check for leading zeros
                if (res[i]->studentID < 10000000)
                    cout << setw(8) << setfill('0') << res[i]->studentID << endl;
                else 
                    cout << res[i]->studentID << endl;
            }
        }

        // If the name does not exist within the tree, print “unsuccessful”
        if (namesMatch.empty())
        {
            cout << "unsuccessful" << endl;
            return false;
        }
        else
            return true;
    }

    // helper function for printInorder
    void sortInorder(Node* head, vector<Node*>& res)
    {
        // inorder: left->root->right
        if (head == nullptr)
            return;
        sortInorder(head->left, res);
        res.push_back(head);
        sortInorder(head->right, res);
    }

    // Print out a comma separated inorder traversal of the names in the tree
    void printInorder(Node* head)
    {
        vector<Node*> res;
        sortInorder(head, res);
        for (int i = 0; i < res.size(); i++)
        {
            if (i != res.size() - 1)
                cout << res[i]->studentName << ", ";
            else
                cout << res[i]->studentName;
        }
    }

    // helper function for printPreorder
    void sortPreorder(Node* head, vector<Node*>& res)
    {
        // preorder: root->left->right
        if (head == nullptr)
            return;
        res.push_back(head);
        sortPreorder(head->left, res);
        sortPreorder(head->right, res);
    }

    // Print out a comma separated preorder traversal of the names in the tree
    void printPreorder(Node* head)
    {
        vector<Node*> res;
        sortPreorder(head, res);
        for (int i = 0; i < res.size(); i++)
        {
            if (i != res.size() - 1)
                cout << res[i]->studentName << ", ";
            else
                cout << res[i]->studentName;
        }
    }
    
    // helper function for printPostorder
    void sortPostorder(Node* head, vector<Node*>& res)
    {
        // postorder: left->right->root
        if (head == nullptr)
            return;
        sortPostorder(head->left, res);
        sortPostorder(head->right, res);
        res.push_back(head);

    }
    
    // Print out a comma separated postorder traversal of the names in the tree
    void printPostorder(Node* head)
    {
        vector<Node*> res;
        sortPostorder(head, res);
        for (int i = 0; i < res.size(); i++)
        {
            if (i != res.size() - 1)
                cout << res[i]->studentName << ", ";
            else
                cout << res[i]->studentName;
        }
    }

    // Prints the number of levels that exist in the tree
    void printLevelCount(Node* head)
    {
        // Prints 0 if the head of the tree is null
        if (head == nullptr)
            cout << "0" << endl;
        else
        {
            int levelCount = getHeight(head);
            cout << levelCount << endl;
        }
    }
    
    // Remove the Nth GatorID from the inorder traversal of the tree
    Node* removeInorder(Node* head, int N)
    {        
        if (head == nullptr)
        {
            cout <<"unsuccessful" << endl;
            return head;
        }

        vector<Node*> res;
        sortInorder(this->head, res);

        // unsuccessful if N >= # nodes in tree
        if (N >= res.size())
        {
            cout << "unsuccessful" << endl;
            return head;
        }
        else
        {
            Node* temp = remove(this->head, res[N]->studentID);
            cout <<"successful" << endl;
            return temp;
        }
    }

    // helper function; checks if ID exists
    bool checkIDStatus(Node* head, int studentID)
    {
        if (head == nullptr)
            return false;

        if (head->studentID == studentID)
            return true;

        bool left = checkIDStatus(head->left, studentID);
            
        if (left)
            return true;

        bool right = checkIDStatus(head->right, studentID);
            return right;
    }
};

bool validIDInput(string ID)
{
    // UFIDs are strictly 8 digits long
    if (ID.length() != 8)
        return false;
    bool valid = true;
    for (int i = 0; i < ID.length(); i++)
    {
        if (isdigit(ID[i]))
            valid = true;
        else
        {
            valid = false;
            break;
        }
    }
    return valid;
}

bool validNameInput(string name)
{
    // Name must include only alphabets from [a-z, A-Z, and spaces]
    bool valid = true;
    for (int i = 0; i < name.length(); i++)
    {
        if (isalpha(name[i]) || isspace(name[i]))
            valid = true;
        else
        {
            valid = false;
            break;
        }
    }
    return valid;
}

bool validN(string n)
{
    // checks if value followed by command removeInorder is valid
    bool valid = true;
    for (int i = 0; i < n.length(); i++)
    {
        if (isdigit(n[i]))
            valid = true;
        else
        {
            valid = false;
            break;
        }
    }
    return valid;
}

void functionCall(string userCommand, AVLTree& gatorAVL)
{
    string studentName;
    int studentID;
    int ind = userCommand.find_first_of(' ');
    string functionToCall = userCommand.substr(0, ind);         // store which function needs to be called
    userCommand = userCommand.substr(ind + 1);                  // update the command that has name or id
    string checkID;

    if (functionToCall == "insert") {
        userCommand = userCommand.substr(1);                    // start at 1 b/c 0 is "
        ind = userCommand.find_first_of('\"');                  // find end of name
        studentName = userCommand.substr(0, ind);
        checkID = userCommand.substr(ind + 2);                  // add 2 b/c ind is where " located

        if (validIDInput(checkID) && validNameInput(studentName))
        {
            studentID = stoi(checkID);
            Node* newHead = gatorAVL.insert(gatorAVL.getHead(), studentName, studentID);
            gatorAVL.setHead(newHead);
        }
        else
            cout << "unsuccessful" << endl;
    }

    else if (functionToCall == "remove")
    {
        if (validIDInput(userCommand))
        {
            studentID = stoi(userCommand);
            if(gatorAVL.checkIDStatus(gatorAVL.getHead(), studentID))
            {
                cout << "successful" << endl;
                gatorAVL.setHead(gatorAVL.remove(gatorAVL.getHead(), studentID));
            }
            else
                cout << "unsuccessful" << endl;
        }
        else
            cout << "unsuccessful" << endl;
    }

    else if (functionToCall == "search")
    {
        // perform search for ID
        if (isdigit(userCommand[0]))
        {
            if (validIDInput(userCommand))
            {
                studentID = stoi(userCommand);
                gatorAVL.searchID(gatorAVL.getHead(), studentID);
            }
            else
                cout << "unsuccessful" << endl;
        }
        // perform search for Name
        else
        {
            studentName = userCommand.substr(1, userCommand.length() - 2);
            if(validNameInput(studentName))
                gatorAVL.searchName(gatorAVL.getHead(), studentName);
            else
                cout << "unsuccessful" << endl;
        }
    }

    else if (functionToCall == "printInorder")
    {
        gatorAVL.printInorder(gatorAVL.getHead());
        cout << endl;
    }

    else if (functionToCall == "printPreorder")
    {
        gatorAVL.printPreorder(gatorAVL.getHead());
        cout << endl;
    }

    else if (functionToCall == "printPostorder")
    {
        gatorAVL.printPostorder(gatorAVL.getHead());
        cout << endl;
    }

    else if (functionToCall == "printLevelCount")
        gatorAVL.printLevelCount(gatorAVL.getHead());

    else if (functionToCall == "removeInorder") {
        if(validN(userCommand))
        {
            int N = stoi(userCommand);
            gatorAVL.setHead(gatorAVL.removeInorder(gatorAVL.getHead(), N));
        }
        else
            cout << "unsuccessful" << endl;
    }
}

int main()
{
    AVLTree gatorAVL;
    int in;
    cin >> in;
    string line;
    vector<string> commands;

    // read lines of input from cin and store in vector
    for (int i = 0; i < in + 1; i++)
    {
        getline(cin, line);
        commands.push_back(line);
    }
    
    // read each element inside vector of commands
    for(int i = 0; i < commands.size(); i++)
    {
        string userCommand = commands[i];
        functionCall(userCommand, gatorAVL);
    }
    return 0;
}