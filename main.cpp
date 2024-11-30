#include<iostream>
#include<chrono>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<iomanip>
#include "RECORD_AVL.h"
#include "RECORD_BST.h"
#include "RECORD_BTREE.h"



#define int long long int

using namespace std;
using namespace std::chrono;

string getRandomName(){
    string s = "";
    s.push_back((char)('A' + rand()%26));
    s.push_back((char)('A' + rand()%26));
    s.push_back((char)('A' + rand()%26));
    s.push_back((char)('A' + rand()%26));
    return s;
}

Record getDummyRecord(){
    Record rec;
    int id = rand() % 50001;
    int age = rand() % 100;
    string name = getRandomName();

    rec.id = id;
    rec.age = age;
    rec.name = name;

    return rec;
}

int getRandomID(){
    return rand()%5001;
}


template<typename T>
double insertionTime(T *&table, int record_size){
    auto start = high_resolution_clock::now();

    for(int i=0;i<record_size;i++){
        Record rec = getDummyRecord();
        table->insert(rec);
    }

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);


    return (double)(duration.count()*1.0);

}

template<typename T>
double searchingTime(T *&table, int record_size){
    auto start = high_resolution_clock::now();

    for(int i=0;i<record_size;i++){
        int id = getRandomID();
        table->search(id);
    }

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);


    return (double)(duration.count()*1.0);

}


template<typename T>
double deletionTime(T *&table, int record_size){
    auto start = high_resolution_clock::now();

    for(int i=0;i<record_size;i++){
        int id = getRandomID();
        table->remove(id);
    }

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);



    return (double)(duration.count()*1.0);
}




signed main(){
    srand(time(NULL));
    AVL *avl_table = new AVL();
    BST *bst_table = new BST();
    BTree *btree_table = new BTree(3);
    vector<double> avg_insertion_times(3);
    vector<double> avg_searching_times(3);
    vector<double> avg_deletion_times(3);


    string operations[3] = {"Insertion", "Searching", "Deletion"};

    int record_size = 10000000;
    // inserting, searching, and deleting 1000 records in AVL
    avg_insertion_times[0] = insertionTime(avl_table, record_size);
    avg_searching_times[0] = searchingTime(avl_table, record_size);
    avg_deletion_times[0] = deletionTime(avl_table, record_size);

    // inserting, searching, and deleting 1000 records in bst
    avg_insertion_times[1] = insertionTime(bst_table, record_size);
    avg_searching_times[1] = searchingTime(bst_table, record_size);
    avg_deletion_times[1] = deletionTime(bst_table, record_size);


    // inserting, searching, and deleting 1000 records in btree
    avg_insertion_times[2] = insertionTime(btree_table, record_size);
    avg_searching_times[2] = searchingTime(btree_table, record_size);
    avg_deletion_times[2] = deletionTime(btree_table, record_size);


    cout << left << setw(15) << "Operation"
         << setw(10) << "AVL"
         << setw(10) << "BST"
         << setw(10) << "BTREE" << endl;
        
    cout << string(50, '-') << endl;

    cout << left << setw(15) << operations[0]
        << setw(10) <<  fixed << setprecision(3) << avg_insertion_times[0]
        << setw(10) <<  fixed << setprecision(3) << avg_insertion_times[1]
        << setw(10) <<  fixed << setprecision(3) << avg_insertion_times[2] << endl;

    cout << left << setw(15) << operations[1]
    << setw(10) <<  fixed << setprecision(3) << avg_searching_times[0]
    << setw(10) <<  fixed << setprecision(3) << avg_searching_times[1]
    << setw(10) <<  fixed << setprecision(3) << avg_searching_times[2] << endl;

    cout << left << setw(15) << operations[2]
        << setw(10) <<  fixed << setprecision(3) << avg_deletion_times[0]
        << setw(10) <<  fixed << setprecision(3) << avg_deletion_times[1]
        << setw(10) <<  fixed << setprecision(3) << avg_deletion_times[2] << endl;
        

}