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
class QPushButton;

/* forward declaration of local classes */
class MyLua;
class CodeEditor;

/*!
  \addtogroup Common

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/libs/Common/LuaEditorDialog_connect.dot" "Connect Graph"
*/
class LuaEditorDialog : public QDialog
{
   Q_OBJECT

public:
   /*!
    \brief \todo

    \param scriptType
    \param parent
    \param flags
   */
   LuaEditorDialog( const QString &scriptType, QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   /*!
    \brief \todo

    \param parent
    \param flags
   */
   virtual ~LuaEditorDialog();

public slots:
   /*!
    \brief set all texts (used to re-translate after language change)

   */
   void setTexts();

private:
   Q_DISABLE_COPY( LuaEditorDialog )

   QLabel               *mpSelectText;
   QComboBox            *mpSelectBox;
   QPushButton          *mpSaveButton;
   QPushButton          *mpSaveAsButton;
   CodeEditor           *mpEditor;

   QString              mScriptType;
};

/*! @} */

#endif
