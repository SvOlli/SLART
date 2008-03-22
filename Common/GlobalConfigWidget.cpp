/**
 * GlobalConfigWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "GlobalConfigWidget.hpp"
#include <QSettings>

#include <QtGui>


GlobalConfigWidget::GlobalConfigWidget( QWidget *parent )
: QGroupBox( parent )
, mpClipboardLabel( new QLabel( tr("Use Clipboard: "), this ) )
, mpClipboardSelection( new QComboBox( this ) )
, mpAnimateViews( new QCheckBox( tr("Animate Views"), this ) )
{
   QGridLayout *mainLayout = new QGridLayout( this );
   setTitle( tr("Global Settings:") );
   
   showClipboard( false );
   showAnimate( false );
   
   QStringList comboBoxText;
   comboBoxText << tr("None") 
                << tr("Read/Write Selection Only") << tr("Read/Write Clipboard Only") 
                << tr("Write Both/Read Selection") << tr("Write Both/Read Clipboard");
   mpClipboardSelection->addItems( comboBoxText );
   
   mainLayout->addWidget( mpClipboardLabel,     0, 0 );
   mainLayout->addWidget( mpClipboardSelection, 0, 1 );
   mainLayout->addWidget( mpAnimateViews,       1, 0, 1, 2 );
 
   readSettings();
   
   setLayout( mainLayout );
}


GlobalConfigWidget::~GlobalConfigWidget()
{
}


void GlobalConfigWidget::readSettings()
{
   QSettings settings( QApplication::organizationName(), "Global" );
   mpClipboardSelection->setCurrentIndex( settings.value( "ClipboardMode", 0 ).toInt() );
   mpAnimateViews->setChecked( settings.value( "AnimateViews", false ).toBool() );
}


void GlobalConfigWidget::writeSettings()
{
   QSettings settings( QApplication::organizationName(), "Global" );
   settings.setValue( "ClipboardMode", mpClipboardSelection->currentIndex() );
   settings.setValue( "AnimateViews",  mpAnimateViews->isChecked() );
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


void GlobalConfigWidget::setClipboard( const QString &text )
{
   QSettings settings( QApplication::organizationName(), "Global" );
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
   QSettings settings( QApplication::organizationName(), "Global" );
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