/*
 * src/libs/Common/Translate.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "Translate.hpp"

/* system headers */

/* Qt headers */
#include <QActionGroup>
#include <QCoreApplication>
#include <QDir>
#include <QLibraryInfo>
#include <QTranslator>

/* local library headers */

/* local headers */

/* class variables */


Translate::Translate( QObject *parent )
: QObject( parent )
{
}


Translate::~Translate()
{
}


void Translate::install( const QString &lang )
{
   QList<QTranslator*> translators( findChildren<QTranslator*>() );
   foreach( QTranslator *t, translators )
   {
      delete t;
   }

   load( "qt", lang );
   load( "Common", lang );
   load( QCoreApplication::instance()->applicationName(), lang );
}


void Translate::load( const QString &catalog, const QString &lang )
{
   QTranslator *translator = new QTranslator( this );

   QString filename = QString( "%1_%2" ).arg( catalog, lang );
   if( !translator->load( filename, location() ) )
   {
      if( !translator->load( filename, QLibraryInfo::location( QLibraryInfo::TranslationsPath ) ) )
      {
         delete translator;
         translator = 0;
      }
   }
   if( translator )
   {
      QCoreApplication::instance()->installTranslator( translator );
   }
}


QString Translate::location()
{
   QDir dir( QCoreApplication::instance()->applicationDirPath() );
   /* try to change to translations directory */
#if defined Q_OS_MAC
   dir.cd( "../Resources/translations" );
#elif defined Q_OS_UNIX
   dir.cd( "../share/slart/translations" );
#endif
   /* if it failed, path of application is returned */
   return dir.absolutePath();
}


QActionGroup *Translate::langGroup()
{
   // search for the only instance of Translate
   QList<Translate*> translates( QCoreApplication::instance()->findChildren<Translate*>() );
   if( translates.size() != 1 )
   {
      qWarning( "found %d Translate instances, expected 1", translates.size() );
      return 0;
   }
   Translate *translate = translates.takeFirst();

   QActionGroup* group = new QActionGroup( translate );

   group->setExclusive( true );

   connect( group, SIGNAL(triggered(QAction*)),
            translate, SLOT(languageChanged(QAction*)) );

   QString defaultLocale = QLocale::system().name();
   defaultLocale.truncate( defaultLocale.lastIndexOf('_') );

   QDir dir( location() );
   QString mask("%1_*.qm");
   mask = mask.arg( QCoreApplication::instance()->applicationName() );
   QStringList fileNames( dir.entryList( QStringList( mask ) ) );

   foreach( const QString &fileName, fileNames )
   {
      QString locale( fileName );
      QRegExp re( ".*_([a-z]*).qm" );
      locale.replace( re, "\\1" );

      QString lang( QLocale::languageToString( QLocale( locale ).language() ) );
      //QIcon icon( QString("%1/%2.png").arg( location(), locale ) );
      //QAction *action = new QAction( icon, lang, langGroup );
      QAction *action = new QAction( lang, group );
      action->setCheckable( true );
      action->setData( locale );
      group->addAction( action );

      if( locale == defaultLocale )
      {
         action->setChecked( true );
      }
   }

   return group;
}


void Translate::languageChanged( QAction* action )
{
   if( action )
   {
      install( action->data().toString() );
   }
}
