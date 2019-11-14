#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPainter>

class Directory : public QGraphicsPixmapItem
{
//    Q_OBJECT
public:
    enum { Type = UserType + 1 };

    int type() const override {
        return Type;
    }

    Directory(int x, int y, QPixmap pixmap, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void setName(std::string n);
    void setParent(Directory* parent);
    std::string getName() const;

//    virtual void onClick() = 0; // must be overriden
    virtual void AddDir(Directory* dir){}
    virtual void RemoveDir(Directory* dir){}
    virtual std::vector<Directory*> getContents() const {}

protected:
    std::string name;
    int x;
    int y;

    Directory* parent;

private:
};

#endif // DIRECTORY_H
