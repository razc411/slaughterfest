#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QFile>
#include <QDir>

class FileManager : public QObject
{
    Q_OBJECT
    public:
        explicit FileManager(QObject *parent = 0);
        int doesFileExist(QString fname);
        QFile * grabFileHandle(QString fname);
        QStringList grabFileListing();
        void setDirectory(QString dir);

    signals:

    public slots:

    private:
        QDir directory;
        QStringList fileListing;

        void loadFileListing();
};

#endif // FILEMANAGER_H