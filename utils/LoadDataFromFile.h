#ifndef LOADDATAFROMFILE_H
#define LOADDATAFROMFILE_H
#include <QString>
#include <QFile>

class LoadDataFromFile
{
public:
    LoadDataFromFile();
    virtual ~LoadDataFromFile() {}
    virtual void loadDataFromFile(const QString path) = 0;
    virtual void saveInFile(QJsonObject & root) = 0;
};

#endif // LOADDATAFROMFILE_H
