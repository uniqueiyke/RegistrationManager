#include "GwStudentData.h"

GwStudentData::GwStudentData()
    :height_{0.0}, weight_ {0.0}
{}

void GwStudentData::setRegNum(const QString &num)
{
    regNum_ = num;
}

void GwStudentData::setName(const QString &name)
{
    name_ = name;
}

void GwStudentData::setDateOfBirth(const QString &dateOfBirth)
{
    dateOfBirth_ = dateOfBirth;
}

void GwStudentData::setReligion(const QString &religion)
{
    religion_ = religion;
}

void GwStudentData::setBloodGroup(const QString &bloodGroup)
{
    bloodGroup_ = bloodGroup;
}

void GwStudentData::setGenotype(const QString &genotype)
{
    genotype_ = genotype;
}

void GwStudentData::setStudHeight(double h)
{
    height_ = h;
}

void GwStudentData::setStudWeight(double w)
{
    weight_ = w;
}

void GwStudentData::setPhotoPath(const QString &photoPath)
{
    photoPath_ =photoPath;
}

QString GwStudentData::regNum() const
{
    return regNum_;
}

QString GwStudentData::name() const
{
    return name_;
}

QString GwStudentData::dateOfBirth() const
{
    return dateOfBirth_;
}

QString GwStudentData::religion() const
{
    return religion_;
}

QString GwStudentData::bloodGroup() const
{
    return bloodGroup_;
}

QString GwStudentData::genotype() const
{
    return genotype_;
}

double GwStudentData::studHeight() const
{
    return height_;
}

double GwStudentData::studWeight() const
{
    return weight_;
}

QString GwStudentData::photoPath() const
{
    return photoPath_;
}
