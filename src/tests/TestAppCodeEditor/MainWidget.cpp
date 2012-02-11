/*
 * src/tests/TestAppWebServer/MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "MainWidget.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <CodeEditor.hpp>
#include <TagList.hpp>
#include <MyLua.hpp>
#include <SyntaxHighlighterLua.hpp>

/* local headers */


MainWidget::MainWidget( QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpLua( new MyLua( 0 ) )
, mpCodeEditor( new CodeEditor( this ) )
, mpLoadButton( new QPushButton( tr("Open"), this ) )
, mpSaveButton( new QPushButton( tr("Save"), this ) )
, mpRunButton( new QPushButton( tr("Run"), this ) )
, mpMessageBuffer( new QListWidget( this ) )
, mMessageBufferAlternateColor( false )
{
   new SyntaxHighlighterLua( mpCodeEditor->document() );

   QGridLayout *mainLayout   = new QGridLayout( this );
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
   mainLayout->addWidget( mpCodeEditor,    0, 0, 1, 3 );
   mainLayout->addWidget( mpLoadButton,    1, 0 );
   mainLayout->addWidget( mpSaveButton,    1, 1 );
   mainLayout->addWidget( mpRunButton,     1, 2 );
   mainLayout->addWidget( mpMessageBuffer, 2, 0, 1, 3 );
   setLayout( mainLayout );

   connect( mpLoadButton, SIGNAL(clicked()),
            this, SLOT(handleLoad()) );
   connect( mpSaveButton, SIGNAL(clicked()),
            this, SLOT(handleSave()) );
   connect( mpRunButton, SIGNAL(clicked()),
            this, SLOT(handleRun()) );
   connect( mpLua, SIGNAL(error(QString)),
            this, SLOT(log(QString)) );
   connect( mpLua, SIGNAL(print(QString)),
            this, SLOT(log(QString)) );
   connect( this, SIGNAL(runCode(QString)),
            mpLua, SLOT(runCode(QString)) );
   connect( mpLua, SIGNAL(success()),
            this, SLOT(handleDone()) );
   mpLoadButton->setShortcut( QKeySequence("Ctrl+O") );
   mpSaveButton->setShortcut( QKeySequence("Ctrl+S") );
   mpRunButton->setShortcut( QKeySequence("F5") );

   mpLua->start();
}


MainWidget::~MainWidget()
{
   if( mpLua->isRunning() )
   {
      QMetaObject::invokeMethod( mpLua, "quit" );
      mpLua->wait( 1000 );
   }
   mpLua->deleteLater();
}


void MainWidget::log( const QString &msg )
{
   QListWidgetItem *item = new QListWidgetItem( msg, mpMessageBuffer );
   if( mMessageBufferAlternateColor )
   {
      item->setBackground( QBrush( mpMessageBuffer->palette().color( QPalette::AlternateBase ) ) );
   }
   mMessageBufferAlternateColor = !mMessageBufferAlternateColor;
   mpMessageBuffer->addItem( item );

   while( mpMessageBuffer->count() > 500 )
   {
      item = mpMessageBuffer->takeItem(0);
      if( item )
      {
         delete item;
      }
   }

   mpMessageBuffer->scrollToBottom();
}


void MainWidget::handleLoad()
{
   QSettings settings;
   QString path( settings.value("Directory", QDir::homePath()).toString() );
   QString fileName( QFileDialog::getOpenFileName( this, QCoreApplication::applicationName() + ": " +
                                                   tr("Open File"),
                                                   path, //dir
                                                   tr("Lua Script Files (*.lua)")
                                                   ) );
   if( fileName.isEmpty() )
   {
      return;
   }
   path = QDir( fileName ).absolutePath();
   settings.setValue( "Directory", path );
   QFile file( fileName );
   if( !file.open(QIODevice::ReadOnly) )
   {
      QMessageBox::warning( this, tr("Open Failed"), tr("Open Failed") );
   }
   else
   {
      mpCodeEditor->setPlainText( QString::fromUtf8( file.readAll().constData() ) );
      file.close();
   }
}


void MainWidget::handleSave()
{
   QSettings settings;
   QString path( settings.value("Directory", QDir::homePath()).toString() );
   QString fileName( QFileDialog::getSaveFileName( this, QCoreApplication::applicationName() + ": " +
                                                   tr("Save File"),
                                                   path, //dir
                                                   tr("Lua Script Files (*.lua)")
                                                   ) );
   if( fileName.isEmpty() )
   {
      return;
   }
   path = QDir( fileName ).absolutePath();
   settings.setValue( "Directory", path );
   QFile file( fileName );
   if( !file.open(QIODevice::WriteOnly) )
   {
      QMessageBox::warning( this, tr("Save Failed"), tr("Save Failed") );
   }
   else
   {
      file.write( mpCodeEditor->toPlainText().toUtf8() );
      file.close();
   }
}


void MainWidget::handleRun()
{
   TagList mTagList;
   mTagList.set("TRACKNUMBER","1");
   mTagList.set("ALBUMARTIST","AlbumArtist");
   mTagList.set("ALBUM","Album");
   mTagList.set("ARTIST","Artist");
   mTagList.set("TITLE","Title");
   mTagList.set("GENRE","Genre");
   mTagList.set("DATE","1986");
   mTagList.set("EXTENSION","mp3");
   mTagList.set("DIRECTORY","/path/to");
   mTagList.set("FILENAME","filename.ext");

   mpMessageBuffer->clear();
   mMessageBufferAlternateColor = false;

   mpLua->setTable( "tags", mTagList.toLuaTable() );
   emit runCode( mpCodeEditor->toPlainText() );
}


void MainWidget::handleDone()
{
   log( QString("dirname=\"%1\"\nfilename=\"%2\"").arg(mpLua->getString("dirname"),mpLua->getString("filename")) );
   QStringList logTable;
   MyLuaTable table( mpLua->getTable( "tags" ) );
   foreach( const QString &key, table.keys() )
   {
      logTable.append( QString("%1=%2").arg( key, table.value(key) ) );
   }
   log( logTable.join("\n") );
}
