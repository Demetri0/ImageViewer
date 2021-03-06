#include "imageviewer.h"

#include <QDebug>
#include <QMouseEvent>

void ImageViewer::initScene()
{
    _scene.addItem( & _pixmap );
    this->setScene( & _scene );
}

void ImageViewer::changeScale(qreal scale)
{
    _pixmap.setScale( _pixmap.scale() * scale );
}

ImageViewer::ImageViewer(QWidget *parent)
    : QGraphicsView(parent)
{
    initScene();
}

ImageViewer::ImageViewer(const QString &filePath, QWidget *parent)
    : QGraphicsView(parent)
{
    initScene();

    setPixmap( filePath );
}

ImageViewer::ImageViewer(const QPixmap &pixmap, QWidget *parent)
    : QGraphicsView(parent)
{
    initScene();

    setPixmap( pixmap );
}

void ImageViewer::zoomIn(const QPointF &p)
{
    changeScale( 1+ZOOM_FACTOR );
//    _pixmap.setPos( _pixmap.pos() - p );
}

void ImageViewer::zoomOut(const QPointF &p)
{
    changeScale( 1-ZOOM_FACTOR );
//    _pixmap.setPos( _pixmap.pos() - p );
}

void ImageViewer::setAdjustScale(const bool )
{
    /// \todo Adjust content scale
}

void ImageViewer::setOriginScale()
{
    _pixmap.setScale( 1 );
}

void ImageViewer::setPixmap(const QPixmap &pixmap)
{
    _pixmap.setPixmap( pixmap );
}

void ImageViewer::setPixmap(const QString &filePath)
{
    QPixmap pix;
    pix.load(filePath);

    setPixmap(pix);
}

QPixmap ImageViewer::pixmap()
{
    return _pixmap.pixmap();
}

void ImageViewer::wheelEvent(QWheelEvent *e)
{
    if( e->delta() > 0 ){
        zoomIn( e->pos() );
    }else{
        zoomOut( e->pos() );
    }
}

void ImageViewer::mousePressEvent(QMouseEvent *e)
{
    _mousePressPoint = e->pos() - _pixmap.pos();
}

void ImageViewer::mouseMoveEvent(QMouseEvent *e)
{
    qreal px = e->pos().x() - _mousePressPoint.x();
    qreal py = e->pos().y() - _mousePressPoint.y();

    _pixmap.setPos( px, py );
}

