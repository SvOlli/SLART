/*
 * src/apps/Innuendo/ExecButton.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef EXECBUTTON_HPP
#define EXECBUTTON_HPP EXECBUTTON_HPP

/* base class */
#include <QPushButton>

/* system headers */

/* Qt headers */
#include <QProcess>
#include <QString>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Innuendo

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/apps/Innuendo/ExecButton_connect.dot" "Connect Graph"
*/
class ExecButton : public QPushButton
{
   Q_OBJECT

public:
   ExecButton( const QString &name, QWidget *parent = 0 );
   virtual ~ExecButton();

private slots:
   /*!
    \brief handle click on button

   */
   void handleClick();
   /*!
    \brief callback for QProcess

   */
   void handleError( QProcess::ProcessError error );
   /*!
    \brief callback for QProcess

   */
   void handleFinished( int exitCode, QProcess::ExitStatus exitStatus );

private:
   Q_DISABLE_COPY( ExecButton )

   QString   mName; /*!< \brief \todo TODO */
   QProcess  *mpProcess; /*!< \brief \todo TODO */
};

/*! @} */

#endif
