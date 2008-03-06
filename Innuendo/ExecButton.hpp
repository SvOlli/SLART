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

private slots:
   void handleClick();
   void handleError( QProcess::ProcessError error );
   void handleFinished( int exitCode, QProcess::ExitStatus exitStatus );

private:
   ExecButton( const ExecButton &other );
   ExecButton &operator=( const ExecButton &other );

   QString   mName;
   QProcess  mProcess;
   bool      mTerminating;
};

#endif
