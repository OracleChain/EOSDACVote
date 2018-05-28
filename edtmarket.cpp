#include "edtmarket.hpp"

using namespace eosio;

edtmarket::edtmarket(account_name s)
    : contract(s),
      _weights(_self, s)
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

    auto to_weight = _weights.find(to);
    if (to_weight == _weights.end()) {
        _weights.emplace(_self, [&](auto& a){
            a.owner = to;
            a.weight = quantity.amount * (yeas ? 1 : -1);
            print("lambda\n");
        });
        print("new vote\n");
    } else {
        _weights.modify(to_weight, _self, [&](auto& a){
            a.weight += quantity.amount * (yeas ? 1 : -1);
            print("lambda\n");
        });
        print("normal vote\n");
    }

    print("vote success");
}

bool edtmarket::is_contract_account(name name)
{
    // if cleos get code {name} returns no zero, means contract account
    return false;
}

EOSIO_ABI( edtmarket, (vote) )
