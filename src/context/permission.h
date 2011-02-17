#ifndef PERMISSION_H
#define PERMISSION_H

#include <QDataStream>

class Permission
{
public:
    Permission() {}
    ~Permission() {}

    bool canCreateClass() const {return createClass;}
    void setCreateClass(bool enable) {createClass = enable;}

    bool canCreateExam() const {return createExam;}
    void setCreateExam(bool enable) {createExam = enable;}

    bool canCreateAccount() const {return createAccount;}
    void setCreateAccount(bool enable) {createAccount = enable;}

    bool canEditScore() const {return editScore;}
    void setEditScore(bool enable) {editScore = enable;}

    friend QDataStream & operator << (QDataStream &stream, const Permission &permission)
    {
        stream <<
                permission.createClass <<
                permission.createExam  <<
                permission.createAccount <<
                permission.editScore;
        return stream;
    }

    friend QDataStream & operator >> (QDataStream &stream, Permission &permission)
    {
        stream >>
                permission.createClass >>
                permission.createExam  >>
                permission.createAccount >>
                permission.editScore;
        return stream;
    }

private:
    bool createClass;
    bool createExam;
    bool createAccount;
    bool editScore;
};

#endif // PERMISSION_H
