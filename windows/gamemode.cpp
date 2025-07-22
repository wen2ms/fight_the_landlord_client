#include "gamemode.h"

#include "ui_gamemode.h"

#include "mainwindow.h"

GameMode::GameMode(QWidget *parent) : QDialog(parent), ui(new Ui::GameMode) {
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    
    connect(ui->standalone_btn, &QPushButton::clicked, this, [=]() {
        MainWindow* main_window = new MainWindow;
        
        connect(main_window, &MainWindow::window_close, this, &GameMode::show);
        
        main_window->show();
        
        hide();
    });
    
    connect(ui->online_btn, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);
    });
}

GameMode::~GameMode() {
    delete ui;
}

void GameMode::closeEvent(QCloseEvent *event) {
    if (ui->stackedWidget->currentIndex() == 1) {
        ui->stackedWidget->setCurrentIndex(0);
        
        event->ignore();
    } else {
        event->accept();
    }
}
