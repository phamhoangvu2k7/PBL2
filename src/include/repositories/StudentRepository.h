#ifndef STUDENT_REPOSITORY_H
#define STUDENT_REPOSITORY_H

#include "Repository.h"
#include "../entities/Student.h"

class StudentRepository : public Repository<Student> {
public:
    explicit StudentRepository(const string& file = "") : Repository<Student>(file) {}

protected:
    string idOf(const Student& item) const override;
    vector<string> toRecord(const Student& item) const override;
    Student* fromRecord(const vector<string>& r) const override;
};

#endif
