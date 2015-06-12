#include "mainwindow.h"

#include <QFileDialog>
#include <QPicture>
#include <QDebug>
#include <QProgressBar>
#include <QMimeType>
#include <QImageReader>
#include <QSettings>
#include <QVector>
#include <QProgressDialog>

#include "imageviewer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);
    QSettings cfg;
    restoreGeometry( cfg.value("MainWindows/Geometry").toByteArray() );
    restoreState( cfg.value("MainWindows/State").toByteArray() );

    ui.Splitter_Main->restoreGeometry( cfg.value("MWSplitter/Geometry").toByteArray() );
    ui.Splitter_Main->restoreState( cfg.value("MWSplitter/State").toByteArray() );

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

//    ui.GraphicsView_Image->setScene( &_scene );
//    _scene.addItem( &_pixmap );
//    ui.verticalLayout->addWidget( &_view );
}

MainWindow::~MainWindow()
{
    QSettings cfg;
    cfg.setValue("MainWindows/Geometry", this->saveGeometry());
    cfg.setValue("MainWindows/State", this->saveState());

    cfg.setValue("MWSplitter/Geometry", ui.Splitter_Main->saveGeometry() );
    cfg.setValue("MWSplitter/State", ui.Splitter_Main->saveState() );

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

    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      tr("Select image files"),
                                                      QStandardPaths::writableLocation( QStandardPaths::PicturesLocation )
                                                      );

    quint64 i = 0;
    QProgressDialog progress(this);
    progress.setMaximum( files.count() );
    progress.setModal( true );
    progress.setLabelText( tr("Files opening...") );
    progress.show();

    foreach (QString filePath, files) {
        if( ! progress.isVisible() )
            return;
        if( openPicture(filePath) )
            addToHistory( filePath );

        progress.setValue( ++i );
        qApp->processEvents();
    }
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
            statusImageWidth.setText( tr("W: ", "Image width") + QString::number( pm.width() ) );
            statusImageHeight.setText( tr("H: ", "Image height") + QString::number( pm.height() ) );
            statusImageFormat.setText( ImageFormat );

//            _view.setPixmap( pm );
            ui.Pixmap->setPixmap( pm );
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
    /// \todo crash fix it
//    _scene.clear();
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
    int notFindCounter = 0;

    QFile file( fileName );
    if( file.open( QIODevice::ReadOnly ) ){
        QTextStream stream(&file);
        while( ! stream.atEnd() ) {
            QString str = stream.readLine();
            if( QFile::exists(str) )
                _history.append( str );
            else
                notFindCounter++;
        }
        file.close();
        statusFilePath.setText( tr("History is succesfull load.") );
        _historyModel.setStringList( _history );
    }
}

void MainWindow::on_actionScaled_content_triggered(bool checked)
{
    /// \todo Adjust scale
//    if(checked)
//        _pixmap.setScale(1);
}

void MainWindow::on_actionRemove_selected_triggered()
{
    QModelIndex currentIndex = ui.ListView_History->currentIndex();
    _history.removeAt( currentIndex.row() );
    _historyModel.setStringList( _history );
}

void MainWindow::on_actionOriginScale_triggered()
{
    ui.Pixmap->setOriginScale();
}

void MainWindow::on_actionZoomIn_triggered()
{
    ui.Pixmap->zoomIn();
}

void MainWindow::on_actionZoomOut_triggered()
{
    ui.Pixmap->zoomOut();
}
