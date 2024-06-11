#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

// Define the Node structure
struct Node {
    string word;
    Node* left;
    Node* right;
};

// Function to create a new Node
Node* createNode(string word) {
    Node* newNode = new Node();
    newNode->word = word;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Function to insert a word into the BST
void insert(Node*& root, string word) {
    if (root == NULL) {
        root = createNode(word);
        return;
    }
    if (word < root->word)
        insert(root->left, word);
    else if (word > root->word)
        insert(root->right, word);
}

// Function to read words from a text file and insert them into the BST
void readWordsFromFileAndInsertIntoBST(Node*& root, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }

    string word;
    while (file >> word) {
        insert(root, word);
    }

    file.close();
}

// Function to perform prefix-based search in the BST
void prefixSearch(Node* root, const string& prefix, vector<string>& suggestions) {
    if (root == NULL)
        return;

    // If current word starts with the prefix, add it to suggestions
    if (root->word.find(prefix) == 0)
        suggestions.push_back(root->word);

    // Recursively search in left and right subtrees
    if (prefix < root->word)
        prefixSearch(root->left, prefix, suggestions);
    else
        prefixSearch(root->right, prefix, suggestions);
}

// Function to display auto-suggestions for a given prefix
vector<string> displayAutoSuggestions(Node* root, const string& prefix) {
    vector<string> suggestions;
    prefixSearch(root, prefix, suggestions);

    cout << "----> " << prefix << endl;
    cout << endl;
    if(suggestions.size() == 0) return {"-"};
    return suggestions;
}

void printFormatted(const vector<string>& words, int maxWidth = 50) {
    ostringstream oss;
    int currentWidth = 0;

    cout << "+" << string(maxWidth, '-') << "+" << endl;

    for (const auto& word : words) {
        if (currentWidth + word.length() + 2 > maxWidth) {
            // If adding the next word exceeds maxWidth, print the current line
            cout << "| " << oss.str() << string(maxWidth - currentWidth - 1, ' ') << "|" << endl;
            oss.str("");  // Clear the stream
            oss.clear();  // Clear any error flags
            currentWidth = 0;
        }

        oss << word << "  ";  // Add the word and two spaces
        currentWidth += word.length() + 2;
    }

    // Print any remaining words
    if (currentWidth > 0) {
        cout << "| " << oss.str() << string(maxWidth - currentWidth - 1, ' ') << "|" << endl;
    }

    cout << "+" << string(maxWidth, '-') << "+" << endl;
    cout << endl;
}

// Driver code
int main() {

    cout << "Initialising dictionary" << endl;
    Node* root = NULL;

    // Read words from the file and insert them into the BST
    string filename = "words.txt";
    readWordsFromFileAndInsertIntoBST(root, filename);

    // Test auto-suggestions for a prefix
    string prefix = "";
    cout << "Enter the letters one by one to get suggestions." << endl;
    cout << "Press < to delete a letter" << endl;
    cout << endl;
    while(true){
        cout << "Typed key: ";
        char ch;
        cin >> ch;
        if(ch != '<') prefix += ch;
        else if(ch == '<' and prefix.size() > 0) prefix.pop_back();
        if(prefix.size()) printFormatted(displayAutoSuggestions(root, prefix));
    }

    return 0;
}
