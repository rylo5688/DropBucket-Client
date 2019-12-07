#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QDebug>

class Directory : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    enum { Type = UserType + 1 };

    int type() const override {
        return Type;
    }

    Directory(int x, int y, QPixmap pixmap, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void setName(QString n);
    void setParent(Directory* parent);
    void setRelativePath(QString path);
    QString getName() const;
    QString getRelativePath() const;
    Directory* getParent();

    virtual void onClick() { } // must be overriden
    virtual void AddDir(Directory* dir){}
    virtual void RemoveDir(Directory* dir){}
    std::vector<Directory*> getContents() {
        qDebug() << contents_.size();
        return contents_;
    }

public slots:
    virtual void NetworkDeleteSuccessful() {};

signals:
    void Deleted(Directory *deleted);

protected:
    QString name;
    QString relativePath_;
    int x;
    int y;

    Directory* parent_;

    std::vector<Directory*> contents_;


};

#endif // DIRECTORY_H
