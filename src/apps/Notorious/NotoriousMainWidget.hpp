/*
 * src/apps/Notorious/MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QLineEdit;
class QPushButton;

/* forward declaration of local classes */
class NotoriousConfigDialog;
class FreeDB;
class Foundlist;
class SearchWidget;

/*!
  \addtogroup Notorious

  \brief application: FreeDB browser
  @{
  */


/*!
 \brief

 \dotfile "graphs/apps/Notorious/NotoriousMainWidget_connect.dot" "Connect Graph"
*/
class NotoriousMainWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief

    \param parent
    \param flags
   */
   NotoriousMainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

protected:

public slots:
   /*!
    \brief

    \param query
   */
   void runQuery( const QString &query );
   /*!
    \brief

    \param category
    \param id
   */
   void handleClicked( const QString &category, const QString &id );

signals:

private:
   Q_DISABLE_COPY( NotoriousMainWidget )

   FreeDB                  *mpFreeDB; /*!< \brief \todo complete documentation */
   SearchWidget            *mpSearch; /*!< \brief \todo complete documentation */
   Foundlist               *mpFound; /*!< \brief \todo complete documentation */
   Foundlist               *mpCDInfo; /*!< \brief \todo complete documentation */
   NotoriousConfigDialog   *mpConfig; /*!< \brief \todo complete documentation */
   QPushButton             *mpSettingsButton; /*!< \brief \todo complete documentation */
};

/*! @} */

#endif
