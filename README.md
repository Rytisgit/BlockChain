# BlockChain
Blokchain based on hashing from hashing repository

## Implementation

For Hashing Uses my Hashing algorithm found at https://github.com/Rytisgit/Hashing

A block's hash is considered valid when it has a sufficient amount of 0's at the start of the hash, dictated by difficulty

Checks for inssuficient funds from generated transactions

Uses 5 different sets of 100 transaction to mine for a correct hash.

#### Compilation

`g++ -O3 main.cpp -o Blockhain`

### Output

function Prints the hashes found for blocks until there are none left in the 10000 generated transaction pool

![output](https://i.imgur.com/wTe1mbk.png "output")

