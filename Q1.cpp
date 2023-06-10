
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



// 2D Linked List Node class
class ListNode {
public:
    int data;
    int color;
    ListNode* right;
    ListNode* down;
    int start_white; // index of the start of white pixels
    int end_white;   // index of the end of white pixels
    int start_black; // index of the start of black pixels
    int end_black;   // index of the end of black pixels

    ListNode(int data, int sw, int ew, int sb, int eb) {
        this->data = data;
        this->right = NULL;
        this->down = NULL;
        this->start_white = sw;
        this->end_white = ew;
        this->start_black = sb;
        this->end_black = eb;
    }

    ListNode() : data(0), right(nullptr), down(nullptr) {}
    ListNode(int data) : data(data), right(nullptr), down(nullptr) {}
   // ListNode(intdata) : data(data), right(nullptr), down(nullptr) {}
};

////// 2D Linked List class
class LinkedList2D {
public:
    ListNode* head;

    LinkedList2D() : head(nullptr) {}



ListNode* convertTo2dLL(int** arr, int rows, int cols) {
    head = new ListNode();
    head->data = -1; // indicate end of black pixels

    ListNode* currentRow = head;
    for (int i = 0; i < rows; i++) {
        currentRow->down = new ListNode();
        currentRow->down->data = i + 1; // set row
        currentRow = currentRow->down;
        ListNode* currentCol = currentRow;
        currentCol->right = new ListNode();
        bool blackPixelStarted = false;
        bool whitePixelStarted = false;
        for (int j = 0; j < cols; j++) {
            if (arr[i][j] == 255) { // white pixel
                if (!whitePixelStarted) {
                    currentCol->right = new ListNode();
                    currentCol->right->data = j + 1;
                    currentCol = currentCol->right;
                    whitePixelStarted = true;
                }
                if (j == cols - 1 || arr[i][j + 1] == 0) {
                    currentCol->right = new ListNode();
                    currentCol->right->data = j + 1;
                    currentCol = currentCol->right;
                    whitePixelStarted = false;
                }
            }
        }
        if (!whitePixelStarted) {
            currentCol->right = new ListNode();
            currentCol->right->data = -2;
            currentCol = currentCol->right;
        }
        for (int j = 0; j < cols; j++) {
            if (arr[i][j] == 0) { // black pixel
                if (!blackPixelStarted) {
                    currentCol->right = new ListNode();
                    currentCol->right->data = j + 1;
                    currentCol = currentCol->right;
                    blackPixelStarted = true;
                }
                if (j == cols - 1 || arr[i][j + 1] == 255) {
                    currentCol->right = new ListNode();
                    currentCol->right->data = j + 1;
                    currentCol = currentCol->right;
                    blackPixelStarted = false;
                }
            }
        }
        if (!blackPixelStarted) {
            currentCol->right = new ListNode();
            currentCol->right->data = -1;
            currentCol = currentCol->right;
        }
    }
    return head;
}

    

    void printLinkedListAsMatrix(LinkedList2D& linkedList, int rows, int cols) 
    {
        if (linkedList.head == nullptr) {
            return;
        }
        ListNode* rowHead = linkedList.head;
        ListNode* currentRow = rowHead;
        ListNode* currentCol = nullptr;

        // row traverse
        for (int i = 0; i < rows; i++) {
            currentCol = currentRow;
            for (int j = 0; j < cols; j++) {
                if (currentCol != nullptr) {
                    cout << currentCol->data << " ";
                    currentCol = currentCol->right;
                }
            }
            cout << endl;
            currentRow = currentRow->down;
        }
    }
    

};



// Quadtree Node class
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

    //Q1
    cv::Mat img = cv::imread("D:\\####Uni Fourth Sem\\DS\\A4\\Test_Images\\Student Test Images\\t1.bmp", cv::IMREAD_GRAYSCALE);

    if (img.empty()) {
        cout << "Failed to read image!" << std::endl;
        return -1;
    }
    else
        cout << "Alhamdulillah, Image read successfully!" << endl;

    
    int rows = img.rows;
    int cols = img.cols;
    int** imgArray = new int* [rows];
    for (int i = 0; i < rows; i++) {
        imgArray[i] = new int[cols];
        uchar* p = img.ptr<uchar>(i);
        for (int j = 0; j < cols; j++) {
            imgArray[i][j] = static_cast<int>(p[j]);
        }
    }


    // Print the contents of imgArray
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << imgArray[i][j] << " ";
        }
        cout << endl;
    }


    // Compare the values of imgArray with the pixel values of the original image
    for (int i = 0; i < rows; i++) 
    {
        for (int j = 0; j < cols; j++) 
        {
            if (imgArray[i][j] != static_cast<int>(img.at<uchar>(i, j))) 
            {
                cout << "Pixel values don't match at (" << i << "," << j << ")" << endl;
                return -1;
            }
        }
    }
    cout << "Pixel values match!" << endl;


    // Convert the 2D array to a 2D linked list
   LinkedList2D linkedList;
   ListNode* newhead = linkedList.convertTo2dLL(imgArray, img.rows, img.cols);
   cout << "-------------------------------------------" << endl;
   cout << "-------------------------------------------" << endl;
   cout << "-------------------------------------------" << endl;
   cout << "-------------------------------------------" << endl;
   cout << "-------------------------------------------" << endl;
   cout << "-------------------------------------------" << endl;
   cout << "-------------------------------------------" << endl;
   cout << "-------------------------------------------" << endl;
   cout << "-------------------------------------------" << endl << endl << endl << endl;

   linkedList.printLinkedListAsMatrix(linkedList, img.rows, img.cols);
   
   cout << "200 1 20 41 100 111 150 161 170 181 190 -2 21 40 101 110 151 160 171 180 191 200 -1" << endl;
   cout << "hello " << endl;
   QuadTree qt;
   node * root = qt.Quadtreeconverter(0, 0, img.rows, img.cols, imgArray);
   qt.bfs(root);
   qt.writeNodeToFile(root, 0, 0, img.rows, img.cols);
   
 return 0;
}

