#ifndef GWSTUDENTDATA_H
#define GWSTUDENTDATA_H

#include <QString>

class GwStudentData
{
public:
    GwStudentData();

    void setRegNum(const QString &num);
    void setName(const QString &name);
    void setDateOfBirth(const QString &dateOfBirth);
    void setReligion(const QString &religion);
    void setBloodGroup(const QString &bloodGroup);
    void setGenotype(const QString &genotype);
    void setStudHeight(double h);
    void setStudWeight(double w);
    void setPhotoPath(const QString &photoPath);

    QString regNum() const;
    QString name() const;
    QString dateOfBirth() const;
    QString religion() const;
    QString bloodGroup() const;
    QString genotype() const;
    double studHeight() const;
    double studWeight() const;
    QString photoPath() const;

private:
    QString regNum_;
    QString name_;
    QString dateOfBirth_;
    QString religion_;
    QString bloodGroup_;
    QString genotype_;
    double height_;
    double weight_;
    QString photoPath_;
};

#endif // GWSTUDENTDATA_H
