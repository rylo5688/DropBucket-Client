#include "profiledialog.h"
#include "ui_profiledialog.h"
#include "mainwindow.h"

/**
 * @brief ProfileDialog::ProfileDialog
 * Profile dialog constructor
 * @param parent Parent
 */
ProfileDialog::ProfileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProfileDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("  ");
}

/**
 * @brief ProfileDialog::~ProfileDialog
 * Profile dialog destructor
 */
ProfileDialog::~ProfileDialog()
{
    delete ui;
}

/**
 * @brief ProfileDialog::on_signOutButton_clicked
 * Handles when the sign out button is clicked
 */
void ProfileDialog::on_signOutButton_clicked()
{
    QByteArray id = QSysInfo::machineUniqueId();
    QString data = QString("{\"username\":\"%1\",\"password\":\"%2\",\"device_id\":\"%3\"}").arg(MainWindow::username, MainWindow::password, id);
    QByteArray toPost = data.toUtf8();
    NetworkManager* nMgr = NetworkManager::getInstance();

    nMgr->SignOutPost(&toPost);
}
