#include "mainwindow.h"

#include <QFileDialog>
#include <QPicture>
#include <QDebug>
#include <QProgressBar>
#include <QMimeType>
#include <QImageReader>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);
    QSettings cfg;
    restoreGeometry( cfg.value("MainWindows/Geometry").toByteArray() );
    restoreState( cfg.value("MainWindows/State").toByteArray() );

    ui.splitter->restoreGeometry( cfg.value("MWSplitter/Geometry").toByteArray() );
    ui.splitter->restoreState( cfg.value("MWSplitter/State").toByteArray() );

    emit ui.actionBottom_buttons->triggered( cfg.value("View/BottomButtons", true).toBool() );
    ui.actionBottom_buttons->setChecked( cfg.value("View/BottomButtons", true).toBool() );
    emit ui.actionHistory->triggered( cfg.value("View/History", true).toBool() );
    ui.actionHistory->setChecked( cfg.value("View/History", true).toBool() );

    ui.ListView_History->setModel( &_historyModel );
    ui.statusBar->addWidget( &statusFilePath );
    ui.statusBar->addWidget( new QWidget, 100 );
    ui.statusBar->addWidget( &statusImageWidth );
    ui.statusBar->addWidget( &statusImageHeight );
    ui.statusBar->addWidget( &statusImageFormat );
}

MainWindow::~MainWindow()
{
    QSettings cfg;
    cfg.setValue("MainWindows/Geometry", this->saveGeometry());
    cfg.setValue("MainWindows/State", this->saveState());

    cfg.setValue("MWSplitter/Geometry", ui.splitter->saveGeometry() );
    cfg.setValue("MWSplitter/State", ui.splitter->saveState() );

    cfg.setValue("View/BottomButtons", ui.actionBottom_buttons->isChecked());
    cfg.setValue("View/History", ui.actionHistory->isChecked());
}

void MainWindow::on_actionExit_triggered()
{
    const int success = 0;
    qApp->exit( success );
}

void MainWindow::on_actionOpenPicture_triggered()
{
    QString filePath = QFileDialog::getOpenFileName();
    if( openPicture(filePath) )
        addToHistory( filePath );
}

void MainWindow::on_PushButton_Open_clicked()
{
    emit ui.actionOpenPicture->triggered();
}

void MainWindow::on_ListView_History_clicked(const QModelIndex &index)
{
    QString filePath = _historyModel.data(index, Qt::DisplayRole).toString();
    openPicture( filePath );
}

bool MainWindow::openPicture(const QString &filePath)
{

    if( ! filePath.isEmpty() ){
        QString ImageFormat;
        {
            QImageReader r(filePath);
            ImageFormat = r.format().toUpper();
        }

        QPixmap pm;
        if( pm.load( filePath ) ){

            statusFilePath.setText( filePath );
            statusImageWidth.setText( "W: " + QString::number( pm.width() ) );
            statusImageHeight.setText( "H: " + QString::number( pm.height() ) );
            qDebug() << filePath.indexOf('.');
            statusImageFormat.setText( ImageFormat );
            ui.Label_Image->setPixmap( pm );
            return true;
        }
    }
    return false;
}

bool MainWindow::addToHistory(const QString &filePath)
{
    if( ! _history.contains( filePath ) ){
        _history.append( filePath );
        _historyModel.setStringList( _history );


//        QModelIndex currentIndex = ui.ListView_History->currentIndex();
//        QModelIndex nextIndex    = currentIndex.sibling(ui.ListView_History->model()->rowCount(),0);
        ui.ListView_History->setCurrentIndex( _historyModel.index(_history.count()-1,0) );

        return true;
    }
    return false;
}

bool MainWindow::selectFromHistory(const QModelIndex &index)
{
    if( ! index.isValid() ){
        qDebug() << "Invalid Index!";
        return false;
    }

    QItemSelectionModel *newSelectionModel = ui.ListView_History->selectionModel();
    ui.ListView_History->setCurrentIndex(index);
    ui.ListView_History->setSelectionModel( newSelectionModel );

    QString filePath = _historyModel.data( index, Qt::DisplayRole ).toString();
    openPicture(filePath);

    return true;
}

void MainWindow::on_actionHistory_triggered(bool checked)
{
    ui.ListView_History->setVisible(checked);
}

void MainWindow::on_actionBottom_buttons_triggered(bool checked)
{
    ui.Widget_BottomButtons->setVisible(checked);
}

void MainWindow::on_actionCloseImage_triggered()
{
    ui.Label_Image->clear();
}

void MainWindow::on_actionNextPicture_triggered()
{
    QModelIndex currentIndex = ui.ListView_History->currentIndex();
    QModelIndex nextIndex    = currentIndex.sibling(currentIndex.row()+1,0);

    selectFromHistory(nextIndex);
}

void MainWindow::on_ToolButton_Next_clicked()
{
    emit ui.actionNextPicture->triggered();
}

void MainWindow::on_actionPrevPicture_triggered()
{
    QModelIndex currentIndex = ui.ListView_History->currentIndex();
    QModelIndex prevIndex    = currentIndex.sibling(currentIndex.row()-1,0);

    selectFromHistory(prevIndex);
}

void MainWindow::on_ToolButton_Back_clicked()
{
    emit ui.actionPrevPicture->triggered();
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName();

    QFile file( fileName );
    if( file.open( QIODevice::WriteOnly ) ){
        QTextStream stream(&file);
        foreach (QString str, _history) {
            stream << str << '\n';
        }
        file.close();
    }
}

void MainWindow::on_actionClear_triggered()
{
    _history.clear();
    _historyModel.setStringList( _history );
}

void MainWindow::on_actionLoad_triggered()
{
    QString fileName = QFileDialog::getOpenFileName();

    QFile file( fileName );
    if( file.open( QIODevice::ReadOnly ) ){
        QTextStream stream(&file);
        while( ! stream.atEnd() ) {
            _history.append( stream.readLine() );
        }
        file.close();
        _historyModel.setStringList( _history );
    }
}

void MainWindow::on_actionScaled_content_triggered(bool checked)
{
    ui.Label_Image->setScaledContents(checked);
}

void MainWindow::on_actionRemove_selected_triggered()
{
    QModelIndex currentIndex = ui.ListView_History->currentIndex();
    _history.removeAt( currentIndex.row() );
    _historyModel.setStringList( _history );
}
