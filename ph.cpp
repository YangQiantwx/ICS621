#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <complex>
#define INF 987654321
#define BILLION 1000000000L
using namespace std;
typedef long long lld;
typedef unsigned long long llu;

/*
 Code modified from Petar Veličković
 https://github.com/PetarV-/Algorithms/tree/master/Data%20Structures
 Pairing Heap data structure satisfying the heap property, with highly efficient asymptotic bounds.
 Often outperforming Fibonacci heaps, and simple to implement.
 Complexity:    O(1) for insert, make and merge
 O(log n) amortized for extractMin and delete
 O(log n) amortized for decreaseKey - not known if bound is tight; Omega(log log N).
*/

struct PNode
{
    int key;
    PNode *b, *f, *c;
    
    PNode()
    {
        this -> key = 0;
        this -> b = this -> f = this -> c = NULL;
    }
    
    PNode(int key)
    {
        this -> key = key;
        this -> b = this -> f = this -> c = NULL;
    }
};

class PHeap
{
    PNode *root;
    
public:
    PHeap();
    PHeap(PNode*);
    bool isEmpty();
    void insert(PNode*);
    void merge(PHeap*);
    PNode* make();
    PNode* extractMin();
    void decreaseKey(PNode*, int);
    void Delete(PNode*);
};

PHeap::PHeap()
{
    this -> root = NULL;
}

PHeap::PHeap(PNode *x)
{
    this -> root = x;
    x -> b = x -> f = NULL;
}

bool PHeap::isEmpty()
{
    return (this -> root == NULL);
}

void PHeap::insert(PNode *x)
{
    //insert node which can be referred to the merge
    this -> merge(new PHeap(x));
}

void PHeap::merge(PHeap *ph)
{
    if (ph -> isEmpty()) return;
    
    if (this -> isEmpty())
    {
        this -> root = ph -> root;
        return;
    }
    
    else
    {
        if (this -> root -> key < ph -> root -> key)
        {
            PNode *fC = this -> root -> c;
            this -> root -> c = ph -> root;
            ph -> root -> b = this -> root;
            ph -> root -> f = fC;
            if (fC != NULL) fC -> b = ph -> root;
        }
        else
        {
            PNode *fC = ph -> root -> c;
            ph -> root -> c = this -> root;
            this -> root -> b = ph -> root;
            this -> root -> f = fC;
            if (fC != NULL) fC -> b = this -> root;
            this -> root = ph -> root;
        }
    }
}

PNode* PHeap::make()
{
    return this -> root;
}

PNode* PHeap::extractMin()
{
    //To delete this element, delete the root node. 
    //If the root had two or more subtrees, these must be merged together into a single tree.
    PNode *ret = this -> make();
    
    PNode *oldList = ret -> c;
    
    if (oldList == NULL)
    {
        this -> root = NULL;
        return ret;
    }
    
    queue<PHeap*> heapFIFO;
    stack<PHeap*> heapLIFO;
    
    while (oldList != NULL)
    {
        PNode *next = oldList -> f;
        heapFIFO.push(new PHeap(oldList));
        oldList = next;
    }
    
    while (!heapFIFO.empty())
    {
        PHeap *t1 = heapFIFO.front();
        heapFIFO.pop();
        if (!heapFIFO.empty())
        {
            PHeap *t2 = heapFIFO.front();
            heapFIFO.pop();
            t1 -> merge(t2);
        }
        heapLIFO.push(t1);
    }
    
    while (heapLIFO.size() > 1)
    {
        PHeap *x = heapLIFO.top();
        heapLIFO.pop();
        PHeap *y = heapLIFO.top();
        heapLIFO.pop();
        x -> merge(y);
        heapLIFO.push(x);
    }
    
    this -> root = heapLIFO.top() -> root;
    
    return ret;
}

void PHeap::decreaseKey(PNode *x, int newKey)
{
    x -> key = newKey;
    
    if (this -> root == x) return;
    
    if (x -> b -> c == x)
    {
        x -> b -> c = x -> f;
        if (x -> f != NULL) x -> f -> b = x -> b;
    }
    else
    {
        x -> b -> f = x -> f;
        if (x -> f != NULL) x -> f -> b = x -> b;
    }
    
    this -> merge(new PHeap(x));
}

void PHeap::Delete(PNode *x)
{
    if (this -> root == x) extractMin();
    else
    {
        if (x -> b -> c == x)
        {
            x -> b -> c = x -> f;
            if (x -> f != NULL) x -> f -> b = x -> b;
        }
        else
        {
            x -> b -> f = x -> f;
            if (x -> f != NULL) x -> f -> b = x -> b;
        }
        
        PHeap *R = new PHeap(x);
        R -> extractMin();
        this -> merge(R);
    }
}

int main(int argc, char *argv[])
{
    PHeap *ph = new PHeap();
    PHeap *ph2 = new PHeap();
    PHeap *ph3 = new PHeap();
    PHeap *ph4 = new PHeap();
    
    int num = atoi(argv[1]);

    struct timespec start, finish;
    struct timespec start2, finish2;
    struct timespec start3, finish3;
    struct timespec start4, finish4;
    double accum; //final time
    double accum2;
    double accum3;
    double accum4;

    //Random insert n numbers
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < num; i++){
       int v1 = rand() % 100;
       ph -> insert(new PNode(v1));
    }
    clock_gettime(CLOCK_REALTIME, &finish);
    accum =(finish.tv_sec - start.tv_sec) + (double)(finish.tv_nsec - start.tv_nsec) / (double)BILLION;
    printf("%.4lf\n",accum);
    


    //Random insert n numbers, then delete them all
    for (int i = 0; i < num; i++){
       int v1 = rand() % 100;
       ph2 -> insert(new PNode(v1));
    }
    clock_gettime(CLOCK_REALTIME, &start2);
    for (int i = 0; i < num; i++){
       ph2 -> extractMin();
    }
    clock_gettime(CLOCK_REALTIME, &finish2);
    accum2 =(finish2.tv_sec - start2.tv_sec) + (double)(finish2.tv_nsec - start2.tv_nsec) / (double)BILLION;
    printf("%.4lf\n",accum2);
    

   //Generate heap, then merge them n times.
    for (int i = 0; i < 1000; i++){
       int v1 = rand() % 100;
       ph4 -> insert(new PNode(v1));
       

    }
       
    clock_gettime(CLOCK_REALTIME, &start4);
    for (int i = 0; i < num; i++){
       ph4 -> merge(new PHeap());
    }
    clock_gettime(CLOCK_REALTIME, &finish4);
    accum4 =(finish4.tv_sec - start4.tv_sec) + (double)(finish4.tv_nsec - start4.tv_nsec) / (double)BILLION;
    printf("%.4lf\n",accum4);
    return 0;
}

