/**
 * src/libs/Common/GlobalConfigWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#include "GlobalConfigWidget.hpp"
#include "MySettings.hpp"
#include "Satellite.hpp"

#include <QtGui>


GlobalConfigWidget::GlobalConfigWidget( QWidget *parent )
: QWidget( parent )
, mpUseSatellite( new QCheckBox( tr("Use Satellite (Inter-Process Communication)"), this ) )
, mpSatellitePortLabel( new QLabel( tr("Satellite Port"), this ) )
, mpSatellitePort( new QSpinBox( this ) )
, mpUseGlobalStyleSheetFile( new QCheckBox( this ) )
, mpStyleSheetFileName( new QLineEdit( this ) )
, mpDotButton( new QPushButton( tr("..."), this ) )
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
   completer->setModel( new QDirModel( completer ) );
   mpStyleSheetFileName->setCompleter( completer );

   showClipboard( false );
   showAnimate( false );
   showNormalize( false );
   showDoubleClickInterval( false );

   mpSatellitePort->setRange( 1025, 65535 );
   mpSatellitePort->setAlignment( Qt::AlignRight );

   mpDoubleClickInterval->setRange( 100, 1000 );
   mpDoubleClickInterval->setSingleStep( 50 );
   mpDoubleClickInterval->setSuffix( tr("ms") );
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


void GlobalConfigWidget::readSettings()
{
   MySettings settings( "Global" );
   MySettings appSettings;
   mpUseSatellite->setChecked( appSettings.VALUE_USE_SATELLITE );
   mpSatellitePort->setEnabled( appSettings.VALUE_USE_SATELLITE );
   mpSatellitePort->setValue( settings.VALUE_SATELLITE_PORT );
   mpUseGlobalStyleSheetFile->setChecked( appSettings.VALUE_USEGLOBALSTYLESHEETFILE );
   if( mpUseGlobalStyleSheetFile->isChecked() )
   {
      mpUseGlobalStyleSheetFile->setText( tr("Use Global Style Sheet File:") );
      mpStyleSheetFileName->setText( settings.VALUE_STYLESHEETFILE );
   }
   else
   {
      mpUseGlobalStyleSheetFile->setText( tr("Use Application Style Sheet File:") );
      mpStyleSheetFileName->setText( appSettings.VALUE_STYLESHEETFILE );
   }
   mpClipboardSelection->setCurrentIndex( settings.VALUE_CLIPBOARDMODE );
   mpAnimateViews->setChecked( settings.VALUE_ANIMATEVIEWS );
   mpNormalizeCase->setChecked( settings.VALUE_NORMALIZECASE );
   mpNormalizeSpaces->setChecked( settings.VALUE_NORMALIZESPACES );
   mpDoubleClickInterval->setValue( settings.VALUE_DOUBLECLICKINTERVAL );
}


void GlobalConfigWidget::writeSettings()
{
   MySettings settings( "Global" );
   MySettings appSettings;
   appSettings.setValue( "UseSatellite", mpUseSatellite->isChecked() );
   settings.setValue( "SatellitePort", mpSatellitePort->value() );
   mpSatellitePort->setValue( settings.VALUE_SATELLITE_PORT );
   appSettings.setValue( "UseGlobalStyleSheetFile", mpUseGlobalStyleSheetFile->isChecked() );
   if( mpUseGlobalStyleSheetFile->isChecked() )
   {
      settings.setValue( "StyleSheetFile", mpStyleSheetFileName->text() );
   }
   else
   {
      appSettings.setValue( "StyleSheetFile", mpStyleSheetFileName->text() );
   }
   settings.setValue( "ClipboardMode", mpClipboardSelection->currentIndex() );
   settings.setValue( "AnimateViews",  mpAnimateViews->isChecked() );
   settings.setValue( "NormalizeCase", mpNormalizeCase->isChecked() );
   settings.setValue( "NormalizeSpaces", mpNormalizeSpaces->isChecked() );
   settings.setValue( "DoubleClickInterval", mpDoubleClickInterval->value() );
   
   QApplication::setDoubleClickInterval( mpDoubleClickInterval->value() );
   
   settings.sync();
   
   QFile qssFile( appSettings.styleSheetFile() );
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
      mpStyleSheetFileName->setText( MySettings( "Global" ).VALUE_STYLESHEETFILE );
   }
   else
   {
      mpUseGlobalStyleSheetFile->setText( tr("Use Application Style Sheet File:") );
      mpStyleSheetFileName->setText( MySettings().VALUE_STYLESHEETFILE );
   }
}


void GlobalConfigWidget::selectFile()
{
   QFileDialog fileDialog( this );

   fileDialog.setFileMode( QFileDialog::ExistingFile );
   QFileInfo qfi( mpStyleSheetFileName->text() );
   fileDialog.setDirectory( qfi.absolutePath() );
   fileDialog.setFilter("Playlists (*.qss)");
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
   MySettings settings( "Global" );
   QClipboard *clipboard = QApplication::clipboard();
   
#if 1
   int mode = settings.value( "ClipboardMode", 0 ).toInt();
   if( (mode == 1) || (mode == 3) || (mode == 4) )
   {
      clipboard->setText( text, QClipboard::Selection );
   }
   if( (mode == 2) || (mode == 3) || (mode == 4) )
   {
      clipboard->setText( text, QClipboard::Clipboard );
   }
#else
   switch( settings.value( "ClipboardMode", 0 ).toInt() )
   {
      case 1:
         clipboard->setText( text, QClipboard::Selection );
         break;
      case 2:
         clipboard->setText( text, QClipboard::Clipboard );
         break;
      case 3:
      case 4:
         clipboard->setText( text, QClipboard::Selection );
         clipboard->setText( text, QClipboard::Clipboard );
         break;
      default:
         break;
   }
#endif
}


QString GlobalConfigWidget::getClipboard()
{
   MySettings settings( "Global" );
   QClipboard *clipboard = QApplication::clipboard();

   switch( settings.value( "ClipboardMode", 0 ).toInt() )
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
