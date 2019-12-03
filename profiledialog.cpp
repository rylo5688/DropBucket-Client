#include "profiledialog.h"
#include "ui_profiledialog.h"

ProfileDialog::ProfileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProfileDialog)
{
    ui->setupUi(this);
}

ProfileDialog::~ProfileDialog()
{
    delete ui;
}

void ProfileDialog::on_signOutButton_clicked()
{
    qDebug() << "sign out button clicked";

    QByteArray id = QSysInfo::machineUniqueId();
    QString data = QString("{\"Username\":\"%1\",\"Password\":\"%2\",\"Deviceid\":\"%3\"}").arg(MainWindow::username, MainWindow::password, id);
//    QFile file;
    qDebug() << data;
    QByteArray toPost = data.toUtf8();
    NetworkManager* nMgr = NetworkManager::getInstance();

    nMgr->SignOutPost(&toPost);
}
