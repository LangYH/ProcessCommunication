#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QProcess>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_browseButton_clicked();
    void convertImage();
    void updateOutputTextEdit();
    void processFinished( int exitCode, QProcess::ExitStatus exitStatus );
    void processError( QProcess::ProcessError error );

private:
    QProcess process;
    QString targetFile;

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
