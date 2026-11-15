#ifndef TUTOR_REPOSITORY_H
#define TUTOR_REPOSITORY_H

#include "Repository.h"
#include "../entities/Tutor.h"

class TutorRepository : public Repository<Tutor> {
public:
    explicit TutorRepository(const string& file = "") : Repository<Tutor>(file) {}

protected:
    string idOf(const Tutor& item) const override;
    vector<string> toRecord(const Tutor& item) const override;
    Tutor* fromRecord(const vector<string>& r) const override;
};

#endif
