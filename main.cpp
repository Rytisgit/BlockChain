#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <ctime>
#include <cstring>
#include "timer.h"
#include "RytisMasterClassOnHashingAlgorithms.h"

thread_local static std::mt19937 rg{
        static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count())};
thread_local static std::uniform_int_distribution<int> pick(0, goodCharsLength-1);
thread_local static std::uniform_int_distribution<int> randommoney(100, 10000);
thread_local static std::uniform_int_distribution<int> person(0, 99);
thread_local static std::uniform_int_distribution<int> amount(1, 100);
std::chrono::high_resolution_clock Timer = std::chrono::high_resolution_clock();
long long static getTime(){ return Timer.now().time_since_epoch().count();}
std::string random_string( size_t length )
{
    auto randchar = []() -> char {return goodChars[pick(rg)];};
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}
class Person{
public:
    Person(int mon, std::string s, std::string d){
        money = mon;
        name = s;
        key = d;
    }
    int money;
    std::string name;
    std::string key;
};

class Transaction{
public:
    Transaction(int mon, std::string s, std::string d){
        amount = mon;
        from = s;
        to = d;
        timestamp = getTime();
        this->hash = HASHING_RYTISMASTERCLASSONHASHINGALGORITHMS_H::hash(std::to_string(mon) + s + d + std::to_string(timestamp));
    }
    int amount;
    long long timestamp;
    std::string from;
    std::string to;
    std::string hash;
};

class Block{
public:
    Block(std::string prevhash, std::string merkel,int diff, int nonce, std::vector<Transaction> b){
        transactions = b;
        this->prevhash = prevhash;
        this->merkel = merkel;
        this->timestamp = getTime();
        this->diff = diff;
        this->nonce = nonce;
    }
    std::string version = "1.0";
    std::string prevhash;
    std::string merkel;
    long long int timestamp;
    std::vector<Transaction> transactions;
    int diff;
    int nonce;
};

std::vector<Block> blockChain;
std::string MerkleRoot(std::vector<Transaction> transactions){
    std::vector<std::string> merkleTree;
    for (Transaction t : transactions)
        merkleTree.push_back(hash(std::to_string(t.amount)+t.from+t.to));

    while (merkleTree.size() > 1){
        if (merkleTree.size()%2 != 0)
            merkleTree.emplace_back("");
        std::vector<std::string> newMerkleTree;
        for (int i = 0; i < merkleTree.size(); i += 2){
            std::string first = merkleTree.at(i);
            std::string second = merkleTree.at(i+1);
            newMerkleTree.push_back(hash(first + second));
        }
        merkleTree = newMerkleTree;
    }
    return merkleTree.at(0);
}
std::vector<Person> people;


std::vector<Transaction> transactions;

std::vector<Transaction> generateTransactions(){
    std::vector<Transaction> transactions;
    for (int i = 0; i < 10000; ++i) {
        auto sender = person(rg);
        auto receiver = person(rg);
        while(sender == receiver) receiver = person(rg);
        auto money = amount(rg);
        if(people.at(sender).money < money){ // check if has sufficient funds
            --i;
            continue;
        }
        auto transaction = new Transaction(money, people.at(sender).name, people.at(receiver).name);
        transactions.push_back(*transaction);
    }
    return transactions;
}

int generateBlock(std::string prevHash, int difficulty, int startIndex , int searchMultiplier, std::vector<Transaction> transactions){
    std::string zeros = "0000000000000000000000";
    auto merkle = MerkleRoot(transactions);
    int nonce = 100000 * startIndex;
    for (; nonce <nonce + 100000 * searchMultiplier; ++nonce) {
        std::ostringstream full;
        full << prevHash << merkle << nonce;
        auto hashed = hash(full.str());
        if(strncmp(hashed.c_str(), zeros.c_str(), difficulty)==0){
            std::cout<<hashed<<std::endl;
            return nonce;
        }

    }
    return -1;
}
std::string hashBlock(Block block){
    std::ostringstream full;
    full << block.prevhash << block.merkel << block.nonce;
    return hash(full.str());
}

std::vector<Transaction> getTransactionsForBlock(std::vector<Transaction> &newtransactions, int size) {
    std::vector<Transaction> transactionsForBlock;
    for (int k = 0; k < size ; ++k) {
        if(strncmp(newtransactions.at(k).hash.c_str(),
                   hash(std::__cxx11::to_string(newtransactions.at(k).amount)
                        + newtransactions.at(k).from
                        + newtransactions.at(k).to
                        + std::__cxx11::to_string(newtransactions.at(k).timestamp)).c_str(), hashSize) != 0) // hashses dont match
        {
            newtransactions.erase(newtransactions.begin()+k);//remove bad input
            --k;//get full amount
            if(newtransactions.size()<100){//if too little left, reduce final output size
                size = newtransactions.size();
                continue;
            }

        }
        transactionsForBlock.push_back(newtransactions.at(k));
    }
    newtransactions.erase(newtransactions.begin() + transactionsForBlock.size());
    return transactionsForBlock;
}


void reinsertUnusedTransactions(std::vector<Transaction> &transactionsForBlockb,
                                std::vector<Transaction> &transactionsForBlockc,
                                std::vector<Transaction> &transactionsForBlockd,
                                std::vector<Transaction> &transactionsForBlocke
) {
    transactions.insert(end(transactions), begin(transactionsForBlockb), begin(transactionsForBlockb));
    transactions.insert(end(transactions), begin(transactionsForBlockc), begin(transactionsForBlockc));
    transactions.insert(end(transactions), begin(transactionsForBlockd), begin(transactionsForBlockd));
    transactions.insert(end(transactions), begin(transactionsForBlocke), begin(transactionsForBlocke));
}

int getNewBlockNonce(std::vector<Transaction> &newtransactions, int size, int difficulty, int startIndex,
                     int searchMultiplier) {
    auto transactionsForBlocka = getTransactionsForBlock(newtransactions, size);
    auto transactionsForBlockb = getTransactionsForBlock(newtransactions, size);
    auto transactionsForBlockc = getTransactionsForBlock(newtransactions, size);
    auto transactionsForBlockd = getTransactionsForBlock(newtransactions, size);
    auto transactionsForBlocke = getTransactionsForBlock(newtransactions, size);
    int nonce =-1;
    while(nonce == -1){
            nonce = generateBlock(hashBlock(blockChain.back()),difficulty,startIndex,searchMultiplier,transactionsForBlocka);
            if(nonce != -1) {reinsertUnusedTransactions(transactionsForBlockb, transactionsForBlockc, transactionsForBlockd, transactionsForBlocke);continue;}

            nonce = generateBlock(hashBlock(blockChain.back()),difficulty,startIndex,searchMultiplier,transactionsForBlockb);
            if(nonce != -1) {reinsertUnusedTransactions(transactionsForBlocka, transactionsForBlockc, transactionsForBlockd, transactionsForBlocke); continue;}

            nonce = generateBlock(hashBlock(blockChain.back()),difficulty,startIndex,searchMultiplier,transactionsForBlockc);
            if(nonce != -1) {reinsertUnusedTransactions(transactionsForBlockb, transactionsForBlocka, transactionsForBlockd, transactionsForBlocke); continue;}

            nonce = generateBlock(hashBlock(blockChain.back()),difficulty,startIndex,searchMultiplier,transactionsForBlockd);
            if(nonce != -1) {reinsertUnusedTransactions(transactionsForBlockb, transactionsForBlockc, transactionsForBlocka, transactionsForBlocke); continue;}

            nonce = generateBlock(hashBlock(blockChain.back()),difficulty,startIndex,searchMultiplier,transactionsForBlocke);
            if(nonce != -1) {reinsertUnusedTransactions(transactionsForBlockb, transactionsForBlockc, transactionsForBlockd, transactionsForBlocka); continue;}
            searchMultiplier++;
        }
    searchMultiplier= 1;//prabandyti 5 tranakciju setai, padidinam paieska

    return nonce;
}

int main() {
    for (int i = 0; i < 100; ++i) {
        auto person = new Person(randommoney(rg),random_string(8),random_string(52));
        people.push_back(*person);
    }

    auto newtransactions = generateTransactions();


    int size = 100;
    if(newtransactions.size()<100){
        size = newtransactions.size();
    }

    auto transactionsForBlock = getTransactionsForBlock(newtransactions, size);



    int difficulty = 2;
    int startIndex = 1;
    int searchMultiplier = 1;




    int correctNonce = generateBlock("000000000000000000000000", difficulty, startIndex, searchMultiplier, transactionsForBlock);

    auto firstBock = new Block("000000000000000000000000", MerkleRoot(transactionsForBlock),difficulty,correctNonce,transactionsForBlock);

    blockChain.push_back(*firstBock);

    for (int j = 0; j < 5; ++j) {
        int nonce = getNewBlockNonce(newtransactions, size, difficulty, startIndex, searchMultiplier);

        blockChain.push_back(*new Block(hashBlock(blockChain.back()), MerkleRoot(transactionsForBlock),difficulty,nonce,transactionsForBlock));
    }

    return 0;
}