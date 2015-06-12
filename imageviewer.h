#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>

class ImageViewer : public QGraphicsView
{
    Q_OBJECT
private:
    constexpr static const qreal ZOOM_FACTOR = 0.25;
    QGraphicsScene      _scene;
    QGraphicsPixmapItem _pixmap;


    void initScene();
    void changeScale(qreal scale);

public:
    ImageViewer(QWidget *parent = 0);
    ImageViewer(const QString &filePath, QWidget *parent = 0);
    ImageViewer(const QPixmap &pixmap, QWidget *parent = 0);

    void zoomIn(const QPointF &pos = QPointF());
    void zoomOut(const QPointF &pos = QPointF());
    void setAdjustScale(const bool enable);
    void setOriginScale();

    void setPixmap(const QPixmap &pixmap);
    void setPixmap(const QString &filePath);
    QPixmap pixmap();

protected:
    void wheelEvent(QWheelEvent *e);
};

#endif // IMAGEVIEWER_H
