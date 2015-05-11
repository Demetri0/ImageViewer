#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QStringListModel>
#include <QStringList>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private slots:
    void on_actionExit_triggered();

    void on_actionOpenPicture_triggered();

    void on_PushButton_Open_clicked();

    void on_ListView_History_clicked(const QModelIndex &index);

    bool openPicture(const QString &filePath);
    bool addToHistory(const QString &filePath);
    bool selectFromHistory(const QModelIndex &index);

    void on_actionHistory_triggered(bool checked);

    void on_actionBottom_buttons_triggered(bool checked);

    void on_actionCloseImage_triggered();

    void on_actionNextPicture_triggered();

    void on_ToolButton_Next_clicked();

    void on_actionPrevPicture_triggered();

    void on_ToolButton_Back_clicked();

    void on_actionSave_triggered();

    void on_actionClear_triggered();

    void on_actionLoad_triggered();

    void on_actionScaled_content_triggered(bool checked);

    void on_actionRemove_selected_triggered();

private:
    Ui::MainWindow ui;
    int _currentHistoryItem = 0;
    QStringListModel _historyModel;
    QStringList _history;

    QLabel statusFilePath;
    QLabel statusImageHeight;
    QLabel statusImageWidth;
    QLabel statusImageFormat;
};

#endif // MAINWINDOW_H
