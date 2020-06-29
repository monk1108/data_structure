//多服务台排队系统，用优先级队列实现，优先级是事件发生的时间
#include <iostream>
#include <cstdlib>
#include <time.h>
#include "E:\大一下\数据结构\template\priorityQueue.cpp"
#include "E:\大一下\数据结构\template\listQueue.cpp"

using namespace std;

class simulator{
    int numofServer;//服务台数
    int arrivalLow;
    int arrivalHigh;//到达间隔时间的上下限
    int servicetimeLow;
    int servicetimeHigh;//服务间隔时间的上下限
    int customNum;//顾客数

    struct eventT
    {
        int time;//事件发生时间
        int type;//事件类型，0为到达，1为离开
        bool operator<(const eventT &e)const{
            return time < e.time;
        }
    };

public:
    simulator();
    int avgWaitTime();
};

simulator::simulator(){
    cout<<"请输入柜台数：";
    cin>>numofServer;
    cout<<"\n输入最小间隔时间和最大间隔时间：";
    cin>>arrivalLow>>arrivalHigh;
    cout<<"\n输入服务时间下限和服务时间上限：";
    cin>>servicetimeLow>>servicetimeHigh;
    cout<<"\n输入模拟的顾客数：";
    cin>>customNum;

    srand(time(NULL));//随机数发生器初始化
}

int simulator::avgWaitTime(){
    int serverBusy = 0; //工作的服务台数
    int currentTime;//模拟过程中的时间
    int totalWaitTime = 0;//顾客等待时间的总和
    listQueue<eventT> waitQueue;//等待队列
    priorityQueue<eventT> eventQueue;//事件队列

    eventT currentEvent;

    //生成初始的事件队列
    currentEvent.time = 0;
    currentEvent.type = 0;//到达事件
    for(int i = 0; i < customNum; i++){
        currentEvent.time += arrivalLow + (arrivalHigh - arrivalLow + 1) * rand() / (RAND_MAX+1);
        eventQueue.enQueue(currentEvent);
    }

    //模拟过程
    while(!eventQueue.isempty()){
        currentEvent = eventQueue.deQueue();
        currentTime = currentEvent.time;
        switch (currentEvent.type)
        {
            case 0:
                //到达事件
                if(serverBusy != numofServer){
                    //服务台有空
                    ++serverBusy;
                    currentEvent.time += servicetimeLow +
                        (servicetimeHigh - servicetimeLow + 1) * rand() / (RAND_MAX + 1);
                    currentEvent.type = 1;//改为离开事件
                    eventQueue.enQueue(currentEvent);
                }
                else{
                    waitQueue.enQueue(currentEvent);
                }
                break;

            case 1:
                //离开事件
                if(!waitQueue.isempty()){
                    currentEvent = waitQueue.deQueue();
                    totalWaitTime += currentTime - currentEvent.time;
                    currentEvent.time = currentTime + servicetimeLow +
                        (servicetimeHigh - servicetimeLow + 1) * rand() / (RAND_MAX + 1);
                    currentEvent.type = 1;
                    eventQueue.enQueue(currentEvent);
                }
                else{
                    --serverBusy;
                }
        }
    }

    return totalWaitTime / customNum;
}

int main(){
    simulator sim;
    cout<<"\n平均等待时间："<<sim.avgWaitTime();
    return 0;
}
