#include "imageviewer.h"

#include <QDebug>
#include <QMouseEvent>

void ImageViewer::initScene()
{
    _scene.addItem( &_pixmap );
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

    QPixmap pix;
    pix.load(filePath);

    _pixmap.setPixmap( filePath );
}

ImageViewer::ImageViewer(const QPixmap &pixmap, QWidget *parent)
    : QGraphicsView(parent)
{
    initScene();

    _pixmap.setPixmap( pixmap );
}

void ImageViewer::setPixmap(const QPixmap &pixmap)
{
    _pixmap.setPixmap( pixmap );
}

QPixmap ImageViewer::pixmap()
{
    return _pixmap.pixmap();
}

void ImageViewer::wheelEvent(QWheelEvent *e)
{
    // e->modifiers() == Qt::ControlModifier
    if( e->delta() > 0 ){
        changeScale( 1.25 );
    }else{
        changeScale( 0.85 );
    }
}
