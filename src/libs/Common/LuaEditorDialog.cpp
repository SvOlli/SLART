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
#include <QPushButton>

/* local library headers */
#include <CodeEditor.hpp>
#include <MyLua.hpp>
#include <SingleInstance.hpp>
#include <SyntaxHighlighterLua.hpp>

/* local headers */


LuaEditorDialog::LuaEditorDialog( const QString &scriptType, QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpSelectText( new QLabel( this ) )
, mpSelectBox( new QComboBox( this ) )
, mpLoadButton( new QPushButton( this ) )
, mpSaveButton( new QPushButton( this ) )
, mpEditor( new CodeEditor )
, mScriptType( scriptType )
{
   setTexts();

   new SyntaxHighlighterLua( mpEditor->document() );

   mpSelectBox->setEditable( true );
   updateSelectBox();
   mpLoadButton->setShortcut( QKeySequence("Ctrl+L") );
   mpSaveButton->setShortcut( QKeySequence("Ctrl+S") );

   QGridLayout *layout( new QGridLayout( this ) );
   layout->addWidget( mpSelectText,   0, 0 );
   layout->addWidget( mpSelectBox,    0, 1 );
   layout->addWidget( mpLoadButton,   0, 2 );
   layout->addWidget( mpSaveButton,   0, 3 );
   layout->addWidget( mpEditor,       1, 0, 1, 4 );

   connect( mpSelectBox, SIGNAL(activated(QString)),
            mpSelectBox, SLOT(setEditText(QString)) );
   connect( mpLoadButton, SIGNAL(clicked()),
            this, SLOT(loadScript()) );
   connect( mpSaveButton, SIGNAL(clicked()),
            this, SLOT(saveScript()) );

   MyLua *lua = SingleInstance::get<MyLua>( "MyLua", false );
   lua->start();

   setLayout( layout );
}


LuaEditorDialog::~LuaEditorDialog()
{
}


void LuaEditorDialog::setTexts()
{
   setWindowTitle( QApplication::applicationName() + ": " + tr("Lua Script Editor") );
   mpSelectText->setText( tr("Edit %1 script:").arg( mScriptType ) );
   mpLoadButton->setText( tr("Load") );
   mpSaveButton->setText( tr("Save") );
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
