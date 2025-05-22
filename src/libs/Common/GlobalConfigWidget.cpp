/*
 * src/libs/Common/GlobalConfigWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "GlobalConfigWidget.hpp"

/* system headers */

/* Qt headers */
#include <QApplication>
#include <QCheckBox>
#include <QClipboard>
#include <QComboBox>
#include <QCompleter>
#include <QCoreApplication>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>

/* local library headers */

/* local headers */
#include "Satellite.hpp"
#include "Settings.hpp"


GlobalConfigWidget::GlobalConfigWidget( QWidget *parent )
: QWidget( parent )
, mpUseSatellite( new QCheckBox( tr("Use Satellite (Inter-Process Communication)"), this ) )
, mpSatellitePortLabel( new QLabel( tr("Satellite Port"), this ) )
, mpSatellitePort( new QSpinBox( this ) )
, mpUseGlobalStyleSheetFile( new QCheckBox( this ) )
, mpStyleSheetFileName( new QLineEdit( this ) )
, mpDotButton( new QPushButton( "...", this ) )
, mpClipboardLabel( new QLabel( tr("Use Clipboard: "), this ) )
, mpClipboardSelection( new QComboBox( this ) )
, mpAnimateViews( new QCheckBox( tr("Animate Views"), this ) )
, mpNormalizeCase( new QCheckBox( tr("Normalize Case In Text"), this ) )
, mpNormalizeSpaces( new QCheckBox( tr("Normalize Spaces In Text"), this ) )
, mpDoubleClickLabel( new QLabel( tr("Double Click Interval:"), this ) )
, mpDoubleClickInterval( new QSpinBox( this ) )
{
   QGridLayout *mainLayout = new QGridLayout( this );

   QCompleter *completer = new QCompleter( this );
   completer->setModel( new QFileSystemModel( completer ) );
   mpStyleSheetFileName->setCompleter( completer );

   showClipboard( false );
   showAnimate( false );
   showNormalize( false );
   showDoubleClickInterval( false );

   mpSatellitePort->setRange( 1025, 65535 );
   mpSatellitePort->setAlignment( Qt::AlignRight );

   mpDoubleClickInterval->setRange( 100, 1000 );
   mpDoubleClickInterval->setSingleStep( 50 );
   mpDoubleClickInterval->setSuffix( "ms" );
   mpDoubleClickInterval->setAlignment( Qt::AlignRight );

   /* evil hack */
   mpDotButton->setMaximumWidth( mpDotButton->height() );

   QStringList comboBoxText;
   comboBoxText << tr("None")
                << tr("Read/Write Selection Only") << tr("Read/Write Clipboard Only")
                << tr("Write Both/Read Selection") << tr("Write Both/Read Clipboard");
   mpClipboardSelection->addItems( comboBoxText );

   mainLayout->addWidget( mpUseSatellite,            0, 0, 1, 4 );
   mainLayout->addWidget( mpSatellitePortLabel,      1, 0, 1, 2 );
   mainLayout->addWidget( mpSatellitePort,           1, 2, 1, 2 );
   mainLayout->addWidget( mpUseGlobalStyleSheetFile, 2, 0, 1, 4 );
   mainLayout->addWidget( mpStyleSheetFileName,      3, 0, 1, 3 );
   mainLayout->addWidget( mpDotButton,               3, 3 );
   mainLayout->addWidget( mpClipboardLabel,          4, 0 );
   mainLayout->addWidget( mpClipboardSelection,      4, 1, 1, 3 );
   mainLayout->addWidget( mpAnimateViews,            5, 0, 1, 4 );
   mainLayout->addWidget( mpNormalizeCase,           6, 0, 1, 4 );
   mainLayout->addWidget( mpNormalizeSpaces,         7, 0, 1, 4 );
   mainLayout->addWidget( mpDoubleClickLabel,        8, 0, 1, 2 );
   mainLayout->addWidget( mpDoubleClickInterval,     8, 2, 1, 2 );

   readSettings();

   mainLayout->setRowStretch( 9, 1 );
   mainLayout->setColumnStretch( 0, 1 );
   mainLayout->setColumnStretch( 1, 1 );

   connect( mpUseSatellite, SIGNAL(clicked(bool)),
            mpSatellitePort, SLOT(setEnabled(bool)) );
   connect( mpUseGlobalStyleSheetFile, SIGNAL(clicked()),
            this, SLOT(updateStyleSheetFileName()) );
   connect( mpStyleSheetFileName, SIGNAL(textChanged(QString)),
            this, SLOT(updateFileNames()) );
   connect( mpDotButton, SIGNAL(clicked()),
            this, SLOT(selectFile()) );
   connect( mpUseSatellite, SIGNAL(clicked(bool)),
            this, SIGNAL(useSatelliteClicked(bool)) );

   setLayout( mainLayout );
}


GlobalConfigWidget::~GlobalConfigWidget()
{
}


void GlobalConfigWidget::setSatelliteClicked( bool isSet )
{
   mpUseSatellite->setChecked( isSet );
}


void GlobalConfigWidget::updateFileNames()
{
   if( mpUseGlobalStyleSheetFile->isChecked() )
   {
      mGlobalStyleSheetFileName = mpStyleSheetFileName->text();
   }
   else
   {
      mApplicationStyleSheetFileName = mpStyleSheetFileName->text();
   }
}


void GlobalConfigWidget::readSettings()
{
   mpUseSatellite->setChecked( Settings::value( Settings::CommonUseSatellite ) );
   mpSatellitePort->setEnabled( Settings::value( Settings::CommonUseSatellite ) );
   mpSatellitePort->setValue( Settings::value( Settings::GlobalSatellitePort ) );
   mpUseGlobalStyleSheetFile->setChecked( Settings::value( Settings::CommonUseGlobalStyleSheetFile ) );
   mGlobalStyleSheetFileName = Settings::value( Settings::GlobalStyleSheetFile );
   mApplicationStyleSheetFileName = Settings::value( Settings::CommonStyleSheetFile );
   mpClipboardSelection->setCurrentIndex( Settings::value( Settings::GlobalClipboardMode ) );
   mpAnimateViews->setChecked( Settings::value( Settings::GlobalAnimateViews ) );
   mpNormalizeCase->setChecked( Settings::value( Settings::GlobalNormalizeCase ) );
   mpNormalizeSpaces->setChecked( Settings::value( Settings::GlobalNormalizeSpaces ) );
   mpDoubleClickInterval->setValue( Settings::value( Settings::GlobalDoubleClickInterval ) );
   updateStyleSheetFileName();
}


void GlobalConfigWidget::writeSettings()
{
   Settings::setValue( Settings::CommonUseSatellite, mpUseSatellite->isChecked() );
   Settings::setValue( Settings::GlobalSatellitePort, mpSatellitePort->value() );
   Settings::setValue( Settings::CommonUseGlobalStyleSheetFile, mpUseGlobalStyleSheetFile->isChecked() );
   Settings::setValue( Settings::GlobalStyleSheetFile, mGlobalStyleSheetFileName );
   Settings::setValue( Settings::CommonStyleSheetFile, mApplicationStyleSheetFileName );
   Settings::setValue( Settings::GlobalClipboardMode, mpClipboardSelection->currentIndex() );
   Settings::setValue( Settings::GlobalAnimateViews, mpAnimateViews->isChecked() );
   Settings::setValue( Settings::GlobalNormalizeCase, mpNormalizeCase->isChecked() );
   Settings::setValue( Settings::GlobalNormalizeSpaces, mpNormalizeSpaces->isChecked() );
   Settings::setValue( Settings::GlobalDoubleClickInterval, mpDoubleClickInterval->value() );

   QApplication::setDoubleClickInterval( mpDoubleClickInterval->value() );

   QFile qssFile( mpUseGlobalStyleSheetFile->isChecked() ?
                     mGlobalStyleSheetFileName :
                     mApplicationStyleSheetFileName );
   if( qssFile.exists() && qssFile.open( QIODevice::ReadOnly ) )
   {
      qApp->setStyleSheet( qssFile.readAll() );
      qssFile.close();
   }
   else
   {
      qApp->setStyleSheet( QString() );
   }
}


void GlobalConfigWidget::updateStyleSheetFileName()
{
   if( mpUseGlobalStyleSheetFile->isChecked() )
   {
      mpUseGlobalStyleSheetFile->setText( tr("Use Global Style Sheet File:") );
      mpStyleSheetFileName->setText( mGlobalStyleSheetFileName );
   }
   else
   {
      mpUseGlobalStyleSheetFile->setText( tr("Use Application Style Sheet File:") );
      mpStyleSheetFileName->setText( mApplicationStyleSheetFileName );
   }
}


void GlobalConfigWidget::selectFile()
{
   QFileDialog fileDialog( this );

   fileDialog.setFileMode( QFileDialog::ExistingFile );
   QFileInfo qfi( mpStyleSheetFileName->text() );
   fileDialog.setDirectory( qfi.absolutePath() );
   fileDialog.setNameFilter("Playlists (*.qss)");
   fileDialog.setReadOnly( true );
   if( fileDialog.exec() )
   {
      mpStyleSheetFileName->setText( fileDialog.selectedFiles().at(0) );
   }
}


void GlobalConfigWidget::showClipboard( bool allow )
{
   mpClipboardLabel->setHidden( !allow );
   mpClipboardSelection->setHidden( !allow );
}


void GlobalConfigWidget::showAnimate( bool allow )
{
   mpAnimateViews->setHidden( !allow );
}


void GlobalConfigWidget::showNormalize( bool allow )
{
   mpNormalizeCase->setHidden( !allow );
   mpNormalizeSpaces->setHidden( !allow );
}


void GlobalConfigWidget::showDoubleClickInterval( bool allow )
{
   mpDoubleClickLabel->setHidden( !allow );
   mpDoubleClickInterval->setHidden( !allow );
}


void GlobalConfigWidget::setClipboard( const QString &text )
{
   QClipboard *clipboard = QApplication::clipboard();

   int mode = Settings::value( Settings::GlobalClipboardMode );
   if( (mode == 1) || (mode == 3) || (mode == 4) )
   {
      clipboard->setText( text, QClipboard::Selection );
   }
   if( (mode == 2) || (mode == 3) || (mode == 4) )
   {
      clipboard->setText( text, QClipboard::Clipboard );
   }
}


QString GlobalConfigWidget::getClipboard()
{
   QClipboard *clipboard = QApplication::clipboard();

   switch( Settings::value( Settings::GlobalClipboardMode ) )
   {
      case 1:
      case 3:
         return clipboard->text( QClipboard::Selection );
      case 2:
      case 4:
         return clipboard->text( QClipboard::Clipboard );
      default:
         return QString();
   }
}


QString GlobalConfigWidget::correctFileName( QString fileName, bool withDir )
{
   fileName.remove( QRegExp("[:?]") );
   fileName.replace( QRegExp("[|\\*]"), "_" );
   fileName.replace( QString("\""), "''" );
   if( withDir )
   {
      fileName.replace( QRegExp("[\\\\/]"), "_" );
   }
   return fileName;
}
