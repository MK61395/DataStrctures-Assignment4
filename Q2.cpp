#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stack>
#include <queue>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <fstream>
#include <filesystem>


using namespace cv;
using namespace std;



class node {
public:
    string color;
    node* topLeft;
    node* topRight;
    node* bottomLeft;
    node* bottomRight;
    bool Leaf;
    
};

// Quadtree class


class QuadTree {
public:
    node* root;

    QuadTree() : root(nullptr) {}
    node* Quadtreeconverter(int beginRow, int beginCol, int endRow, int endCol, int** arr)
    {
        bool flag_black = false;
        bool flag_white = false;
        int i = beginRow;
        while (i < endRow)
        {
            int j = beginCol;
            while (j < endCol)
            {
                switch (arr[i][j])
                {
                case 0:
                    flag_black = true;
                    break;
                case 255:
                    flag_white = true;
                    break;
                }
                j++;
            }
            i++;
        }
        node* newnode = new node;

        if (flag_black)
        {
            if (!flag_white)
            {
                newnode->color = "Black";
                newnode->Leaf = true;
                return newnode;
            }
        }


        if (!flag_black)
        {
            if (flag_white)
            {
                newnode->color = "White";
                newnode->Leaf = true;
                return newnode;
            }
        }


        if (flag_black)
        {
            if (flag_white)
            {
                newnode->color = "Grey";
                int row2 = (beginRow + endRow) / 2;
                int col2 = (beginCol + endCol) / 2;
                newnode->topLeft = Quadtreeconverter(beginRow, beginCol, row2, col2, arr);
                newnode->topRight = Quadtreeconverter(beginRow, col2, row2, endCol, arr);
                newnode->bottomLeft = Quadtreeconverter(row2, beginCol, endRow, col2, arr);
                newnode->bottomRight = Quadtreeconverter(row2, col2, endRow, endCol, arr);
                return newnode;
            }
        }
    }

    void bfs(node* root)
    {
        if (root == nullptr) return;
        queue<node*> q;
        q.push(root);
        while (!q.empty())
        {
            int numNodes = q.size();
            for (int i = 0; i < numNodes; i++) {
                node* current = q.front();
                q.pop();
                int colorVal = 0; // default value for black
                if (current->color == "Grey") {
                    colorVal = 128;
                }
                else if (current->color == "White") {
                    colorVal = 255;
                }
                switch (colorVal) 
                {
                case 0:
                    cout << "Black";
                    break;
                case 128:
                    cout << "Grey";
                    break;
                case 255:
                    cout << "White";
                    break;
                }
                cout << " ";
                if (current->topLeft) q.push(current->topLeft);
                if (current->topRight) q.push(current->topRight);
                if (current->bottomLeft) q.push(current->bottomLeft);
                if (current->bottomRight) q.push(current->bottomRight);
            }
            cout << endl;
        }
    }

    void writeNodeToFile(node* root, int beginRow, int beginCol, int endRow, int endCol) {
        // Create file for root node
        if (!root) {
            // handle null pointer error
            //cout << "Masla" << endl;
            return;
        }
        fstream outfile;
        outfile.open("root.txt" ,ios::out);
        outfile << "Color: " << root->color << endl;
        outfile << "Leaf: " << (root->Leaf ? "true" : "false") << endl;
        outfile << "Start Row: " << beginRow << endl;
        outfile << "End Row: " << endRow << endl;
        outfile << "Start Col: " << beginCol << endl;
        outfile << "End Col: " << endCol << endl;
        outfile.close();

        // Recursively create files for each child node
        if (!root->Leaf) {
            writeNodeToFile(root->topLeft, beginRow, beginCol, (beginRow + endRow) / 2, (beginCol + endCol) / 2);
            writeNodeToFile(root->topRight, beginRow, (beginCol + endCol) / 2 + 1, (beginRow + endRow) / 2, endCol);
            writeNodeToFile(root->bottomLeft, (beginRow + endRow) / 2 + 1, beginCol, endRow, (beginCol + endCol) / 2);
            writeNodeToFile(root->bottomRight, (beginRow + endRow) / 2 + 1, (beginCol + endCol) / 2 + 1, endRow, endCol);
        }
        else {
            // Create file for leaf node
            string filename = "node1.txt";
            static int count = 1;
            if (count > 1) {
                filename = "node" + to_string(count) + ".txt";
            }
            count++;
            fstream outfile;
            outfile.open(filename, ios::out);
            outfile << "Color: " << root->color << endl;
            outfile << "Leaf: " << (root->Leaf ? "true" : "false") << endl;
            outfile << "Start Row: " << beginRow << endl;
            outfile << "End Row: " << endRow << endl;
            outfile << "Start Col: " << beginCol << endl;
            outfile << "End Col: " << endCol << endl;
            outfile.close();
        }
    }


};


int main() {

fstream file("root.txt", ios::in);

   // Check if the file is open
   if (!file.is_open()) {
       cerr << "Failed to open file." << endl;
       return 1;
   }

   // Read the contents of the file
   string color;
   bool leaf;
   int startRow, endRow, startCol, endCol;
   file >> color >> leaf >> startRow >> endRow >> startCol >> endCol;

   // Close the file
   file.close();

   // Iterate over the files
   //int numfiles = 7055;

   // Arrays to store data for all nodes
const int NUM_NODES = 7055;
string colors[NUM_NODES];
bool leafs[NUM_NODES];
int startRows[NUM_NODES];
int endRows[NUM_NODES];
int startCols[NUM_NODES];
int endCols[NUM_NODES];

// Initialize the arrays
std::fill(colors, colors + NUM_NODES, "");
std::fill(leafs, leafs + NUM_NODES, false);
std::fill(startRows, startRows + NUM_NODES, -1);
std::fill(endRows, endRows + NUM_NODES, -1);
std::fill(startCols, startCols + NUM_NODES, -1);
std::fill(endCols, endCols + NUM_NODES, -1);

// Read data from the files and store it in the arrays
for (int i = 1; i <= NUM_NODES; i++) {
    string filename = "node" + to_string(i) + ".txt";
    ifstream nodeFile(filename);

    nodeFile >> colors[i - 1] >> leafs[i - 1] >> startRows[i - 1] >> endRows[i - 1] >> startCols[i - 1] >> endCols[i - 1];
}

// Print the data in the arrays
for (int i = 0; i < NUM_NODES; i++) 
{
    cout << "Color: " << colors[i] << endl;
    cout << "Leaf: " << (leafs[i] ? "true" : "false") << endl;
    cout << "Start Row: " << startRows[i] << endl;
    cout << "End Row: " << endRows[i] << endl;
    cout << "Start Col: " << startCols[i] << endl;
    cout << "End Col: " << endCols[i] << endl;
}


    
       // Construct the quadtree using the read values
       int** arr = new int* [endRow - startRow];
       for (int i = 0; i < endRow - startRow; i++) 
       {
           arr[i] = new int[endCol - startCol];
           // Fill the array with pixel values
       }
       QuadTree qt2;
       node* root2 = qt2.Quadtreeconverter(startRow, startCol, endRow, endCol, arr);
       qt2.bfs(root2);
  

return 0;
}


