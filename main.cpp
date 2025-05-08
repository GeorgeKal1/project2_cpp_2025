#include <iostream>
#include <string>
#include <fstream>
#include <queue>   
#include <vector>  
#include <functional> 
#include <algorithm>
using namespace std;



class participant{
    friend class Tree;

    private:
    string fullname;
    int ID;
    int ranking;
    string origin;
    int timestamp;

    participant* parent;
    participant* left;
    participant* right;

    participant(const string& str) : parent(nullptr), left(nullptr), right(nullptr){
        parseData(str);
    }


    void parseData(const string& str);//constructor helper function
};


class Tree{
    private:
        participant* root;

        //functions that help perform the insertion of the node
        void insertAsBST(participant* node){
            participant* current = root;
            participant* parent1 = nullptr;

            // Traverse the tree to find the correct position for the new node
            while (current != nullptr) {
            parent1 = current;
            if (node->timestamp < current->timestamp) {
                // Move to the left subtree if the new node's timestamp is smaller
                current = current->left;
            } else {
                // Move to the right subtree if the new node's timestamp is greater or equal
                current = current->right;
            }
            }

            // Set the parent of the new node
            node->parent = parent1;

            // Insert the new node as a child of the found parent
            if (parent1 == nullptr) {
            // If the tree is empty, the new node becomes the root
            root = node;
            } else if (node->timestamp < parent1->timestamp) {
            // Insert as the left child
            parent1->left = node;
            } else {
            // Insert as the right child
            parent1->right = node;
            }
        }
        void heapifyUp(participant* node){//suitable for inserts
            while (node->parent != nullptr && node->ranking < node->parent->ranking) {
                
                swap(node->ranking, node->parent->ranking);
                swap(node->fullname, node->parent->fullname);
                swap(node->ID, node->parent->ID);
                swap(node->origin, node->parent->origin);
                swap(node->timestamp, node->parent->timestamp);
        
                node = node->parent;
            }
        }      

        //function that helps with deletes
        void heapifyDown(participant* node) {//suitable for deletions
            if (!node) return;
        
            while (true) {
                participant* smallest = node;
        
                if (node->left && node->left->ranking < smallest->ranking)
                    smallest = node->left;
        
                if (node->right && node->right->ranking < smallest->ranking)
                    smallest = node->right;
        
                if (smallest == node) break;
        
                // Swap όλο το περιεχόμενο
                swap(node->ranking, smallest->ranking);
                swap(node->fullname, smallest->fullname);
                swap(node->ID, smallest->ID);
                swap(node->origin, smallest->origin);
                swap(node->timestamp, smallest->timestamp);
        
                node = smallest;
            }
        }
        
        // Helper function for in-order traversal and storing players in a vector
        void inOrderTraversal(participant* node, vector<participant*>& players) {

            if (node == nullptr) return; // Base case: if the node is null, do nothing
            inOrderTraversal(node->left, players); // Traverse left subtree
            players.push_back(node); // Add current node to players vector
            inOrderTraversal(node->right, players); // Traverse right subtree
        }
        
        //helper function for the 4rth action
        void printIDsInTimeRangeHelper(participant* node, int startTime, int endTime) {
            if (!node) return;
        
            // Visit left subtree if possible
            if (node->timestamp >= startTime)
                printIDsInTimeRangeHelper(node->left, startTime, endTime);
        
            // Check current node
            if (node->timestamp >= startTime && node->timestamp <= endTime)
                cout << "ID: " << node->ID << " (Timestamp: " << node->timestamp << ")" << endl;
        
            // Visit right subtree if possible
            if (node->timestamp <= endTime)
                printIDsInTimeRangeHelper(node->right, startTime, endTime);
        }
        
        participant* findByName(participant* node, const string& fname) {//helper function for deletes          
            if (!node) return nullptr;
            if (node->fullname == fname) return node;
        
            participant* found = findByName(node->left, fname);
            if (found) return found;
        
            return findByName(node->right, fname);
        }
        
        void deleteSubtree(participant* node) {//helper functions for 5th method
            if (node == nullptr) return;
        
            //recursive delete
            deleteSubtree(node->left);
            deleteSubtree(node->right);
        
            
            deleteByPointer(node);
        }
        void deleteByPointer(participant* node) {
            if (!node) return;
        
            // Ενημέρωσε τον γονέα του node ώστε να μην το δείχνει πλέον
            if (node->parent) {
                if (node->parent->left == node) {
                    node->parent->left = nullptr;
                } else if (node->parent->right == node) {
                    node->parent->right = nullptr;
                }
            } else {
                // Αν είναι η ρίζα
                root = nullptr;
            }
        
            delete node;
        }
        

        void findBestBefore(participant* node, int targetTimestamp, participant*& best) {//helper function for 6th method
            if (!node) return;
        
            // Αν ο timestamp είναι μικρότερος από το όριο, τότε είναι υποψήφιος
            if (node->timestamp < targetTimestamp) {
                if (!best || node->ranking < best->ranking) {
                    best = node;
                }
                // Δεξί υποδέντρο μπορεί να έχει μεγαλύτερους timestamps αλλά ίσως και καλύτερη κατάταξη
                findBestBefore(node->left, targetTimestamp, best);
                findBestBefore(node->right, targetTimestamp, best);
            } else {
                // Αν δεν είναι μικρότερος από τον στόχο, δεν εξετάζουμε το δεξί υποδέντρο
                findBestBefore(node->left, targetTimestamp, best);
            }
        }
        
        
        
    public:
        
        void insertPlayers(string filename){//1st method 
            fstream file{filename, ios::in}; 
            string str;
                
            if (!file) {
                cout << "No such file" << endl;
                return;
            }    

            while (getline(file,str)){
                participant* newP= new participant(str);
                insertAsBST(newP);//call helper functions
                heapifyUp(newP);
            }
            
            file.close();
        
        };
        void deletePlayer(const string& fname) {//2nd method
            participant* target = findByName(root, fname);
        
            if (!target) {
                cout << "Participant not found." << endl;
                return;
            }
        
            // Push down to leaf
            while (target->left || target->right) {
                participant* swapChild = nullptr;
        
                if (target->left && target->right) {
                    swapChild = (target->left->ranking < target->right->ranking) ? target->left : target->right;
                } else if (target->left) {
                    swapChild = target->left;
                } else {
                    swapChild = target->right;
                }
        
                // Swap all fields
                swap(target->fullname, swapChild->fullname);
                swap(target->ID, swapChild->ID);
                swap(target->ranking, swapChild->ranking);
                swap(target->origin, swapChild->origin);
                swap(target->timestamp, swapChild->timestamp);
        
                target = swapChild;
            }
        
            // Remove the leaf node
            if (target->parent) {
                if (target->parent->left == target) {
                    target->parent->left = nullptr;
                } else {
                    target->parent->right = nullptr;
                }
            } else {
                root = nullptr;
            }
        
            delete target;
        }
        
        void findKthBestPlayer(int k) {//3rd method
            if (k <= 0 || root == nullptr) {
                cout << "Invalid input or empty tree." << endl;
                return;
            }
        
            auto cmp = [](participant* a, participant* b) {
                return a->ranking > b->ranking; // min-heap based on ranking
            };
            priority_queue<participant*, vector<participant*>, decltype(cmp)> minHeap(cmp);
        
            minHeap.push(root);
            int count = 0; 
        
            while (!minHeap.empty()) {
                participant* current = minHeap.top();
                minHeap.pop();
                count++;
        
                if (count == k) {
                    cout << "The " << k << "-th best ranked player is:\n";
                    cout << "Full name: " << current->fullname << endl;
                    cout << "ID: " << current->ID << endl;
                    cout << "Ranking: " << current->ranking << endl;
                    cout << "Origin: " << current->origin << endl;
                    cout << "Timestamp: " << current->timestamp << endl;
                    return;
                }
        
                if (current->left) minHeap.push(current->left);
                if (current->right) minHeap.push(current->right);
            }
        
            cout << "Less than " << k << " players in the tree." << endl;
        }
        
        
        void printIDsInTimeRange(int startTime, int endTime) {//4rth method
            if(startTime>= endTime){
                cerr<<"End timestamp must be greater than start timestamp"<<endl;
                return;
            }

            printIDsInTimeRangeHelper(root, startTime, endTime);
        }
        // This is a helper method to recursively print the tree in-order
        void printInOrder(participant* node) {
            if (node == nullptr) return;  // Base case: if the node is null, do nothing

            // Traverse left subtree
            printInOrder(node->left);

            // Print current node's details (in this case, just the fullname, ID, ranking, and timestamp)
            cout << "Full name: " << node->fullname<<endl
                << ", ID: " << node->ID << endl
                << ", Ranking: " << node->ranking <<endl
                << ", Origin: " << node->origin << endl
                << ", Timestamp: " << node->timestamp << endl
                <<"------------------------------------------------"<<endl;

            // Traverse right subtree
            printInOrder(node->right);
        }


        // Method to print the entire tree
        void printTree() {

            cout << "Printing the tree (in-order traversal):" << endl;
            printInOrder(root);  // Start printing from the root
        }
        
        void deleteAllAfterTimestamp(int targetTimestamp) {//5th method
            // Βρίσκουμε τον πρώτο κόμβο με timestamp >= targetTimestamp
            participant* current = root;
            
            // Μετακινούμαστε στο κατάλληλο υποδέντρο ανάλογα με το timestamp
            while (current != nullptr) {
                if (current->timestamp >= targetTimestamp) {
                    break;
                }
                if (current->timestamp < targetTimestamp) {
                    current = current->right; // Μετακίνηση δεξιά αν το timestamp είναι μεγαλύτερο
                } else {
                    current = current->left; // Μετακίνηση αριστερά αν το timestamp είναι μικρότερο
                }
            }
        
            if (current == nullptr) {
                cout << "No participants found with the specified timestamp or greater." << endl;
                return;
            }
        
            // Ξεκινάμε τη διαγραφή όλων των κόμβων με timestamp >= targetTimestamp
            deleteSubtree(current);
        }
        
        void bestBeforeTimestamp(int targetTimestamp) {//6th method
            participant* best = nullptr;
            findBestBefore(root, targetTimestamp, best);
        
            if (best) {
                cout << "Best-ranked player before timestamp " << targetTimestamp << " is:" << endl;
                cout << "Full name: " << best->fullname << endl
                     << "ID: " << best->ID << endl
                     << "Ranking: " << best->ranking << endl
                     << "Origin: " << best->origin << endl
                     << "Timestamp: " << best->timestamp << endl;
            } else {
                cout << "No players found before the given timestamp." << endl;
            }
        }
        
        

};





void participant::parseData(const string &str) {
    size_t start = 0, end;

    // Extract full name (everything before the first comma)
    end = str.find(',');
    if (end == string::npos) {
        cerr << "Error: Missing comma after full name!" << endl;
        return;
    }
    fullname = str.substr(start, end - start);
    start = end + 1;

    // Extract ID
    end = str.find(',', start);
    if (end == string::npos) {
        cerr << "Error: Missing data fields!" << endl;
        return;
    }
    ID = stoi(str.substr(start, end - start));
    start = end + 1;

    // Extract ranking
    end = str.find(',', start);
    if (end == string::npos) {
        cerr << "Error: Missing data fields!" << endl;
        return;
    }
    ranking = stoi(str.substr(start, end - start));
    start = end + 1;

    // Extract origin
    end = str.find(',', start);
    if (end == string::npos) {
        cerr << "Error: Missing data fields!" << endl;
        return;
    }
    origin = str.substr(start, end - start);
    start = end + 1;

    // extract timestamp
    timestamp = stoi(str.substr(start));
}







int menu(){
    int answ;
    string message = "-----------------------------------------------------------\n"
                     "                      MAIN MENU                            \n"
                     "-----------------------------------------------------------\n"
                     "1. Load players from file\n"
                     "2. Delete a player\n"
                     "3. Show the k-th best-ranked participant\n"
                     "4. Show IDs of players within a certain time frame\n"
                     "5. Delete all users after a specific date\n"
                     "6. Show ID of the best-ranked player before a specific date\n"
                     "7. Exit\n"
                     "(8. Show all players by traversal(helper function))\n"
                     "-----------------------------------------------------------\n"
                     "Enter your choice (1-7): ";
    cout<< message<< endl;
    
    cin >> answ;
    while(cin.fail()){//error handling
        cin.clear();
        cin.ignore();
        cerr<<"Invalid input. Please give number: ";
        cin>> answ;        
    }
    return answ;
};
void actions(int answ,Tree& tree1){
    string filename,fname;
    int k;
    int l;

    switch (answ) {
        case 1:
            cout << "Option 1 selected: Load players from file. Please give the name of the file: " << endl;
            cin >> filename;
            tree1.insertPlayers(filename);

            break;
        case 2:
            cout << "Option 2 selected: Delete a player. Please give full name: " << endl;

            cin.ignore();
            getline(cin, fname);
            tree1.deletePlayer(fname);

            break;
        case 3:
        
            cout << "Option 3 selected: Show the k-th best-ranked participant." << endl;
            cout << "Give the number k: ";
            cin >> k;
            while (cin.fail()){
                cin.clear();
                cin.ignore();
                cout << "Wrong input. Please give number: ";
                cin>>k;
            }
            tree1.findKthBestPlayer(k);

            break;
        case 4:
            cout << "Option 4 selected: Show IDs of players within a certain time frame." << endl;
            cout << "Give starting timestamp: ";
            cin >> k;
            cout << "Give ending timestamp";
            cin >> l;

            tree1.printIDsInTimeRange(k,l);
            break;
        case 5:
            cout << "Option 5 selected: Delete all users after a specific date." << endl;
            cout << "Give timestamp: ";
            cin >> l;
            while (cin.fail()){
                cin.clear();
                cin.ignore();
                cout<< "Wrong Input. Please give number: ";
                cin>> l;
            }
            cout<<'\n';

            tree1.deleteAllAfterTimestamp(l);

            break;
        case 6:
            cout << "Option 6 selected: Show ID of the best-ranked player before a specific date." << endl;
            cout << "Give timestamp: ";
            cin >> l;
            while (cin.fail()) {
                cin.clear();
                cin.ignore();
                cout << "Wrong input. Please give a number: ";
                cin >> l;
            }

            cout<< '\n';
            tree1.bestBeforeTimestamp(l);
            break;
        case 7:
            cout << "Exiting program." << endl;
            break;
        case 8:
            tree1.printTree();
            break;
        default:
            cout << "Invalid choice. Please select a valid option." << endl;
            break;
    }
};

int main(){
    int answ = menu();
    Tree tree1;

    while (answ !=7){
        actions(answ,tree1);
        answ = menu();
    }


    
    return 0;
}