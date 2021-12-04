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
 Fibonacci Heap data structure satisfying the heap property, with highly efficient asymptotic bounds.
 Useful for a priority queue implementation in Dijkstra's and Prim's algorithms.
 Complexity:    O(1) for insert, make and merge
                O(1) amortized for decreaseKey
                O(log N) amortized for extractMin and delete
*/

struct FibNode
{
    int key;
    bool marked;
    int degree;
    FibNode *b, *f, *p, *c;
    
    FibNode()
    {
        this -> key = 0;
        this -> marked = false;
        this -> degree = 0;
        this -> b = this -> f = this -> p = this -> c = NULL;
    }
    
    FibNode(int key)
    {
        this -> key = key;
        this -> marked = false;
        this -> degree = 0;
        this -> b = this -> f = this -> p = this -> c = NULL;
    }
};

class FibHeap
{
    FibNode *min;
    int N;
    
public:
    FibHeap();
    FibHeap(FibNode*);
    bool isEmpty();
    void insert(FibNode*);
    void merge(FibHeap*);
    FibNode* make();
    FibNode* extractMin();
    void decreaseKey(FibNode*, int);
    void Delete(FibNode*);
};

FibHeap::FibHeap()
{
    this -> min = NULL;
    this -> N = 0;
}

FibHeap::FibHeap(FibNode *n)
{
    this -> min = n;
    n -> b = n -> f = n;
    n -> p = n -> c = NULL;
    
    this -> N = 1;
}

bool FibHeap::isEmpty()
{
    return (this -> min == NULL);
}
//insert  method
void FibHeap::insert(FibNode *n)
{
    this -> merge(new FibHeap(n));
}
//merge method
void FibHeap::merge(FibHeap *h)
{
    this -> N += h -> N;
    if (h -> isEmpty()) return;
    if (this -> isEmpty())
    {
        this -> min = h -> min;
        return;
    }
    FibNode *make1 = this -> min;
    FibNode *last1 = this -> min -> b;
    FibNode *make2 = h -> min;
    FibNode *last2 = h -> min -> b;
    make1 -> b = last2;
    last1 -> f = make2;
    make2 -> b = last1;
    last2 -> f = make1;
    if (h -> min -> key < this -> min -> key) this -> min = h -> min;
}

FibNode* FibHeap::make()
{
    return this -> min;
}
//extractMin method
FibNode* FibHeap::extractMin()
{
    FibNode *ret = this -> min;
    this -> N = this -> N - 1;
    
    if (ret -> f == ret)
    {
        this -> min = NULL;
    }
    else
    {
        FibNode *prev = ret -> b;
        FibNode *next = ret -> f;
        prev -> f = next;
        next -> b = prev;
        this -> min = next; // Not necessarily a minimum. This is for assisting with the merge w/ min's children.
    }
    
    if (ret -> c != NULL)
    {
        FibNode *makeChd = ret -> c;
        FibNode *currChd = makeChd;
        
        do
        {
            currChd -> p = NULL;
            currChd = currChd -> f;
        } while (currChd != makeChd);
        
        if (this -> isEmpty())
        {
            this -> min = makeChd;
        }
        else
        {
            FibNode *make1 = this -> min;
            FibNode *last1 = this -> min -> b;
            FibNode *make2 = makeChd;
            FibNode *last2 = makeChd -> b;
            make1 -> b = last2;
            last1 -> f = make2;
            make2 -> b = last1;
            last2 -> f = make1;
        }
    }
    
    if (this -> min != NULL)
    {
        int maxAuxSize = 5 * (((int)log2(this -> N + 1)) + 1);
        FibNode *aux[maxAuxSize + 1];
        for (int i=0;i<=maxAuxSize;i++) aux[i] = NULL;
        int maxDegree = 0;
        
        FibNode *curr = this -> min;
        
        do
        {
            FibNode *next = curr -> f;
            int deg = curr -> degree;
            FibNode *P = curr;
            while (aux[deg] != NULL)
            {
                FibNode *Q = aux[deg];
                aux[deg] = NULL;
                
                if (P -> key > Q -> key)
                {
                    FibNode *tmp = P;
                    P = Q;
                    Q = tmp;
                }
                
                Q -> p = P;
                if (P -> c == NULL)
                {
                    P -> c = Q;
                    Q -> b = Q -> f = Q;
                }
                else
                {
                    FibNode *last = P -> c -> b;
                    last -> f = Q;
                    Q -> b = last;
                    P -> c -> b = Q;
                    Q -> f = P -> c;
                }
                
                deg++;
                P -> degree = deg;
            }
            aux[deg] = P;
            if (deg > maxDegree) maxDegree = deg;
            curr = next;
        } while (curr != this -> min);
        
        
        FibNode *previous = aux[maxDegree];
        this -> min = previous;
        for (int i=0;i<=maxDegree;i++)
        {
            if (aux[i] != NULL)
            {
                previous -> f = aux[i];
                aux[i] -> b = previous;
                if (aux[i] -> key < this -> min -> key) this -> min = aux[i];
                previous = aux[i];
            }
        }
        
    }
    
    return ret;
}

void FibHeap::decreaseKey(FibNode *n, int newKey)
{
    // Precondition: newKey < n -> key
    n -> key = newKey;
    
    FibNode *curr = n;
    if (curr -> p != NULL)
    {
        if (curr -> key < curr -> p -> key)
        {
            FibNode *parent = curr -> p;
            curr -> marked = false;
            
            curr -> p = NULL;
            if (curr -> f == curr) parent -> c = NULL;
            else
            {
                FibNode *prev = curr -> b;
                FibNode *next = curr -> f;
                prev -> f = next;
                next -> b = prev;
                if (parent -> c == curr) parent -> c = prev;
            }
            parent -> degree = parent -> degree - 1;
            
            FibNode *last = this -> min -> b;
            last -> f = curr;
            curr -> b = last;
            this -> min -> b = curr;
            curr -> f = this -> min;
            
            if (curr -> key < this -> min -> key) this -> min = curr;
            
            while (parent -> p != NULL && parent -> marked)
            {
                curr = parent;
                parent = curr -> p;
                curr -> marked = false;
                
                curr -> p = NULL;
                if (curr -> f == curr) parent -> c = NULL;
                else
                {
                    FibNode *prev = curr -> b;
                    FibNode *next = curr -> f;
                    prev -> f = next;
                    next -> b = prev;
                    if (parent -> c == curr) parent -> c = prev;
                }
                parent -> degree = parent -> degree - 1;
                
                FibNode *last = this -> min -> b;
                last -> f = curr;
                curr -> b = last;
                this -> min -> b = curr;
                curr -> f = this -> min;
                
            }
            if (parent -> p != NULL) parent -> marked = true;
        }
    }
    else if (n -> key < this -> min -> key) this -> min = n;
}

void FibHeap::Delete(FibNode *n)
{
    this -> decreaseKey(n, -INF);
    this -> extractMin();
}

int main(int argc, char *argv[])
{
    FibHeap *bh = new FibHeap();
    FibHeap *bh2 = new FibHeap();
    FibHeap *bh4 = new FibHeap();
    
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
       bh -> insert(new FibNode(v1));
    }
    clock_gettime(CLOCK_REALTIME, &finish);
    accum =(finish.tv_sec - start.tv_sec) + (double)(finish.tv_nsec - start.tv_nsec) / (double)BILLION;
    printf("%.4lf\n",accum);
    


    //Random insert n numbers, then delete them all
    for (int i = 0; i < num; i++){
       int v1 = rand() % 100;
       bh2 -> insert(new FibNode(v1));
    }
    clock_gettime(CLOCK_REALTIME, &start2);
    for (int i = 0; i < num; i++){
       bh2 -> extractMin();
    }
    clock_gettime(CLOCK_REALTIME, &finish2);
    accum2 =(finish2.tv_sec - start2.tv_sec) + (double)(finish2.tv_nsec - start2.tv_nsec) / (double)BILLION;
    printf("%.4lf\n",accum2);
 

   //Generate heap, then merge them n times.
    for (int i = 0; i < 1000; i++){
       int v1 = rand() % 100;
       bh4 -> insert(new FibNode(v1));
       

    }
       
    clock_gettime(CLOCK_REALTIME, &start4);
    for (int i = 0; i < num; i++){
       bh4 -> merge(new FibHeap());
    }
    clock_gettime(CLOCK_REALTIME, &finish4);
    accum4 =(finish4.tv_sec - start4.tv_sec) + (double)(finish4.tv_nsec - start4.tv_nsec) / (double)BILLION;
    printf("%.4lf\n",accum4);
    return 0;


}

