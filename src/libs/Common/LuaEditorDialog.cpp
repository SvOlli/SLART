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
#include <QSplitter>
#include <QTimer>

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
, mLastPos()
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
   mpShowOutputButton->setShortcut( QKeySequence("F6") );

   QSplitter *splitter = new QSplitter( Qt::Vertical, this );
   splitter->addWidget( mpEditor );
   splitter->addWidget( mpMessageBuffer );

   QGridLayout *layout( new QGridLayout( this ) );
   layout->addWidget( mpSelectText,       0, 0 );
   layout->addWidget( mpSelectBox,        0, 1 );
   layout->addWidget( mpLoadButton,       0, 2 );
   layout->addWidget( mpSaveButton,       0, 3 );
   layout->addWidget( mpRunButton,        0, 4 );
   layout->addWidget( mpShowOutputButton, 0, 5 );
   layout->addWidget( splitter,           1, 0, 1, 6 );

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
   connect( this, SIGNAL(runCode(QObject*,QString,QString,QString)),
            mpLua, SLOT(runCode(QObject*,QString,QString,QString)) );

   mpLua->start();

   setLayout( layout );
}


LuaEditorDialog::~LuaEditorDialog()
{
}


QPushButton *LuaEditorDialog::newEditorButton( const QString &scriptType, QWidget *parent )
{
   QPushButton *button = new QPushButton( tr("Open Lua Editor"), parent );

   LuaEditorDialog *editor = new LuaEditorDialog( scriptType, button );

   button->setCheckable( true );

   connect( button, SIGNAL(clicked(bool)),
            editor, SLOT(setVisible(bool)) );
   connect( editor, SIGNAL(visible(bool)),
            button, SLOT(setChecked(bool)) );

   return button;
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

void LuaEditorDialog::repos()
{
   move( mLastPos );
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
   mpShowOutputButton->setChecked( true );
   mpMessageBuffer->show();

   emit runCode( this, "runSucceeded", "runFailed", mpEditor->toPlainText() );
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


void LuaEditorDialog::showEvent( QShowEvent *e )
{
   QDialog::showEvent( e );
   if( !mLastPos.isNull() )
   {
      move( mLastPos );
      /* workaround for at least compiz */
      QTimer::singleShot( 50, this, SLOT(repos()) );
   }
   emit visible( true );
}

void LuaEditorDialog::hideEvent( QHideEvent *e )
{
   QDialog::hideEvent( e );
   mLastPos = pos();
   emit visible( false );
}
