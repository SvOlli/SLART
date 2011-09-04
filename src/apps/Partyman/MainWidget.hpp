/**
 * src/apps/Partyman/MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
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
class QPushButton;

/* forward declaration of local classes */
class ConfigDialog;
class ControlWidget;
class Database;
class PlaylistControlWidget;

/*!
  \addtogroup Partyman Partyman: dual slot audio player
  @{
  */



/*!
 \brief

*/
class MainWidget : public QWidget
{
   Q_OBJECT

public:

   /*!
    \brief

    \param parent
    \param flags
   */
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   /*!
    \brief

   */
   virtual ~MainWidget();

   /* prepare startup of application */
   /*!
    \brief

   */
   void startUp();

protected:

public slots:
   /* handle request for new icon and title */
   /*!
    \brief

    \param icon
    \param title
   */
   void changeTitle( const QIcon &icon, const QString &title );
   /* don't autostart if database is empty */
   /*!
    \brief

    \param allow
   */
   void allowAutostart( bool allow );

signals:
   /* request a new icon and title */
   /*!
    \brief

    \param icon
    \param title
   */
   void requestChangeTitle( const QIcon &icon, const QString &title );

private:
   Q_DISABLE_COPY( MainWidget )

   bool                    mAllowAutostart; /*!< TODO */
   QWidget                 *mpParent; /*!< TODO */
   Database                *mpDatabase; /*!< TODO */
   ConfigDialog            *mpConfig; /*!< TODO */
   PlaylistControlWidget   *mpPlaylist; /*!< TODO */
   ControlWidget           *mpControl; /*!< TODO */
};

/*! @} */

#endif
