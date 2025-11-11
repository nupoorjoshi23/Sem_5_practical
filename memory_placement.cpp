#include <bits/stdc++.h>
using namespace std;

class MemoryAllocator{
private:
    vector<int>blockSize; //size of each memory block
    vector<int>processSize; //memory size required by each process
    int nb; //number of blocks
    int np; //number of processes
    // These store the results of a single simulation
    vector<int>allocation;
    vector<int>tempBlockSize; 

    void reset(){ //copies original block size to temp list and clears allocation result
        tempBlockSize=blockSize;
        allocation.assign(np,-1);
    }

    void printAllocation(const string& strategyName){
        cout << "-------" << strategyName << "-------" << endl;
        for(int i=0;i<np;i++){
            if(allocation[i]!=-1){
                cout << "Process " << i+1 << " -> Block " << allocation[i]+1 << endl;
            }
            else cout << "Process " << i+1 << " -> Not Allocated" << endl;
        }
    }

    void firstFit(){
        reset();
        for(int i=0;i<np;i++){
            for(int j=0;j<nb;j++){  //for every process loop through the blocks
                if(tempBlockSize[j]>=processSize[i]){
                    allocation[i]=j; //allocate that block to the process 
                    tempBlockSize[j]-=processSize[i];
                    break;
                }
            }
        }
        printAllocation("FIRST FIT");
    }
    void bestFit(){
        reset();
        for(int i=0;i<np;i++){
            int best=-1;
            for(int j=0;j<nb;j++){
                if(tempBlockSize[j]>=processSize[i]){
                    if(best==-1 || tempBlockSize[j]<tempBlockSize[best]) best=j;
                }
            }
            if(best!=-1){
                allocation[i]=best;
                tempBlockSize[best]-=processSize[i];
            }
        }
        printAllocation("BEST FIT");
    }
    void worstFit(){
        reset();
        for(int i=0;i<np;i++){
            int worst=-1;
            for(int j=0;j<nb;j++){
                if(tempBlockSize[j]>=processSize[i]){
                    if(worst==-1 || tempBlockSize[j]>tempBlockSize[worst]) worst=j;
                }
            }
            if(worst!=-1){
                allocation[i]=worst;
                tempBlockSize[worst]-=processSize[i];
            }
        }
        printAllocation("WORST FIT");
    }
    void nextFit(){
        reset();
        int pos=0; //the "memory" pointer for next fit
        for(int i=0;i<np;i++){
            bool allocated=false;
            for(int j=pos;j<nb;j++){  //search from the last position
                if(tempBlockSize[j]>=processSize[i]){
                    allocation[i]=j;
                    tempBlockSize[j]-=processSize[i];
                    allocated=true;
                    break;
                }
            }
            if(!allocated){ //if not found reset the pos to 0 to check from start
                pos=0;
            }
        }
        printAllocation("NEXT FIT");
    }
public:
    MemoryAllocator() : nb(0), np(0) {} //constructor

    void readInput(){
        cout << "Enter the number of memory blocks: ";
        cin >> nb;
        cout << "Enter size of each block: ";
        blockSize.resize(nb);
        for(int i=0;i<nb;i++){
            cin >> blockSize[i];
        }
        cout << "Enter the number of processes: ";
        cin >> np;
        cout << "Enter memory required by each process: ";
        processSize.resize(np);
        for(int i=0;i<np;i++){
            cin >> processSize[i];
        }
    }
    void runAllStrategies(){
            firstFit();
            bestFit();
            worstFit();
            nextFit();
    }
};

int main(){
    MemoryAllocator allocator;
    allocator.readInput();
    allocator.runAllStrategies();
    return 0;
}

/* 
Enter the number of memory blocks: 5
Enter size of each block: 100 500 200 300 600
Enter the number of processes: 4
Enter memory required by each process: 212 417 112 426
-------FIRST FIT-------
Process 1 -> Block 2
Process 2 -> Block 5
Process 3 -> Block 2
Process 4 -> Not Allocated
-------BEST FIT-------
Process 1 -> Block 4
Process 2 -> Block 2
Process 3 -> Block 3
Process 4 -> Block 5
-------WORST FIT-------
Process 1 -> Block 5
Process 2 -> Block 2
Process 3 -> Block 5
Process 4 -> Not Allocated
-------NEXT FIT-------
Process 1 -> Block 2
Process 2 -> Block 5
Process 3 -> Block 2
Process 4 -> Not Allocated
*/