#include "dialog.h"
#include "ui_dialog.h"
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QPushButton *convertButton = ui->buttonBox->button( QDialogButtonBox::Ok );
    convertButton->setText( tr( "&Convert" ) );
    convertButton->setEnabled( false );

    connect( convertButton, SIGNAL( clicked()), this, SLOT( convertImage() ) );

    connect( &process, SIGNAL( readyReadStandardError()), this, SLOT( updateOutputTextEdit()) );

    connect( &process, SIGNAL( finished(int, QProcess::ExitStatus ) ), this, SLOT(processFinished(int,QProcess::ExitStatus)) );

    connect( &process, SIGNAL( error(QProcess::ProcessError)), this, SLOT( processError(QProcess::ProcessError)) );

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_browseButton_clicked()
{
    QString initialName = ui->sourceFileEdit->text();
    if( initialName.isEmpty() )
        initialName = QDir::homePath();
   QString fileName = QFileDialog::getOpenFileName( this, tr( "Choose file" ), initialName );
   fileName + QDir::toNativeSeparators(fileName);
   if( !fileName.isEmpty() ){
       ui->sourceFileEdit->setText( fileName );
       ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( true );
   }

}

void Dialog::convertImage()
{
    QString sourceFile = ui->sourceFileEdit->text();
    targetFile = QFileInfo( sourceFile ).path() + QDir::separator()
            + QFileInfo( sourceFile ).baseName() + "."
            + ui->comboBox->currentText().toLower();

    ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( false );
    ui->outputTextEdit->clear();

    QStringList args;
    if( ui->enhanceCheckBox->isChecked() )
        args << "-enhance";
    if( ui->monochromeCheckBox->isChecked() )
        args << "-monochrome";
    args << sourceFile << targetFile;
    process.start( "convert", args );
}

void Dialog::updateOutputTextEdit()
{
    QByteArray newData = process.readAllStandardError();
    QString text = ui->outputTextEdit->toPlainText() + QString::fromLocal8Bit( newData );
    ui->outputTextEdit->setPlainText( text );
}

void Dialog::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if( exitStatus == QProcess::CrashExit ){
        ui->outputTextEdit->append( tr("Conversion program crashed" ) );
    }else if( exitCode != 0 ){
        ui->outputTextEdit->append( tr( "Conversion failed" ) );
    }else {
        ui->outputTextEdit->append( tr("File %l created" ).arg( targetFile ) );
    }
    ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( true );
}

void Dialog::processError(QProcess::ProcessError error)
{
    if( error == QProcess::FailedToStart ){
        ui->outputTextEdit->append( tr( "Convertsion program not found" ) );
        ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( true );
    }
}
