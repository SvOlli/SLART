/**
 * ExecButton.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef EXECBUTTON_HPP
#define EXECBUTTON_HPP EXECBUTTON_HPP

#include <QPushButton>
#include <QProcess>
#include <QString>


class ExecButton : public QPushButton
{
Q_OBJECT

public:
   ExecButton( const QString &name, QWidget *parent = 0 );
   virtual ~ExecButton();

private slots:
   /* handle click on button */
   void handleClick();
   /* callback for QProcess */
   void handleError( QProcess::ProcessError error );
   /* callback for QProcess */
   void handleFinished( int exitCode, QProcess::ExitStatus exitStatus );

private:
   ExecButton( const ExecButton &other );
   ExecButton &operator=( const ExecButton &other );

   QString   mName;
   QProcess  mProcess;
   bool      mTerminating;
};

#endif
