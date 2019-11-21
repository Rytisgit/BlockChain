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
    }
    int amount;
    std::string from;
    std::string to;
};

class Block{
public:
    Block(std::string prevhash, std::string merkel, std::string timestamp,int diff, int nonce, std::vector<Transaction> b){
        transactions = b;
        this->prevhash = prevhash;
        this->merkel = merkel;
        auto a = std::chrono::high_resolution_clock();
        this->timestamp = a.now().time_since_epoch().count();
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
        auto person1 = person(rg);
        auto person2 = person(rg);
        while(person1 == person2) person2 = person(rg);
        auto transaction = new Transaction(amount(rg),people.at(person1).name, people.at(person2).name);
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
int main() {
    for (int i = 0; i < 100; ++i) {
        auto person = new Person(randommoney(rg),random_string(8),random_string(52));
        people.push_back(*person);
    }

    auto newtransactions = generateTransactions();

    int difficulty = 2;
    int startIndex = 1;
    int searchMultiplier = 1;



    std::time_t ticks = std::time(nullptr);
    auto time  = std::asctime(std::localtime(&ticks));
    int correctNonce = generateBlock("000000000000000000000000", difficulty, startIndex, searchMultiplier, newtransactions);

    auto firstBock = new Block("000000000000000000000000", MerkleRoot(newtransactions),time,difficulty,correctNonce,newtransactions);

    blockChain.push_back(*firstBock);

    blockChain.back();
    for (int j = 0; j < 5; ++j) {
        newtransactions = generateTransactions();
        auto nonce = generateBlock(hashBlock(blockChain.back()),difficulty,startIndex,searchMultiplier,newtransactions);
        auto block = new Block(hashBlock(blockChain.back()), MerkleRoot(newtransactions),time,difficulty,correctNonce,newtransactions);
        blockChain.push_back(*block);
    }

    return 0;
}