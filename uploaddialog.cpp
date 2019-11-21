#include "uploaddialog.h"
#include "ui_uploaddialog.h"

UploadDialog::UploadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UploadDialog)
{
    ui->setupUi(this);
    uploadedFile_ = "";
    filePath_ = ui->dirEdit;
}

UploadDialog::~UploadDialog()
{
    delete ui;
}

void UploadDialog::on_uploadButton_clicked()
{
    if(uploadedFile_ != "") {
        QFile file(uploadedFile_);
        if(!file.open(QIODevice::ReadOnly)) {
            return;
        }
        QByteArray data = file.readAll();
        QByteArray md5 = QCryptographicHash::hash(data, QCryptographicHash::Md5);
        std::string Md5 = QString(md5).toStdString();
        UploadFile(uploadedFile_, Md5);

        close();
    }
}

void UploadDialog::on_chooseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), "",
        tr("Images (*.png, *.xpm, *.jpg);;Text Files (*.txt);;XML files (*.xml)"));

    filePath_->setText(fileName);
    uploadedFile_ = fileName;

    qDebug() << fileName;
}

