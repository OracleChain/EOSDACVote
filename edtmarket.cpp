#include "edtmarket.hpp"
#include <eosiolib/system.h>

using namespace eosio;

edtmarket::edtmarket(account_name s)
    : contract(s),
      _weights(_self, s),
      _contract_vote_weight(1),
      _user_vote_weight(1),
      _contract_time_weight(1),
      _user_time_weight(1),
      _max_time_limit(60 * 24 * 30 * 3)  // 90 days
{
}

edtmarket::~edtmarket()
{
}

void edtmarket::vote(name from, name to, asset quantity, bool yeas)
{
    eosio_assert(from != to, "cannot vote to self");    // can I ?
    eosio_assert(is_account(from), "Invalid account");
    eosio_assert(is_account(to), "Invalid account");
    //eosio_assert(!(!is_contract_account(from) && !is_contract_account(to)), "Can't be two normal accounts");

    require_auth(from);

    require_recipient(from);
    require_recipient(to);

    eosio_assert(quantity.amount > 0, "must vote positive stake");

    //accounts from_acnts(N("eosio.token"), from);
    //const auto& f = from_acnts.get(quantity.symbol.name(), "no balance object found");
    //eosio_assert(f.balance.amount >= quantity.amount, "overddrawn balance");
    //asset f_balance = eosdactoken(tokenContract).get_balance(from, quantity.symbol.name());
    //eosio_assert(f_balance.amount >= quantity.amount, "overddrawn balance");

    int64_t vote_weight = 1, time_weight = 1;
    if (is_contract_account(from)) {
        vote_weight = _contract_vote_weight;
        time_weight = _contract_time_weight;
    } else {
        vote_weight = _user_vote_weight;
        time_weight = _user_time_weight;
    }

    uint32_t minutes = now() / 60;
    auto to_weight = _weights.find(to);
    if (to_weight == _weights.end()) {
        _weights.emplace(_self, [&](auto& a){
            a.owner = to;
            a.weight = quantity.amount * (yeas ? vote_weight : (-1 * vote_weight))
                    + ((minutes - a.last_vote_time) > _max_time_limit ? _max_time_limit : (minutes - a.last_vote_time)) * (yeas ? time_weight : (-1 * time_weight));
            a.last_vote_time = minutes;
        });
    } else {
        _weights.modify(to_weight, _self, [&](auto& a){
            a.weight += (quantity.amount * (yeas ? vote_weight : (-1 * vote_weight))
                    + ((minutes - a.last_vote_time) > _max_time_limit ? _max_time_limit : (minutes - a.last_vote_time)) * (yeas ? time_weight : (-1 * time_weight)));
            a.last_vote_time = minutes;
        });
    }
}

void edtmarket::configvote(int64_t contract_vote_weight, int64_t user_vote_weight, int64_t contract_time_weight, int64_t user_time_weight, int64_t max_time_limit)
{
    require_auth(_self);

    _contract_time_weight = contract_vote_weight;
    _user_vote_weight = user_vote_weight;
    _contract_time_weight = contract_time_weight;
    _user_time_weight = user_time_weight;
    _max_time_limit = max_time_limit;
}

bool edtmarket::is_contract_account(name name)
{
    // if cleos get code {name} returns no zero, means contract account
    return false;
}

EOSIO_ABI( edtmarket, (configvote) (vote) )
