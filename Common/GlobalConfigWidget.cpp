/**
 * GlobalConfigWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "GlobalConfigWidget.hpp"
#include "MySettings.hpp"

#include <QtGui>


GlobalConfigWidget::GlobalConfigWidget( QWidget *parent )
: QWidget( parent )
, mpClipboardLabel( new QLabel( tr("Use Clipboard: "), this ) )
, mpClipboardSelection( new QComboBox( this ) )
, mpAnimateViews( new QCheckBox( tr("Animate Views"), this ) )
, mpNormalizeCase( new QCheckBox( tr("Normalize Case In Text"), this ) )
, mpNormalizeSpaces( new QCheckBox( tr("Normalize Spaces In Text"), this ) )
, mpDoubleClickLabel( new QLabel( tr("Double Click Interval:"), this ) )
, mpDoubleClickInterval( new QSpinBox( this ) )
{
   QGridLayout *mainLayout = new QGridLayout( this );
   
   showClipboard( false );
   showAnimate( false );
   showNormalize( false );
   showDoubleClickInterval( false );
   
   mpDoubleClickInterval->setRange( 100, 1000 );
   mpDoubleClickInterval->setSingleStep( 50 );
   mpDoubleClickInterval->setSuffix( tr("ms") );
   mpDoubleClickInterval->setAlignment( Qt::AlignRight );
   
   QStringList comboBoxText;
   comboBoxText << tr("None") 
                << tr("Read/Write Selection Only") << tr("Read/Write Clipboard Only") 
                << tr("Write Both/Read Selection") << tr("Write Both/Read Clipboard");
   mpClipboardSelection->addItems( comboBoxText );
   
   mainLayout->addWidget( mpClipboardLabel,      0, 0 );
   mainLayout->addWidget( mpClipboardSelection,  0, 1, 1, 2 );
   mainLayout->addWidget( mpAnimateViews,        1, 0, 1, 3 );
   mainLayout->addWidget( mpNormalizeCase,       2, 0, 1, 3 );
   mainLayout->addWidget( mpNormalizeSpaces,     3, 0, 1, 3 );
   mainLayout->addWidget( mpDoubleClickLabel,    4, 0, 1, 2 );
   mainLayout->addWidget( mpDoubleClickInterval, 4, 2 );
   
   readSettings();
   
   mainLayout->setRowStretch( 5, 1 );
   
   setLayout( mainLayout );
}


GlobalConfigWidget::~GlobalConfigWidget()
{
}


void GlobalConfigWidget::readSettings()
{
   MySettings settings( "Global" );
   mpClipboardSelection->setCurrentIndex( settings.VALUE_CLIPBOARDMODE );
   mpAnimateViews->setChecked( settings.VALUE_ANIMATEVIEWS );
   mpNormalizeCase->setChecked( settings.VALUE_NORMALIZECASE );
   mpNormalizeSpaces->setChecked( settings.VALUE_NORMALIZESPACES );
   mpDoubleClickInterval->setValue( settings.VALUE_DOUBLECLICKINTERVAL );
}


void GlobalConfigWidget::writeSettings()
{
   MySettings settings( "Global" );
   settings.setValue( "ClipboardMode", mpClipboardSelection->currentIndex() );
   settings.setValue( "AnimateViews",  mpAnimateViews->isChecked() );
   settings.setValue( "NormalizeCase", mpNormalizeCase->isChecked() );
   settings.setValue( "NormalizeSpaces", mpNormalizeSpaces->isChecked() );
   settings.setValue( "DoubleClickInterval", mpDoubleClickInterval->value() );
   QApplication::setDoubleClickInterval( mpDoubleClickInterval->value() );
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
