#ifndef CONTRACT_REPOSITORY_H
#define CONTRACT_REPOSITORY_H

#include "Repository.h"
#include "../entities/Contract.h"

class ContractRepository : public Repository<Contract> {
public:
    explicit ContractRepository(const string& file = "") : Repository<Contract>(file) {}

protected:
    string idOf(const Contract& item) const override;
    vector<string> toRecord(const Contract& item) const override;
    Contract* fromRecord(const vector<string>& r) const override;
};

#endif
