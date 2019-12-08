#include "uploaddialog.h"
#include "ui_uploaddialog.h"

/**
 * @brief UploadDialog::UploadDialog
 * Upload dialog constructor
 * @param parent Parent
 */
UploadDialog::UploadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UploadDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("  ");
    uploadedFile_ = "";
    filePath_ = ui->dirEdit;
}

/**
 * @brief UploadDialog::~UploadDialog
 * Upload dialog destructor
 */
UploadDialog::~UploadDialog()
{
    delete ui;
}

/**
 * @brief UploadDialog::on_uploadButton_clicked
 * Handles when an upload button is clicked
 */
void UploadDialog::on_uploadButton_clicked()
{
    if(uploadedFile_ != "") {
        QFile file(uploadedFile_);
        if(!file.open(QIODevice::ReadOnly)) {
            return;
        }
        QByteArray data = file.readAll();
        QByteArray md5 = QCryptographicHash::hash(data, QCryptographicHash::Md5);
        QString Md5 = md5.toHex();
        UploadFileSignal(uploadedFile_, Md5);

        close();
    }
}

/**
 * @brief UploadDialog::on_chooseButton_clicked
 * Handles when the file choose button is clicked
 */
void UploadDialog::on_chooseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), "",
        tr("*"));

    filePath_->setText(fileName);
    uploadedFile_ = fileName;
}

