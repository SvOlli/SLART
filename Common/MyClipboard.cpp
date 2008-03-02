/**
 * MyClipboard.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MyClipboard.hpp"
#include "MySettings.hpp"

#include <QtGui>
#include <QHttp>


MyClipboard::MyClipboard( QWidget *parent, bool withLayout )
: QWidget( parent )
, mpLabel( new QLabel( tr("Copy Clicked Filename To Clipboard: "), this ) )
, mpComboBox( new QComboBox( this ) )
{
   mpLabel->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
   
   QStringList comboBoxText;
   comboBoxText << tr("None") << tr("Selection") << tr("Clipboard") << tr("Both");
   mpComboBox->addItems( comboBoxText );
   
   readSettings();
   
   if( withLayout )
   {
      QHBoxLayout *mainLayout = new QHBoxLayout( this );
      
#if QT_VERSION < 0x040300
      mainLayout->setMargin( 0 );
#else
      mainLayout->setContentsMargins( 0, 0, 0, 0 );
#endif
      
      mainLayout->addWidget( mpLabel );
      mainLayout->addWidget( mpComboBox );
      
      setLayout( mainLayout );
   }
}


void MyClipboard::readSettings()
{
   QSettings settings( QApplication::organizationName(), "Global" );
   mpComboBox->setCurrentIndex( settings.value("ClipboardMode", 0).toInt() );
}


void MyClipboard::writeSettings()
{
   QSettings settings( QApplication::organizationName(), "Global" );
   settings.setValue( "ClipboardMode", mpComboBox->currentIndex() );
}


void MyClipboard::set( const QString &text )
{
   QSettings settings( QApplication::organizationName(), "Global" );
   QClipboard *clipboard = QApplication::clipboard();
   int mode = settings.value( "ClipboardMode", 0 ).toInt();
   if( mode & 1 )
   {
      clipboard->setText( text, QClipboard::Selection );
   }
   if( mode & 2 )
   {
      clipboard->setText( text, QClipboard::Clipboard );
   }
}


QString MyClipboard::get()
{
   QSettings settings( QApplication::organizationName(), "Global" );
   QClipboard *clipboard = QApplication::clipboard();
   int mode = settings.value( "ClipboardMode", 0 ).toInt();
   if( mode & 1 )
   {
      return clipboard->text( QClipboard::Selection );
   }
   if( mode & 2 )
   {
      return clipboard->text( QClipboard::Clipboard );
   }
   return QString();
}
