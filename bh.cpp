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
#define MAX_N 100000000
#define BILLION 1000000000L
using namespace std;
typedef long long lld;
typedef unsigned long long llu;

/*
 Code modified from Petar Veličković
 https://github.com/PetarV-/Algorithms/tree/master/Data%20Structures
 Binomial Heap data structure satisfying the heap property, with an efficient merge operation.
 Useful for a priority queue implementation where merges are frequent.
 Complexity:    O(1) for make
                O(log N) for insert, merge, extractMin, decreaseKey and delete
*/

struct BinNode
{
    int key;
    int degree;
    BinNode *f, *p, *c;
    
    BinNode()
    {
        this -> key = 0;
        this -> degree = 0;
        this -> f = this -> p = this -> c = NULL;
    }
    
    BinNode(int key)
    {
        this -> key = key;
        this -> degree = 0;
        this -> f = this -> p = this -> c = NULL;
    }
};

class BinHeap
{
    BinNode *roots;
    BinNode *min;
    void linkTrees(BinNode*, BinNode*);
    BinNode* mergeRoots(BinHeap*, BinHeap*);
    
public:
    BinHeap();
    BinHeap(BinNode*);
    bool isEmpty();
    void insert(BinNode*);
    void merge(BinHeap*);
    BinNode* make();
    BinNode* extractMin();
    void decreaseKey(BinNode*, int);
    void Delete(BinNode*);
};

BinHeap::BinHeap()
{
    this -> roots = NULL;
}

BinHeap::BinHeap(BinNode *x)
{
    this -> roots = x;
}

bool BinHeap::isEmpty()
{
    return (this -> roots == NULL);
}

void BinHeap::insert(BinNode *x)
{
    this -> merge(new BinHeap(x));
}

void BinHeap::linkTrees(BinNode *y, BinNode *z)
{
    // Precondition: y -> key >= z -> key
    y -> p = z;
    y -> f = z -> c;
    z -> c = y;
    z -> degree = z -> degree + 1;
}

BinNode* BinHeap::mergeRoots(BinHeap *x, BinHeap *y)
{
    BinNode *ret = new BinNode();
    BinNode *end = ret;
    
    BinNode *L = x -> roots;
    BinNode *R = y -> roots;
    if (L == NULL) return R;
    if (R == NULL) return L;
    while (L != NULL || R != NULL)
    {
        if (L == NULL)
        {
            end -> f = R;
            end = end -> f;
            R = R -> f;
        }
        else if (R == NULL)
        {
            end -> f = L;
            end = end -> f;
            L = L -> f;
        }
        else
        {
            if (L -> degree < R -> degree)
            {
                end -> f = L;
                end = end -> f;
                L = L -> f;
            }
            else
            {
                end -> f = R;
                end = end -> f;
                R = R -> f;
            }
        }
    }
    return (ret -> f);
}

void BinHeap::merge(BinHeap *bh)
{
    BinHeap *H = new BinHeap();
    H -> roots = mergeRoots(this, bh);
    
    if (H -> roots == NULL)
    {
        this -> roots = NULL;
        this -> min = NULL;
        return;
    }
    
    BinNode *prevX = NULL;
    BinNode *x = H -> roots;
    BinNode *nextX = x -> f;
    while (nextX != NULL)
    {
        if (x -> degree != nextX -> degree || (nextX -> f != NULL && nextX -> f -> degree == x -> degree))
        {
            prevX = x;
            x = nextX;
        }
        else if (x -> key <= nextX -> key)
        {
            x -> f = nextX -> f;
            linkTrees(nextX, x);
        }
        else
        {
            if (prevX == NULL) H -> roots = nextX;
            else prevX -> f = nextX;
            linkTrees(x, nextX);
            x = nextX;
        }
        nextX = x -> f;
    }
    
    this -> roots = H -> roots;
    this -> min = H -> roots;
    BinNode *cur = this -> roots;
    while (cur != NULL)
    {
        if (cur -> key < this -> min -> key) this -> min = cur;
        cur = cur -> f;
    }
}

BinNode* BinHeap::make()
{
    return this -> min;
}

BinNode* BinHeap::extractMin()
{
    BinNode *ret = this -> make();
    
    // delete ret from the list of roots
    BinNode *prevX = NULL;
    BinNode *x = this -> roots;
    while (x != ret)
    {
        prevX = x;
        x = x -> f;
    }
    if (prevX == NULL) this -> roots = x -> f;
    else prevX -> f = x -> f;
    
    // reverse the list of ret's children
    BinNode *revChd = NULL;
    BinNode *cur = ret -> c;
    while (cur != NULL)
    {
        BinNode *next = cur -> f;
        cur -> f = revChd;
        revChd = cur;
        cur = next;
    }
    
    // merge the two lists
    BinHeap *H = new BinHeap();
    H -> roots = revChd;
    this -> merge(H);
    
    return ret;
}

void BinHeap::decreaseKey(BinNode *x, int newKey)
{
    // Precondition: x -> key > newKey
    x -> key = newKey;
    BinNode *y = x;
    BinNode *z = y -> p;
    while (z != NULL && y -> key < z -> key)
    {
        // swap contents
        swap(y -> key, z -> key);
        
        y = z;
        z = y -> p;
    }
    
    if (y -> key < this -> min -> key) this -> min = y;
}

void BinHeap::Delete(BinNode *x)
{
    decreaseKey(x, -INF);
    extractMin();
}

int main(int argc, char *argv[])
{
    BinHeap *bh = new BinHeap();
    BinHeap *bh2 = new BinHeap();
    BinHeap *bh3 = new BinHeap();
    int num = atoi(argv[1]);

    struct timespec start, finish;
    struct timespec start2, finish2;
    struct timespec start4, finish4;
    double accum; //final time
    double accum2;
    double accum3;
    double accum4;

    //Random insert n numbers
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < num; i++){
       int v1 = rand() % 100;
       bh -> insert(new BinNode(v1));
    }
    clock_gettime(CLOCK_REALTIME, &finish);
    accum =(finish.tv_sec - start.tv_sec) + (double)(finish.tv_nsec - start.tv_nsec) / (double)BILLION;
    printf("%.4lf\n",accum);
    


    //Random insert n numbers, then delete them all
    for (int i = 0; i < num; i++){
       int v1 = rand() % 100;
       bh2 -> insert(new BinNode(v1));
    }
    clock_gettime(CLOCK_REALTIME, &start2);
    for (int i = 0; i < num; i++){
       bh2 -> extractMin();
    }
    clock_gettime(CLOCK_REALTIME, &finish2);
    accum2 =(finish2.tv_sec - start2.tv_sec) + (double)(finish2.tv_nsec - start2.tv_nsec) / (double)BILLION;
    printf("%.4lf\n",accum2);
 

   //Generate heap, then merge them n times.
    for (int i = 0; i < 100000; i++){
       int v1 = rand() % 100;
       bh3 -> insert(new BinNode(v1));

    }
       
    clock_gettime(CLOCK_REALTIME, &start4);
    for (int i = 0; i < num; i++){
       bh3 -> merge(new BinHeap());
    }
    clock_gettime(CLOCK_REALTIME, &finish4);
    accum4 =(finish4.tv_sec - start4.tv_sec) + (double)(finish4.tv_nsec - start4.tv_nsec) / (double)BILLION;
    printf("%.4lf\n",accum4);
    return 0;
}