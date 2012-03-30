/*
 * src/apps/Innuendo/DropDialog.cpp
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
#include <QLabel>
#include <QPushButton>

/* local library headers */
#include <CodeEditor.hpp>

/* local headers */


LuaEditorDialog::LuaEditorDialog( const QString &scriptType, QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpSelectText( new QLabel( this ) )
, mpSelectBox( new QComboBox( this ) )
, mpSaveButton( new QPushButton( this ) )
, mpSaveAsButton( new QPushButton( this ) )
, mpEditor( new CodeEditor )
, mScriptType( scriptType )
{
   setWindowTitle( QApplication::applicationName() + ": " + tr("Lua Script Editor") );
   setTexts();
   mpSaveButton->setShortcut( QKeySequence("Ctrl+S") );
   mpSaveAsButton->setShortcut( QKeySequence("Ctrl+Shift+S") );

   QGridLayout *layout( new QGridLayout( this ) );
   layout->addWidget( mpSelectText,   0, 0 );
   layout->addWidget( mpSelectBox,    0, 1 );
   layout->addWidget( mpSaveButton,   0, 2 );
   layout->addWidget( mpSaveAsButton, 0, 3 );
   layout->addWidget( mpEditor,       1, 0, 1, 4 );

   setLayout( layout );
}


LuaEditorDialog::~LuaEditorDialog()
{
}


void LuaEditorDialog::setTexts()
{
   mpSelectText->setText( tr("Edit %1 script:").arg( mScriptType ) );
   mpSaveButton->setText( tr("Save") );
   mpSaveAsButton->setText( tr("Save as") );
}
