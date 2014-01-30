// Author: Justin Fung

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <iostream>
#include <fstream>
#include "funix.h"
#include "directory.h"

using namespace std;

Funix::Funix()
{
  currentDirectory = new Directory("/", NULL, "root");
//  ifstream inf("directories.txt");

//  if(inf)
//    inf >> *this;
//  else
//  {
    user = new char[strlen("root") + 1];
    strcpy(user, "root");
    const char *argv[] = {"umask", "22"};
    Permissions::setUmask(2, argv);
//  } // else no directories.txt
} // Funix()


Funix::~Funix()
{
  const char *arguments[] = {"cd", ".."};
  Directory *temp;

  temp = currentDirectory->cd(2, arguments, user);

  while(temp != currentDirectory)
  {
    currentDirectory = temp;
    temp = currentDirectory->cd(2, arguments, user);
  } // while still moving up directory structure.

  delete currentDirectory;
} // ~Funix()


void Funix::cd( int argCount, const char *arguments[])
{
  currentDirectory = currentDirectory->cd(argCount, arguments, user);
} // cd()


void Funix::chmod( int argCount, const char *arguments[])
{
  currentDirectory->chmod(argCount, arguments, user);
} // chmod()


void Funix::chown( int argCount, const char *arguments[])
{
  currentDirectory->chownCMD(argCount, arguments, user);
} // chown()


void Funix::cp( int argCount, const char *arguments[])
{
  currentDirectory->cp(argCount, arguments, user);
} // cp()


int Funix::eXit( int argCount, const char *arguments[])
{
  const char *args[] = {"cd", ".."};

  if(argCount == 1) // proper
  {
    Directory *temp;

    temp = currentDirectory->cd(2, args, user);

    while(temp != currentDirectory)
    {
      currentDirectory = temp;
      temp = currentDirectory->cd(2, args, user);
    } // while still moving up directory structure.

    ofstream outf("directories.txt");
    outf << *this;
    outf.close();
    return 0;
  } // if proper number of arguments

  cout << "usage: exit\n";
  return 1;
} // eXit()


void Funix::getCommand( char *command)
{
  writePrompt();
  fgets(command, 80, stdin);

  if(strlen(command))
    command[strlen(command) - 1] = '\0';  // eliminate '\n'

} // getCommand()


void Funix::ls( int argCount, const char *arguments[])
{
  currentDirectory->ls(argCount, arguments, user);
} // ls()


void Funix::mkdir( int argCount, const char *arguments[])
{
  currentDirectory->mkdir( argCount, arguments, user);
} // mkdir()


void Funix::touch( int argCount, const char *arguments[] )
{
  currentDirectory->touch( argCount, arguments, user);
} // touch()


int Funix::processCommand( char *command)
{
  static const char *commands[] = {"cd", "chmod", "chown", "cp", "exit", "ls",
    "mkdir", "su", "touch", "umask"};
  char *ptr;
  const char* arguments[40];
  int commandNum, argCount = 0;

  ptr = strtok(command, " \n");

  while(ptr)
  {
    arguments[argCount++] = ptr;
    ptr = strtok(NULL, " \n");
  } // while more arguments in command

  if(argCount > 0)
  {
    for(commandNum = 0; commandNum < 10; commandNum++)
      if(!strcmp(arguments[0], commands[commandNum]))
        break;

    switch(commandNum)
    {
      case 0 : cd(argCount, arguments); break;
      case 1 : chmod(argCount, arguments); break;
      case 2 : chown(argCount, arguments); break;
      case 3 : cp(argCount, arguments); break;
      case 4 : return eXit(argCount, arguments);
      case 5 : ls(argCount, arguments); break;
      case 6 : mkdir(argCount, arguments); break;
      case 7 : su(argCount, arguments); break;
      case 8 : touch(argCount, arguments); break;
      case 9 : Permissions::setUmask(argCount, arguments); break;
      default : cout << arguments[0] << ": Command not found.\n";
    } // switch()
  } // if there are arguments

  return 1; // not exit
} // processCommand()


void Funix::run()
{
  char command[80];
  getCommand(command);

  while(processCommand(command))
  {
    getCommand(command);
  } // while not an exit
} // run()


void Funix::su( int argCount, const char *arguments[])
{
  if(argCount != 2)
  {
    cout << "usage: su user_name\n";
    return;
  } // if incorrect number of arguments

  delete user;
  user = new char[strlen(arguments[1]) + 1];
  strcpy(user, arguments[1]);
} // su()


void Funix::writePrompt()
{
  currentDirectory->showPath();
  cout << " # ";
} // writePrompt()


istream& operator>> (istream &is, Funix &funix)
{
  char s[256];
  is >> s;
  funix.user = new char[strlen(s) + 1];
  strcpy(funix.user, s);
  is.ignore();
  Permissions::readUmask(is);
  is >> *(funix.currentDirectory);
  return is;
} // operator>>


ostream& operator<< (ostream &os, Funix &funix)
{
  os<< funix.user << ' ';
  Permissions::writeUmask(os);
  os << 1 << ' ' << *(funix.currentDirectory);
  return os;
} // operator<<