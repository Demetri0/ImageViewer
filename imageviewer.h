#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>

class ImageViewer : public QGraphicsView
{
    Q_OBJECT
private:
    QGraphicsScene      _scene;
    QGraphicsPixmapItem _pixmap;


    void initScene();
    void changeScale(qreal scale);

public:
    ImageViewer(QWidget *parent = 0);
    ImageViewer(const QString &filePath, QWidget *parent = 0);
    ImageViewer(const QPixmap &pixmap, QWidget *parent = 0);

    void setPixmap(const QPixmap &pixmap);
    QPixmap pixmap();

protected:
    void wheelEvent(QWheelEvent *e);
};

#endif // IMAGEVIEWER_H
