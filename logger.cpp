#include<iostream>
#include<bits/stdc++.h>
using namespace std;

enum MessageType
{
    ERROR,
    WARNING,
    GENERAL,
};
class Subject;
class Observer
{
    public:
    virtual void  update(Subject * ptrSubject)=0;
    virtual ~Observer(){};
};

class Subject
{
    map<int,vector<Observer *>> registrymap;
    string data;
    public:
        string getData(){return data;}
        void setData(string data){this->data=data;}
        void attach(int event,Observer * pObserver);
        void deattach(Observer * pObserver);
        void notify(int event);
};

void Subject::attach(int event, Observer * pObserver)
{
    registrymap[event].push_back(pObserver);
}
void Subject::deattach(Observer * pObserver){
    for( auto p=registrymap.begin();p!=registrymap.end();p++){
        auto it=find(p->second.begin(),p->second.end(),pObserver);
        if(it!=p->second.end())
            p->second.erase(it);
    }
      
    
}

void Subject::notify(int event)
{
    auto pair= registrymap.find(event);
    if(pair!=registrymap.end()){
        for( auto obs_ptr:pair->second){
            obs_ptr->update(this);
        }
    }
}

class Logger:public Subject
{
    map<MessageType, bool> messageStatusType;
    public:
        Logger()
        {
            messageStatusType[ERROR]=true;
            messageStatusType[WARNING]=true;
            messageStatusType[GENERAL]=true;

        }
        void WriteLog(MessageType type,string message)
        {
            if(messageStatusType[type]){
                setData(message);
                notify(type);
            }
        }
        void enableLoggingOfMessage(MessageType type)
        {
            messageStatusType[type]=true;
        }
        void disableLoggingOfMessage(MessageType type){
            messageStatusType[type]=false;
        }
};



class ConsoleLoggingPlatform: public Observer
{
    public:
    void update(Subject * ptrSubject){
        cout<<ptrSubject->getData()<<endl;
    }
};
class FlatFileLoggingPlatform: public Observer
{
    string fileName;
    public:
    FlatFileLoggingPlatform(string name):fileName(name){}
    void update(Subject *ptrSubject){
        string data=ptrSubject->getData();
        cout<<"added file to "<<fileName<<" with data " <<data<<endl;
    }
};

class NetworkFileLoggingPlatform:public Observer
{
    public:
    void update(Subject *ptrSubject){
         string data=ptrSubject->getData();
        cout<<"send  vai network  with data " <<data<<endl;
    }

};

int main(){
   Logger* ptrLogger=new Logger();
   ConsoleLoggingPlatform* ptrConsolePlatform= new ConsoleLoggingPlatform();
   FlatFileLoggingPlatform * ptrFilePlatform= new FlatFileLoggingPlatform("file1");
   NetworkFileLoggingPlatform * ptrNetworkPlatform= new  NetworkFileLoggingPlatform();

   ptrLogger->attach(ERROR,ptrConsolePlatform);
   ptrLogger->attach(WARNING,ptrConsolePlatform);
   ptrLogger->attach(GENERAL,ptrConsolePlatform);
   ptrLogger->attach(ERROR,ptrFilePlatform);
   ptrLogger->attach(ERROR,ptrNetworkPlatform);

   ptrLogger->WriteLog(ERROR,"error1");
  
   ptrLogger->deattach(ptrConsolePlatform); 
   ptrLogger->WriteLog(ERROR,"error2");
  
}