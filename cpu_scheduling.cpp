#include <bits/stdc++.h>
using namespace std;

struct Process{
    int pid;
    int AT;
    int BT;
    int CT;
    int TAT; //CT-AT
    int WT;  //TAT-BT
    int RT;  //for preemptive
};

void resetProcess(vector<Process>& processes, const vector<Process>& original){
    for(int i=0;i<processes.size();i++){
        processes[i]=original[i];
        processes[i].CT=0;
        processes[i].TAT=0;
        processes[i].WT=0;
        processes[i].RT=processes[i].BT;
    }
}

void display(const string& name, const vector<Process>& processes){
    cout << "\n=== " << name << " ===\n";
    cout << "PID\tAT\tBT\tCT\tTAT\tWT\n";
    for(auto p:processes){
        cout << p.pid << "\t" << p.AT << "\t" << p.BT << "\t" << p.CT << "\t"
            << p.TAT << "\t" << p.WT << "\n";
    }
}

pair<float,float> calculateAvg(const vector<Process>& processes){
    int n=processes.size();
    float totalTAT=0, totalWT=0;
    for(auto p:processes){
        totalTAT+=p.TAT;
        totalWT+=p.WT;
    }
    return {totalTAT/n, totalWT/n};
}

void FCFS(vector<Process>& processes){
    sort(processes.begin(),processes.end(), [](auto a, auto b){
        return (a.AT==b.AT)? (a.pid<b.pid):(a.AT<b.AT);
    });  //if arrival time is same sort according to pid
    int time=0;
    for(auto &p:processes){
        if(time<p.AT){ //CPU is idle
            time=p.AT;
        }
        time+=p.BT;
        p.CT=time;
        p.TAT=p.CT-p.AT;
        p.WT=p.TAT-p.BT;
    }
}

void SJF_preemptive(vector<Process>& processes){
    int n=processes.size(), completed=0, time=0;
    while(completed<n){
        int shortest=-1;
        for(int i=0;i<n;i++){
            if(processes[i].AT<=time && processes[i].RT>0){
                if(shortest==-1 || processes[i].RT<processes[shortest].RT ||
                 (processes[i].RT==processes[i].RT && processes[i].pid<processes[shortest].pid)){
                     shortest=i;
                 }
            }
        }
        if(shortest==-1){
            time++;
            continue;
        }
        processes[shortest].RT--;
        time++;
        if(processes[shortest].RT==0){
            completed++;
            processes[shortest].CT=time;
            processes[shortest].TAT=processes[shortest].CT-processes[shortest].AT;
            processes[shortest].WT=processes[shortest].TAT-processes[shortest].BT;
        }
    }
}

void RoundRobin(vector<Process>& processes, int quantum){
    queue<int>q;
    int n=processes.size(), completed=0, time=0;
    vector<bool>inQueue(n,false);
    while(completed<n){
        for(int i=0;i<n;i++){ //enque arrived processes
            if(processes[i].AT<=time && !inQueue[i] && processes[i].RT>0){
                q.push(i);
                inQueue[i]=true;
            }
        }
        if(q.empty()){
            time++;
            continue;
        }
        int idx=q.front(); q.pop();
        inQueue[idx]=false;
        int run=min(quantum, processes[idx].RT);
        processes[idx].RT-=run;
        time+=run;
        
        for(int i=0;i<n;i++){ //add newly arrived process during this time
            if(processes[i].AT<=time && !inQueue[i] && processes[i].RT>0){
                q.push(i);
                inQueue[i]=true;
            }
        }
        
        if(processes[idx].RT>0){ //unfinished process goes back to the queue
            q.push(idx);
            inQueue[idx]=true;
        }
        else{
            completed++;
            processes[idx].CT=time;
            processes[idx].TAT=processes[idx].CT-processes[idx].AT;
            processes[idx].WT=processes[idx].TAT-processes[idx].BT;
        }
    }
}

void Priority_Preemptive(vector<Process>& processes,vector<int>priorities){
    int n=processes.size(), time=0, completed=0;
    while(completed<n){
        int idx=-1, best_priority=INT_MAX;
        for(int i=0;i<n;i++){
            if(processes[i].AT<=time && processes[i].RT>0){
                if(priorities[i]<best_priority || (priorities[i]==best_priority
                && (idx==-1 || processes[i].pid<processes[idx].pid))){
                    bestPriority=priorities[i];
                    idx=i;
                }
            }
        }
        if(idx==-1){
            time++;
            continue;
        }
        processes[idx].RT--;
        time++;
        
        if(processes[idx].RT==0){
            completed++;
            processes[idx].CT=time;
            processes[idx].TAT=processes[idx].CT-processes[idx].AT;
            processes[idx].WT=processes[idx].TAT-processes[idx].BT;
        }
    }
}

int main(){
    int n, quantum;
    cout << "Enter the number of processes: ";
    cin >> n;
    
    vector<Process> original(n);
    for(int i=0;i<n;i++){
        original[i].pid=i+1;
        cout << "Enter arrival time and burst time for process " << i+1 << ": ";
        cin >> original[i].AT >> original[i].BT;
        original[i].RT=original[i].BT;
    }
    
    cout << "Enter time quantum for Round Robin: ";
    cin >> quantum;
    
    vector<int>priorities(n);
    for(int i=0;i<n;i++){
        cout << "Enter priority for process " << i+1 << " (lower=higher priority): ";
        cin >> priorities[i];
    }
    vector<Process>processes=original;
    pair<float,float> avgFCFS, avgSJF, avgRR, avgPriority;
    
    FCFS(processes);
    display("FCFS (Non-Preemptive)", processes);
    avgFCFS=calculateAvg(processes);
    
    resetProcess(processes,original);
    SJF_preemptive(processes);
    display("SJF (Preemptive)", processes);
    avgSJF=calculateAvg(processes);
    
    resetProcess(processes,original);
    RoundRobin(processes,quantum);
    display("Round Robin (Preemptive)", processes);
    avgRR=calculateAvg(processes);
    
    resetProcess(processes,original);
    Priority_Preemptive(processes,priorities);
    display("Priority (Preemptive)", processes);
    avgPriority=calculateAvg(processes);
    
    cout << "\n================== AVERAGE TIMES SUMMARY ==================\n";
    cout << "Algorithm\t\tAvg TAT\t\tAvg WT\n";
    cout << "-----------------------------------------------------------\n";
    cout << "FCFS\t\t\t" << avgFCFS.first << "\t\t" << avgFCFS.second << "\n";
    cout << "SJF (Preemptive)\t" << avgSJF.first << "\t\t" << avgSJF.second << "\n";
    cout << "Round Robin\t\t" << avgRR.first << "\t\t" << avgRR.second << "\n";
    cout << "Priority (Preemptive)\t" << avgPriority.first << "\t\t" << avgPriority.second << "\n";

    return 0;
}
