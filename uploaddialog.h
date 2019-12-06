#ifndef UPLOADDIALOG_H
#define UPLOADDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QDebug>
#include <QLineEdit>
#include <QCryptographicHash>

namespace Ui {
class UploadDialog;
}

class UploadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UploadDialog(QWidget *parent = nullptr);
    ~UploadDialog();

signals:
    void UploadFile(QString fileName, QString md5);

private slots:

    void on_chooseButton_clicked();

    void on_uploadButton_clicked();

private:
    Ui::UploadDialog *ui;

    QString uploadedFile_;

    QLineEdit *filePath_;
};

#endif // UPLOADDIALOG_H
