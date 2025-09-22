#include "mainwindow.h"
#include "ui_mainwindow.h"

ReactionTimeTester tester;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(&tester, &ReactionTimeTester::NewRound,   this, &MainWindow::onNewRound);
    QObject::connect(&tester, &ReactionTimeTester::Penalty,    this, &MainWindow::onPenalty);
    QObject::connect(&tester, &ReactionTimeTester::NotifyUser, this, &MainWindow::onNotifyUser);
    QObject::connect(&tester, &ReactionTimeTester::SetResult,  this, &MainWindow::onSetResult);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onNewRound()
{
    ui->label_Info->setText(QString("Round started!"));
}

void MainWindow::onPenalty()
{
    ui->label_Info->setText(QString("250 ms penalty added\n for random clicking!"));
}

void MainWindow::onNotifyUser()
{
    ui->label_Info->setText(QString("Click!"));
}

void MainWindow::onSetResult()
{
    ui->label_Info->setText(QString("Result: ") + QString::number(tester.GetElapsedTime()) + QString(" ms"));
}


void MainWindow::on_Button_StartStop_clicked()
{
    tester.EvaluateStatus();
}

