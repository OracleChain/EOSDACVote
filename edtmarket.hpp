#pragma once

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>

/*
struct vote_info
{
    account_name voter;
    account_name voted;
    asset quantity;
    int64_t stake;
    bool yeas; // true for yeas, false for nays
};
*/

using namespace eosio;

/*
@abi table weight_info
*/
struct weight_info
{
    name owner;
    int64_t weight;

    uint64_t primary_key() const { return owner; }

    EOSLIB_SERIALIZE( weight_info, (owner) (weight))
};

typedef eosio::multi_index<N(weight), weight_info> weight_table;

class edtmarket : public eosio::contract
{
public:
    edtmarket(account_name s);
    ~edtmarket();

    // @abi action
    void vote(name from,
              name to,
              asset quantity,
              bool yeas);

private:
    struct account {
        asset balance;

        uint64_t primary_key() const { return balance.symbol.name(); }
    };

    typedef eosio::multi_index<N(accounts), account> accounts;

private:
    bool is_contract_account(name name);

private:
    weight_table _weights;
};
