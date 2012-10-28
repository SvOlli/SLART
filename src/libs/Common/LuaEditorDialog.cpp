/*
 * src/libs/Common/LuaEditorDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "LuaEditorDialog.hpp"

/* system headers */

/* Qt headers */
#include <QApplication>
#include <QComboBox>
#include <QGridLayout>
#include <QInputDialog>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>

/* local library headers */
#include <CodeEditor.hpp>
#include <MyLua.hpp>
#include <SingleInstance.hpp>
#include <SyntaxHighlighterLua.hpp>
#include <TagMap.hpp>

/* local headers */


LuaEditorDialog::LuaEditorDialog( const QString &scriptType, QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpSelectText( new QLabel( this ) )
, mpSelectBox( new QComboBox( this ) )
, mpLoadButton( new QPushButton( this ) )
, mpSaveButton( new QPushButton( this ) )
, mpRunButton( new QPushButton( this ) )
, mpShowOutputButton( new QPushButton( this ) )
, mpEditor( new CodeEditor )
, mpLua( SingleInstance::get<MyLua>( "MyLua", false ) )
, mpMessageBuffer( new QListWidget( this ) )
, mMessageBufferAlternateColor( false )
, mScriptType( scriptType )
{
   mpShowOutputButton->setCheckable( true );
   mpMessageBuffer->hide();
   setTexts();

   new SyntaxHighlighterLua( mpEditor->document() );

   mpSelectBox->setEditable( true );
   updateSelectBox();
   mpLoadButton->setShortcut( QKeySequence("Ctrl+L") );
   mpSaveButton->setShortcut( QKeySequence("Ctrl+S") );
   mpRunButton->setShortcut( QKeySequence("F5") );

   QGridLayout *layout( new QGridLayout( this ) );
   layout->addWidget( mpSelectText,       0, 0 );
   layout->addWidget( mpSelectBox,        0, 1 );
   layout->addWidget( mpLoadButton,       0, 2 );
   layout->addWidget( mpSaveButton,       0, 3 );
   layout->addWidget( mpRunButton,        0, 4 );
   layout->addWidget( mpShowOutputButton, 0, 5 );
   layout->addWidget( mpEditor,           1, 0, 1, 6 );
   layout->addWidget( mpMessageBuffer,    2, 0, 1, 6 );

   connect( mpSelectBox, SIGNAL(activated(QString)),
            mpSelectBox, SLOT(setEditText(QString)) );
   connect( mpLoadButton, SIGNAL(clicked()),
            this, SLOT(loadScript()) );
   connect( mpSaveButton, SIGNAL(clicked()),
            this, SLOT(saveScript()) );
   connect( mpRunButton, SIGNAL(clicked()),
            this, SLOT(runEditor()) );
   connect( mpShowOutputButton, SIGNAL(clicked(bool)),
            mpMessageBuffer, SLOT(setVisible(bool)) );
   connect( this, SIGNAL(runCode(QString,QObject*,QString,QString)),
            mpLua, SLOT(runCode(QString,QObject*,QString,QString)) );

   mpLua->start();

   setLayout( layout );
}


LuaEditorDialog::~LuaEditorDialog()
{
}


void LuaEditorDialog::log( const QString &msg, bool error )
{
   QListWidgetItem *item = new QListWidgetItem( msg, mpMessageBuffer );
   if( error )
   {
      item->setForeground( Qt::red );
   }
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


void LuaEditorDialog::setTexts()
{
   setWindowTitle( QApplication::applicationName() + ": " + tr("Lua Script Editor") );
   mpSelectText->setText( tr("Edit %1 script:").arg( mScriptType ) );
   mpLoadButton->setText( tr("Load") );
   mpSaveButton->setText( tr("Save") );
   mpRunButton->setText( tr("Run") );
   mpShowOutputButton->setText( tr("Show Output") );
}


void LuaEditorDialog::loadScript()
{
   mpEditor->setPlainText( MyLua::script( mScriptType, mpSelectBox->currentText() ));
   updateSelectBox();
}


void LuaEditorDialog::saveScript()
{
   MyLua::setScript( mScriptType, mpSelectBox->currentText(), mpEditor->toPlainText() );
   updateSelectBox();
}


void LuaEditorDialog::updateSelectBox()
{
   QString current( mpSelectBox->currentText() );

   mpSelectBox->clear();
   mpSelectBox->addItems( MyLua::scriptNames( mScriptType ) );
   int index = mpSelectBox->findText( current );
   if( index >= 0 )
   {
      mpSelectBox->setCurrentIndex( index );
   }
}

void LuaEditorDialog::runEditor()
{
   TagMap tagMap;
   tagMap.insert("TRACKNUMBER","1");
   tagMap.insert("ALBUMARTIST","AlbumArtist");
   tagMap.insert("ALBUM","Album");
   tagMap.insert("ARTIST","Artist");
   tagMap.insert("TITLE","Title");
   tagMap.insert("GENRE","Genre");
   tagMap.insert("DATE","1986");
   tagMap.insert("EXTENSION","mp3");
   tagMap.insert("DIRECTORY","/path/to");
   tagMap.insert("FILENAME","filename.ext");

   mpMessageBuffer->clear();
   mMessageBufferAlternateColor = false;

   mpLua->mutex( true );
   connect( mpLua, SIGNAL(print(QString)),
            this, SLOT(log(QString)) );
   mpLua->setTable( "tags", tagMap.toLuaTable() );
   emit runCode( mpEditor->toPlainText(), this, "runSucceeded", "runFailed" );
}

void LuaEditorDialog::runSucceeded()
{
   log( QString("dirname=\"%1\"\nfilename=\"%2\"").arg(mpLua->getString("dirname"),mpLua->getString("filename")) );
   QStringList logTable;
   MyLuaTable table( mpLua->getTable( "tags" ) );
   foreach( const QString &key, table.keys() )
   {
      logTable.append( QString("%1=%2").arg( key, table.value(key) ) );
   }
   log( logTable.join("\n") );
   disconnect( mpLua, SIGNAL(print(QString)),
               this, SLOT(log(QString)) );
   mpLua->mutex()->unlock();
}


void LuaEditorDialog::runFailed( const QString &msg )
{
   log( msg, true );
   disconnect( mpLua, SIGNAL(print(QString)),
               this, SLOT(log(QString)) );
   mpLua->mutex()->unlock();
}
