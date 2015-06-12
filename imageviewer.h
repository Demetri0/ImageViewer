#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>

class ImageViewer : public QWidget
{
    Q_OBJECT
private:
    QPixmap _pixmap;

public:
    ImageViewer(QWidget *parent = 0);
    ImageViewer(const QString &filePath, QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // IMAGEVIEWER_H
