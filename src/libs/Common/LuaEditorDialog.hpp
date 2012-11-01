/*
 * src/libs/Common/LuaEditorDialog.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef LUAEDITORDIALOG_HPP
#define LUAEDITORDIALOG_HPP LUAEDITORDIALOG_HPP

/* base class */
#include <QDialog>

/* system headers */

/* Qt headers */
#include <QStringList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QComboBox;
class QLabel;
class QListWidget;
class QPushButton;

/* forward declaration of local classes */
class MyLua;
class CodeEditor;

/*!
  \addtogroup Common

  @{
*/

/*!
 \brief small editor for Lua scripts

 \todo complete documentation

 \dotfile "graphs/libs/Common/LuaEditorDialog_connect.dot" "Connect Graph"
*/
class LuaEditorDialog : public QDialog
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param scriptType type ("family") of scripts to edit
    \param parent
    \param flags
   */
   LuaEditorDialog( const QString &scriptType, QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   /*!
    \brief destructor

   */
   virtual ~LuaEditorDialog();

   /*!
    \brief create a button that will open an editor dialog

    Also creates the editor itself, as a child of the button

    \param scriptType passed on to LuaEditorDialog::LuaEditorDialog( scriptType, ... )
    \param parent
    \return QPushButton newly created button
   */
   static QPushButton *newEditorButton( const QString &scriptType, QWidget *parent = 0 );

public slots:
   /*!
    \brief set all texts (used to re-translate after language change)

   */
   void setTexts();

   /*!
    \brief update the drop down select box with all available scripts

   */
   void updateSelectBox();

   /*!
    \brief load current script (name taken from mpSelectBox), target for mpLoadButton click signal

   */
   void loadScript();

   /*!
    \brief save current script (name taken from mpSelectBox), target for mpSaveButton click signal

   */
   void saveScript();

   /*!
    \brief run the current script in editor context, target for mpRunButton click signal

   */
   void runEditor();

   /*!
    \brief handle successful run of Lua script

   */
   void runSucceeded();

   /*!
    \brief handle unsuccessful run of Lua script

    \param msg error message
   */
   void runFailed( const QString &msg );

   /*!
    \brief write a message in message buffer

    \param msg message to write
    \param error message is an error message which get a different font color
   */
   void log( const QString &msg, bool error = false );

   /*!
    \brief workaround for (at least) compiz window manager: set position of dialog a second time

   */
   void repos();

signals:
   /*!
    \brief send code to Lua for execution

    \param code code to execute
    \param target target which should get called on completion
    \param runSucceeded method to call on success parameters: ()
    \param runFailed method to call on success parameters: (const QString &errmsg)
   */
   void runCode( const QString &code, QObject *target,
                 const QString &runSucceeded, const QString &runFailed );

   /*!
    \brief used to signalize if the editor widget is visible

    \param shown
   */
   void visible( bool shown );

protected:
   /*!
    \brief overloaded from QDialog to emit visible( true )

    \param e event passed on to QDialog::showEvent(QShowEvent *)
   */
   void showEvent( QShowEvent *e );

   /*!
    \brief overloaded from QDialog to emit visible( false )

    \param e event passed on to QDialog::hideEvent(QHideEvent *)
   */
   void hideEvent( QHideEvent *e );

private:
   Q_DISABLE_COPY( LuaEditorDialog )

   QLabel               *mpSelectText; /*!< \brief text for select box */
   QComboBox            *mpSelectBox; /*!< \brief selectbox with script names */
   QPushButton          *mpLoadButton; /*!< \brief load button */
   QPushButton          *mpSaveButton; /*!< \brief save button */
   QPushButton          *mpRunButton; /*!< \brief run button */
   QPushButton          *mpShowOutputButton; /*!< \brief show output button */
   CodeEditor           *mpEditor; /*!< \brief editor widget */
   MyLua                *mpLua; /*!< \brief handle for Lua engine */
   QListWidget          *mpMessageBuffer; /*!< \brief log messages */
   bool                 mMessageBufferAlternateColor; /*!< \brief indicate if next log message will get different background color */

   QString              mScriptType; /*!< \brief script type ("family") to edit */
   QPoint               mLastPos; /*!< \brief remember last position of dialog for reappearance */
};

/*! @} */

#endif
