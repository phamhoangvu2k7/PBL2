#ifndef CLASS_REPOSITORY_H
#define CLASS_REPOSITORY_H

#include "Repository.h"
#include "../entities/Class.h"

class ClassRepository : public Repository<Class> {
public:
    explicit ClassRepository(const string& file = "") : Repository<Class>(file) {}

protected:
    string idOf(const Class& item) const override;
    vector<string> toRecord(const Class& item) const override;
    Class* fromRecord(const vector<string>& r) const override;
};

#endif
