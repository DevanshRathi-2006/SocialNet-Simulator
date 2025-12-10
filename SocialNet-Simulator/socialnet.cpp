#include <iostream>
#include <vector>
#include <sstream>        // used this for using ss(stringstream) input in the main function 
#include <algorithm>      // For sort and max functions usage
#include <queue>          // For BFS implementaion is used 
#include <unordered_map> // Allowed per instructions, for user_to_id mapping

using namespace std;

class AVLTree {
private:
    struct Node {     // keeping the struct node inside it is a node struct for only AVL tree, we have 
                     //no need to access it globally so i kept it inside which prevents leakage of data
    long long key;    // for timestamp
    string value;    // for post content;
    Node* left;
    Node* right;
    int height;

    Node(long long k, string value) : key(k), value(value),left(nullptr), right(nullptr), height(1) {}
};
    Node* root;

    int height(Node* N) {
        if (N == nullptr)
            return 0;
        return N->height;
    }

    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;    

        // Update heights
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        // Return new root
        return x;
    }

    Node* leftRotate(Node* x) {     
        Node* y=x->right;
        Node* T2=y->left;
        
        y->left=x;
        x->right=T2;   
        
        x->height = max(height(x->left), height(x->right)) + 1;  // x==child of y
        y->height = max(height(y->left), height(y->right)) + 1;    // y==parent

        return y;                         
    }

    
    int getBalance(Node* N) {
        if (N == nullptr)
            return 0;
        return height(N->left) - height(N->right);
    }

    
    Node* insertHelper(Node* node, long long key,string value) {
        // Standard BST insertion is we are doing here.
        if (node == nullptr)
            return new Node(key,value);
                                                 
        if (key < node->key)
            node->left = insertHelper(node->left, key,value);
        else if (key > node->key)
            node->right = insertHelper(node->right, key,value);
        else     // Duplicate keys are not allowed
            return node;

        // Update height of the ancestor node
        node->height = 1 + max(height(node->left), height(node->right));

        //Get the balance factor to check for imbalance
        int balance = getBalance(node);

        //If unbalanced, perform rotations
        // Left Left Case                         //left subtree is left heavy.
        if(balance>1 && key<node->left->key){
            return rightRotate(node);
        }

        // Right Right Case
        if(balance<-1 && key>node->right->key){
            return leftRotate(node);
        }

        // Left Right Case
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if(balance<-1 && key<node->right->key){
            node->right=rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    // Traversal for Posts
    // Helper for OUTPUT_POSTS. Traverses in reverse order (Right, Node, Left)
    // to get newest posts first.
    void reverseInOrderHelper(Node* node, int& n, vector<string>& posts) {
        if (node == nullptr || (n != -1 && n <= 0)) {
            return;
        }
        //Recurse on right subtree ( on newest posts)
        reverseInOrderHelper(node->right, n, posts);

        // 2. Process this node (if we still need posts)
        if (n == -1 || n > 0) {
            posts.push_back(node->value);
            if (n != -1) {
                n--;
            }
        }

        //Recurse on left subtree (oldest posts)
        if (n == -1 || n> 0) { // Only continue if we still need posts
            reverseInOrderHelper(node->left, n, posts);
        }
    }

    // Recursive Delete for Tree Cleanup 
    void clear(Node *node) {
        if (node != nullptr) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

public:

    //this is the Default Constructor
    AVLTree() : root(nullptr) {}

    //this is the Destructor 
    ~AVLTree() {
        clear(root);
    }

    //Delete Copy Constructor
    AVLTree(const AVLTree &other) = delete;

    //Delete Copy Assignment Operator
    AVLTree &operator=(const AVLTree &other) = delete;

    //Default Move Constructor (this is what vector::push_back will use)
    AVLTree(AVLTree &&other) noexcept : root(other.root) {
        other.root = nullptr; //set old root to null
    }

    //this is the Default Move Assignment Operator
    AVLTree &operator=(AVLTree &&other) noexcept {
        if (this != &other) {
            clear(root);          // Delete our current tree
            root = other.root;    // Take their root
            other.root = nullptr; // Set their root to null
        }
        return *this;
    }

    //Public Functions

    // Public insert function
    void insert(long long key, const string &value) {
        root = insertHelper(root, key, value);
    }

    // Public function to get the n most recent posts
    vector<string> getRecentPosts(int n) {
        vector<string> posts;
        reverseInOrderHelper(root, n, posts);
        return posts;
    }

};

// User struct for use in graph implementation 
struct User{
    string Username;
    AVLTree posts;
    string originalUsername;
    User(string uname,string origUName) : Username(uname), originalUsername(origUName) {}
};

//Graph Implementation (SocialNet) 

class SocialNet {
private:
    // Adjacency list: index = user ID, value = vector of friend IDs
    vector<vector<int>> adj;

    // Stores all User objects. The index is the user ID.
    vector<User> users;

    //Maps a lowercase username string to its integer ID.
    unordered_map<string, int> user_to_id;

    // Gets a unique timestamp for a new post
    long long postcounter;     // postcounter is the variable here for time of 
                           // posting which is unique for every post

    // Helper Functions: 

    // Helper to convert a string to lowercase
    string toLower(string s) {
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

    long long getTimestamp(){    
        postcounter++;
        return postcounter;
    }

    // Check if two users are friends (by ID)
    // add friends and check
    bool areFriends(int userId1, int userId2) {
        if (userId1 >= adj.size() || userId2 >= adj.size()){
            return false;
        } 
        
        // Check if userId2 is in userId1's friend list
        for (int friendId : adj[userId1]) {
            if (friendId == userId2) {
                return true;
            }
        }
        return false;
    }

    // This is the Helper struct for friend suggestions
    struct FriendSuggestion {
        int id;
        string username;
        int mutualFriends;

        FriendSuggestion(int i, string u, int m) : id(i), username(u), mutualFriends(m) {}

        // this is sorting for std::sort
        static bool compare(const FriendSuggestion& a, const FriendSuggestion& b) {
            if (a.mutualFriends != b.mutualFriends) {
                return a.mutualFriends > b.mutualFriends; // Highest mutuals first
            }
            return a.username < b.username; // Alphabetical tie-breaker 
        }
    };

public:
    SocialNet() : postcounter(0){}

    //3.1 Social Network Operations

    void addUser(string username) {
        string lowerUser = toLower(username);
        if (user_to_id.count(lowerUser)) {
            // User already exists
            return;
        }

        // This user's new ID will be the current size of the users vector
        int newID = users.size();
        user_to_id[lowerUser] = newID;

        // Add the new user to the users list
        users.push_back(User(lowerUser, username));

        // Add a new (empty) entry for this user in the adjacency list
        adj.resize(users.size());
    }

    void addFriend(string username1, string username2) {
        string lowerUser1 = toLower(username1);
        string lowerUser2 = toLower(username2);

        // Check if both users exist
        if(!user_to_id.count(lowerUser1) || !user_to_id.count(lowerUser2)){
        return;
        }

        int id1 = user_to_id[lowerUser1];
        int id2 = user_to_id[lowerUser2];

        // Check if they are already friends
        for (int friendId : adj[id1]) {
            if (friendId == id2) {
                return; // Already friends
            }
        }

        // Add the bidirectional friendship edge
        adj[id1].push_back(id2);
        adj[id2].push_back(id1);
    }

    void ListFriends(string username){
        string lowerUser=toLower(username);
        if(!user_to_id.count(lowerUser)){
            return ;
        }
        int userId=user_to_id[lowerUser];
        vector<string> friendNames;

        // Getting all friend names from their IDs
        for(int friendId:adj[userId]){
            friendNames.push_back(users[friendId].originalUsername);
        }
        // Sorting them alphabetically
        sort(friendNames.begin(),friendNames.end());

        //Printing the list of friends.
        for(const string& name :friendNames){
            cout<<name<<" "<<endl;
        }
        cout<<"\n";
    }

    void SUGGEST_FRIENDS(string username,int n){
        if(n==0){
            return;
        }
        string lowerUser=toLower(username);
        if(!user_to_id.count(lowerUser)){     // if user doesn't exist
            return;
        }

        int userId=user_to_id[lowerUser];
        //Create a set of the user's current friends for fast lookup
        unordered_map<int ,bool> isFriend;
        isFriend[userId]=true;      // we cant suggest self
        for(int friendId:adj[userId]){
            isFriend[friendId]=true;
        }

        //Find all "friends of friends" (FoFs)
        // We use a map to count mutual friends
        unordered_map<int,int> fof_mutuals;
        for(int friendId:adj[userId]){
            for(int fof_id:adj[friendId]){
                if(!isFriend.count(fof_id)){
                    fof_mutuals[fof_id]++;
                }
            }
        }
        //Convert map to a vector of FriendSuggestion structs
        vector<FriendSuggestion> suggestions;      // line 230 where we created our own helper struct for Friend suggestions
        for(auto const& pair:fof_mutuals){     // here pair returns us a [id,count] pair from where it is stored in fof_mutuals 
            int id=pair.first;
            int count=pair.second;
            suggestions.push_back(FriendSuggestion(id,users[id].originalUsername,count));
        }
        //sort the suggestions
        sort(suggestions.begin(),suggestions.end(),FriendSuggestion::compare);

        //print the top n suggestions
        for(int i=0;i<suggestions.size() && i<n;++i){
            cout<<suggestions[i].username<<" "<<endl;
        }
        cout<<endl;
    }

    int DEGREES_OF_SEPARATION(string username1,string username2){
        string loweruser1=toLower(username1);
        string loweruser2=toLower(username2);

        if(!user_to_id.count(loweruser1) || !user_to_id.count(loweruser2)){
            return -1;
        }
        int startId=user_to_id[loweruser1];
        int endId=user_to_id[loweruser2];

        if(startId==endId){
            return 0;
         }
        // Use Breadth-First Search (BFS) to find the shortest path
        queue<pair<int,int>> q;    // Pair: {userId, distance}
        // q.push(make_pair(startId,0));
        q.push({startId,0});        // we are pushing a pair {startId,0} in queue q
        unordered_map<int,bool> visited;
        visited[startId]=true;
        // while(!q.empty()){
        //     // pair<int,int> q;
        //     int currId=q.front().first;
        //     int distance=q.front().second;
        //     q.pop();
        while (!q.empty()) {
            std::pair<int, int> frontOfQueue = q.front();
            int currId = frontOfQueue.first;
            int distance = frontOfQueue.second;
            q.pop();
            for(int friendId:adj[currId]){
                if(friendId==endId){
                    return distance+1;
                }
                if(!visited.count(friendId)){
                    visited[friendId]=true;
                    q.push(make_pair(friendId,distance+1)); // here we make a pair of {friendId,distance} and push it into the queue q
                }
            }
        }
        return -1  ;  // if no path found
    }

//3.2 User Content Operations

    void ADD_POST(string username,string postcontent){
        string loweruser=toLower(username);
        if(!user_to_id.count(loweruser)){
            return;
        }
        int userId=user_to_id[loweruser];
        long long postkey=getTimestamp();
        users[userId].posts.insert(postkey,postcontent);
    }

    void OUTPUT_POSTS(string username,int n){
        string loweruser=toLower(username);
        if(!user_to_id.count(loweruser)){
            return;
        }
        int userId=user_to_id[loweruser];
        vector<string> posts=users[userId].posts.getRecentPosts(n);    // the posts in rhs comes from our implementation of 
                                             // getRecentPosts function . posts in rhs and lhs are different
        for(const string& post :posts){
            cout<<post<<endl;
        }
    }
};    

int main(){
    SocialNet network;
    string line;

    // Read commands from std line
    while(getline(cin,line)){
        stringstream ss(line);     // due to sstream library can use this
        string command;
        ss>>command;

        if(command=="ADD_USER"){
            string username;
            ss>>username;
            network.addUser(username);
        }else if(command=="ADD_FRIEND"){
            string u1,u2;
            ss>>u1>>u2;
            network.addFriend(u1,u2);
        }else if(command=="LIST_FRIENDS"){
            string username;
            ss>>username;
            network.ListFriends(username);
        }else if(command=="SUGGEST_FRIENDS"){
            string username;int n;
            ss>>username>>n;
            network.SUGGEST_FRIENDS(username,n);
        }else if(command=="DEGREES_OF_SEPARATION"){
            string u1,u2;
            ss>>u1>>u2;
            int degrees=network.DEGREES_OF_SEPARATION(u1,u2);
            cout<<degrees<<endl;
        }else if(command=="ADD_POST"){
            string username;
            ss>>username;
            // Read the rest of the line as the post content
            string postcontent;
            getline(ss,postcontent);

            // // we here remove leading space and quotes
            // // we here remove the single leading space left by getline
            if (!postcontent.empty()) {
                // 1. Find the first non-space character
                size_t first = postcontent.find_first_not_of(' ');
                if (first == string::npos) { // String is all spaces
                    postcontent = "";
                } else {
                    //Find the last non-space character
                    size_t last = postcontent.find_last_not_of(' ');
                    postcontent = postcontent.substr(first, (last - first + 1));
                }
            }

            // If the string starts and ends with quotes, remove them.
            if (!postcontent.empty() && postcontent.front() == '"' && postcontent.back() == '"' && postcontent.length() > 1) {
                //Extract content from inside quotes and add the post
                string finalContent = postcontent.substr(1, postcontent.length() - 2);
                network.ADD_POST(username, finalContent); 
            } else {
                //this is a invalid case as Input is not in quotes, or is just "".
            }

        }else if(command=="OUTPUT_POSTS"){
            string username;
            int n;
            ss>>username>>n;
            network.OUTPUT_POSTS(username,n);
        }
    }
    return 0;
}


