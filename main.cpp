#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

class Node{
private:
    unsigned int acc_num;
    int bal;
    Node* next;

    Node(int num){
        acc_num = num;
        bal = 1000;
        next = nullptr;
    }
friend class SLL;
friend class DLL;
};

class SLL{
private:
    Node* first;
    Node* last;
    int len;
public:
    SLL(){
        first = nullptr;
        last = nullptr;
        len = 0;
    }

    void addNode(int num){
        Node* temp = new Node(num);
        len++;
        if(first == nullptr){
            first = temp;
            last = temp;
            return;
        }
        else{
            last->next = temp;
            last = last->next;
            return;
        }
    }

    void balCount(int X){//count of accounts whose balance > X
        if(first == nullptr){
            return;
        }
        int count = 0;
        Node* temp = first;
        while(temp != nullptr){
            if(temp->bal >= X)
                count++;
            temp = temp->next;
        }
        cout<<count<<endl;
        return;
    }

    void printBal(int X){// balance of account number X
        if(first == nullptr){
            return;
        }
        Node* temp = first;
        while(temp != nullptr){
            if(temp->acc_num == X){
                cout<<temp->bal<<endl;
                return;
            }
            temp = temp->next;
        }
    }

    void highestBal(){//bank accounts of highest balance in ascending order
        if(first == nullptr){
            return;
        }
        Node* temp = first;
        int max = temp->bal;
        while(temp != nullptr){
            if(temp->bal > max){
                max = temp->bal;
            }
            temp = temp->next;
        }
        vector<int> v;
        temp = first;
        while(temp != nullptr){
            if(temp->bal == max){
                v.push_back(temp->acc_num);
            }
            temp = temp->next;
        }
        sort(v.begin(), v.end());
        for(int i = 0; i < v.size(); i++)
            cout<<v[i]<<" ";
        cout<<endl;
    }

    int isThere(int X){//returns 1 if account number is present
        Node* temp = first;
        while(temp != nullptr){
            if(temp->acc_num == X)
                return 1;
                temp = temp->next;
        }
        return 0;
    }

    void transaction(int acc, int change){
        Node* temp = first;
        while(temp != nullptr){
            if(temp->acc_num == acc){
                temp->bal += change;
            }
            temp = temp->next;
        }
    }
    void rev_transaction(int acc, int change){
        Node* temp = first;
        while(temp != nullptr){
            if(temp->acc_num == acc){
                temp->bal -= change;
            }
            temp = temp->next;
        }
    }
friend class DLL;
};

class Tran{
private:
    int account_no;
    int change;//+ve if deposit and -ve if withdrawal
    Tran* next;
    Tran* prev;

    Tran(int n, char ch, int diff){
        account_no = n;
        if(ch == 'D')
            change = diff;
        else
            change = -diff;
    }
friend class DLL;
};

class DLL{
private:
    Tran* head;
    Tran* tail;
    Tran* cur;
public:
    DLL(){
        head = new Tran(-1, 'D', 0);
        tail = new Tran(-1, 'D', 0);
        head->next = tail;
        head->prev = nullptr;
        tail->prev = head;
        tail->next = nullptr;
        cur = head;
    }
    void addTran(int n, char ch, int diff){//adds a valid transaction
        Tran* temp = new Tran(n, ch, diff);
        Tran* pre = tail->prev;
        pre->next = temp;
        temp->prev = pre;
        temp->next = tail;
        tail->prev = temp;
    }
    void insertTran(int n, char ch, int diff, int k, SLL list){
        Tran* new_tran = new Tran(n, ch, diff);
        Tran* temp = head;
        for(int i = 1; i <= k; i++){
            if(temp == tail){
                return;
            }
            temp = temp->next;
        }
        Tran* post = temp->next;//new_tran is to be between temp and post
        new_tran->next = post;
        post->prev = new_tran;
        temp->next = new_tran;
        new_tran->prev = temp;
        temp = temp->next;
        while(temp != tail){
            if(temp == cur){
                list.transaction(new_tran->account_no, new_tran->change);
            }
            temp = temp->next;
        }
    }
    void trans(SLL list){
        if(cur != tail->prev){
            cur = cur->next;
            list.transaction(cur->account_no, cur->change);
        }
    }
    void transAll(SLL list){
        while(cur != tail->prev){
            cur = cur->next;
            list.transaction(cur->account_no, cur->change);
        }
    }
    void rev_trans(SLL list){
        if(cur != head){
            list.rev_transaction(cur->account_no, cur->change);
            cur = cur->prev;
        }
    }
    void printY(int Y){
        Tran* temp = head;
        while(temp != cur){
            temp = temp->next;
            if(temp->account_no == Y){
                cout<<Y<<" ";
                if(temp->change >= 0)
                    cout<<"D "<<temp->change<<endl;
                else
                    cout<<"W "<<(-temp->change)<<endl;
            }
        }
    }
    void DAM(int A, int M, SLL list){
        if(M > 0){
            Tran* temp = cur->next;
            while(temp != tail && M != 0){
                if(temp->account_no == A){
                    M--;
                    Tran* bef = temp->prev;
                    Tran* af = temp->next;
                    bef->next = af;
                    af->prev = bef;
                    delete temp;
                    temp = bef;
                }
                temp = temp->next;
            }
        }
        else{
            Tran* temp = cur->prev;
            while(temp !=  head && M != 0){
                if(temp->account_no == A){
                    M++;
                    Tran* bef = temp->prev;
                    Tran* af = temp->next;
                    bef->next = af;
                    af->prev = bef;
                    list.rev_transaction(temp->account_no, temp->change);
                    delete temp;
                    temp = af;
                }
                temp = temp->prev;
            }
        }
    }
};

int main() {
    int C, num, amt, a, b, m;
    string ch; char c;

    cin>>C;
    SLL list;
    for(int i = 0; i < C; i++){
        cin>>num;
        list.addNode(num);
    }

    DLL tlist;
    int N;
    cin>>N;
    for(int i = 0; i < N; i++){
        cin>>num>>ch>>amt;
        if(list.isThere(num) == 1){
            tlist.addTran(num, ch[0], amt);
        }
    }

    while(true){
        cin>>ch;
        if(ch[0] == 'E'){ return 0;}
        if(ch[0] == 'F'){
            cin>>a;
            for(int i = 0; i < a; i++){
                tlist.trans(list);
            }
        }
        if(ch[0] == 'R'){
            cin>>a;
            for(int i = 0; i < a; i++){
                tlist.rev_trans(list);
            }
        }
        if(ch[0] == 'I'){
            cin>>num>>c>>amt;
            cin>>a;
            if(list.isThere(num) == 1){
                tlist.insertTran(num, c, amt, a, list);
            }
        }
        if(ch[0] == 'D'){
            cin>>a>>m;
            tlist.DAM(a, m, list);
        }
        if(ch[0] == 'C'){
            tlist.transAll(list);
        }
        if(ch[0] == 'S'){
            cin>>a;
            tlist.printY(a);
        }
        if(ch[0] == 'G'){
            // cout<<ch;
            cin>>a;
            list.balCount(a);
        }
        if(ch[0] == 'M'){
            list.highestBal();
        }
        if(ch[0] == 'V'){
            cin>>a;
            list.printBal(a);
        }
    }

    return 0;
}
