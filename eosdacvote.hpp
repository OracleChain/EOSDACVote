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
    uint32_t last_vote_time = 0;

    uint64_t primary_key() const { return owner; }

    EOSLIB_SERIALIZE( weight_info, (owner) (weight) (last_vote_time))
};

typedef eosio::multi_index<N(weights), weight_info> weight_table;

class eosdacvote : public eosio::contract
{
public:
    eosdacvote(account_name s);
    ~eosdacvote();

    // @abi action
    void vote(name from,
              name to,
              asset quantity,
              bool yeas);

    // @abi action
    void configvote(int64_t contract_vote_weight,
                    int64_t user_vote_weight,
                    int64_t contract_time_weight,
                    int64_t user_time_weight,
                    int64_t max_time_limit);

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

    int64_t _contract_vote_weight;
    int64_t _user_vote_weight;
    int64_t _contract_time_weight;
    int64_t _user_time_weight;
    int64_t _max_time_limit;
};
